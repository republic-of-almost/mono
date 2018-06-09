#include <roa_renderer/roa_renderer.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <gltf/gltf.h>


/* ---------------------------------------------------------- [ GLTF Ext ] -- */


int
gltf_to_stride(int component_type)
{
        switch (component_type) {
        case(GLTF_COMPONENT_TYPE_BYTE):           return sizeof(char);
        case(GLTF_COMPONENT_TYPE_UNSIGNED_BYTE):  return sizeof(unsigned char);
        case(GLTF_COMPONENT_TYPE_SHORT):          return sizeof(short);
        case(GLTF_COMPONENT_TYPE_UNSIGNED_SHORT): return sizeof(unsigned short);
        case(GLTF_COMPONENT_TYPE_UNSIGNED_INT):   return sizeof(unsigned int);
        case(GLTF_COMPONENT_TYPE_FLOAT):          return sizeof(float);
        }

        ROA_ASSERT(0); /* missing a type? */
        return 0;
}


/* ----------------------------------------------------------- [ Helpers ] -- */


struct node_scan {
        int is_decal;
};


/*
        Scans the gltf nodes and returns how many aren't decals.
        Nodes must be the correct size.
*/
int
scan_nodes(
        const struct gltf_import *gltf,
        struct node_scan *nodes)
{
        ROA_ASSERT(gltf);
        ROA_ASSERT(nodes);

        int i;
        int node_count = gltf->node_count;
        int nodes_to_create = 0;

        /* Find decals and mark */
        for (i = 0; i < node_count; ++i)
        {
          if (strcmp(gltf->nodes[i].name, "Decals") == 0) {
            nodes[i].is_decal = 1;

            int j;
            int child_count = gltf->nodes[i].child_count;

            for (j = 0; j < child_count; ++j) {
              int node_id = gltf->nodes[i].children[j];
              nodes[node_id].is_decal = 1;
            }
          }
        }

        /* count */
        for (i = 0; i < node_count; ++i) {
                if (!nodes[i].is_decal) {
                        nodes_to_create += 1;
                }
        }

        return nodes_to_create;
}


/*
        Pulls the mesh details for non-decals
        meshes must be the correct size.

        returns number of meshes to generate.
*/
int
generate_meshes(
        struct gltf_import *gltf,
        struct roa_renderer_mesh_resource *meshes,
        struct node_scan *nodes)
{
        int i;
        int node_count = gltf->node_count;
        int meshes_to_generate = 0;

        for (i = 0; i < node_count; ++i) {

                /* skip decals*/
                if (nodes[i].is_decal) {
                        continue;
                }

                int mesh_index = gltf->nodes[i].mesh;

                const char *name = gltf->meshes[mesh_index].name;
                meshes[meshes_to_generate].name = name;

                /* vertex data */
                int pos = gltf->meshes[mesh_index].primitives[0].attributes.POSITION;

                if (pos < 0) {  
                        /* a mesh without positional data? */
                        ROA_ASSERT(0);
                        continue;
                }

                int pos_view = gltf->accessors[pos].buffer_view;
                int pos_buffer = gltf->buffer_views[pos_view].buffer;
                int pos_offset = gltf->buffer_views[pos_view].byte_offset;

                meshes[meshes_to_generate].position_vec3_array = (float*)&gltf->buffers[pos_buffer].uri_data[pos_offset];

                int nor = gltf->meshes[mesh_index].primitives[0].attributes.NORMAL;

                if (nor < 0) {
                        /* is this a decal not in the correct place? */
                        ROA_ASSERT(0);
                        continue;
                }

                int nor_view = gltf->accessors[nor].buffer_view;
                int nor_buffer = gltf->buffer_views[nor_view].buffer;
                int nor_offset = gltf->buffer_views[nor_view].byte_offset;

                meshes[meshes_to_generate].normal_vec3_array = (float*)&gltf->buffers[nor_buffer].uri_data[nor_offset];

                int texc = gltf->meshes[mesh_index].primitives[0].attributes.TEXCOORD_0;

                if (texc < 0) {
                        /* is this a decal not in the correct place? */
                        ROA_ASSERT(0);
                        continue;
                }

                int texc_view = gltf->accessors[texc].buffer_view;
                int texc_buffer = gltf->buffer_views[texc_view].buffer;
                int texc_offset = gltf->buffer_views[texc_view].byte_offset;

                const unsigned char *buffer = gltf->buffers[texc_buffer].uri_data;
                const unsigned char *texc_rdata = &buffer[texc_offset];
                const float *texc_fdata = (float*)texc_rdata;
                
                meshes[meshes_to_generate].texture_coord_vec2_array = texc_fdata;
                meshes[meshes_to_generate].vertex_count = gltf->accessors[pos].count;

                /* index data */
                int index = gltf->meshes[mesh_index].primitives[0].indices;
                int index_view = gltf->accessors[index].buffer_view;
                int index_buffer = gltf->buffer_views[index_view].buffer;
                int index_offset = gltf->buffer_views[index_view].byte_offset;

                meshes[meshes_to_generate].index_stride = gltf_to_stride(gltf->accessors[index_view].component_type);
                meshes[meshes_to_generate].index_array = (void*)&gltf->buffers[index_buffer].uri_data[index_offset];
                meshes[meshes_to_generate].index_count = gltf->accessors[index_view].count;

                struct roa_renderer_decal *decals = 0;
                roa_array_create_with_capacity(decals, 32);

                int decal_count = 0;


                int j = 0;
                for (j = 0; j < gltf->nodes[i].child_count; ++j)
                {
                  if (strcmp(gltf->nodes[gltf->nodes[i].children[j]].name, "Decals") == 0) {

                    int m = 0;
                    int decal_count = gltf->nodes[gltf->nodes[i].children[j]].child_count;

                    for (m = 0; m < decal_count; ++m)
                    {
                      int n = gltf->nodes[gltf->nodes[i].children[j]].children[m];

                      struct roa_renderer_decal decal;

                      memcpy(decal.position, gltf->nodes[n].translation, sizeof(decals->position));
                      memcpy(decal.rotation, gltf->nodes[n].rotation, sizeof(decals->rotation));
                      memcpy(decal.scale, gltf->nodes[n].scale, sizeof(decals->scale));

                      /* projector is unit cube, but blenders default a 2x2x2 cube */
                      decal.scale[0] *= 2.f;
                      decal.scale[1] *= 2.f;
                      decal.scale[2] *= 2.f;

                      roa_array_push(decals, decal);
                    }

                    break;
                  }
                }

                meshes[meshes_to_generate].decals_lod0 = decals;
                meshes[].decals_lod0_count = roa_array_size(decals);

                meshes_to_generate += 1;
        }

        return meshes_to_generate;
}


void
load(
        struct roa_renderer_ctx *ctx,
        struct gltf_import *gltf)
{
        /* param check */
        ROA_ASSERT(ctx);
        ROA_ASSERT(gltf);

        int node_count = gltf->node_count;
        struct node_scan *nodes = ROA_NULL;

        /* process the nodes first */
        int nodes_to_create_count = 0;
        {
                roa_array_create_with_capacity(nodes, node_count);
                roa_array_resize(nodes, node_count);

                memset(nodes, 0, sizeof(nodes[0]) * node_count);

                nodes_to_create_count = scan_nodes(gltf, nodes);
        }

        /* create meshes */
        struct roa_renderer_mesh_resource *meshes = ROA_NULL;
        int mesh_count = 0;
        {
                roa_array_create_with_capacity(meshes, nodes_to_create_count);
                roa_array_resize(meshes, nodes_to_create_count);

                memset(meshes, 0, sizeof(meshes[0]) * nodes_to_create_count);

                mesh_count = generate_meshes(gltf, meshes, nodes);

                ROA_ASSERT(mesh_count == nodes_to_create_count);
        }

        /* add meshes */
        int m;

        for(m = 0; m < mesh_count; ++m)
        {
          roa_renderer_mesh_resource_add(ctx, &meshes[m]);
        }
}


/* ----------------------------------------------------------- [ Loading ] -- */


ROA_BOOL
roa_renderer_load(
        roa_renderer_ctx_t ctx,
        const char *file)
{
        /* param check */
        ROA_ASSERT(ctx);
        ROA_ASSERT(file);

        /* load file */
        struct gltf_import gltf;
        ROA_MEM_ZERO(gltf);

        gltf_import(file, &gltf);
        ROA_ASSERT(gltf.mesh_count);
       
        load(ctx, &gltf);
}
