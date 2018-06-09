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


void
load_mesh(
        struct roa_renderer_ctx *ctx,
        struct gltf_import *gltf,
        int mesh_index)
{
        ROA_ASSERT(gltf->mesh_count > mesh_index);

        struct roa_renderer_mesh_resource mesh_rsrc;
        ROA_MEM_ZERO(mesh_rsrc);

        mesh_rsrc.name = gltf->meshes[mesh_index].name;

        /* vertex data */
        int pos = gltf->meshes[mesh_index].primitives[0].attributes.POSITION;

        /* we need position data - bail if none */
        if (pos < 0) {
                return;
        }

        int pos_view = gltf->accessors[pos].buffer_view;
        int pos_buffer = gltf->buffer_views[pos_view].buffer;
        int pos_offset = gltf->buffer_views[pos_view].byte_offset;

        mesh_rsrc.position_vec3_array = (float*)&gltf->buffers[pos_buffer].uri_data[pos_offset];

        int nor = gltf->meshes[mesh_index].primitives[0].attributes.NORMAL;
        int nor_view = gltf->accessors[nor].buffer_view;
        int nor_buffer = gltf->buffer_views[nor_view].buffer;
        int nor_offset = gltf->buffer_views[nor_view].byte_offset;

        mesh_rsrc.normal_vec3_array = (float*)&gltf->buffers[nor_buffer].uri_data[nor_offset];

        int texc = gltf->meshes[mesh_index].primitives[0].attributes.TEXCOORD_0;
        int texc_view = gltf->accessors[texc].buffer_view;
        int texc_buffer = gltf->buffer_views[texc_view].buffer;
        int texc_offset = gltf->buffer_views[texc_view].byte_offset;

        const unsigned char *buffer = gltf->buffers[texc_buffer].uri_data;
        const unsigned char *texc_rdata = &buffer[texc_offset];
        const float *texc_fdata = (float*)texc_rdata;

        float a = texc_fdata[0];

        mesh_rsrc.texture_coord_vec2_array = texc_fdata;
        mesh_rsrc.vertex_count = gltf->accessors[pos].count;

        /* index data */
        int index = gltf->meshes[mesh_index].primitives[0].indices;
        int index_view = gltf->accessors[index].buffer_view;
        int index_buffer = gltf->buffer_views[index_view].buffer;
        int index_offset = gltf->buffer_views[index_view].byte_offset;

        mesh_rsrc.index_stride = gltf_to_stride(gltf->accessors[index_view].component_type);
        mesh_rsrc.index_array = (void*)&gltf->buffers[index_buffer].uri_data[index_offset];
        mesh_rsrc.index_count = gltf->accessors[index_view].count;

        /* add mesh */
        roa_renderer_mesh_resource_add(ctx, &mesh_rsrc);
}


void
load(
        struct roa_renderer_ctx *ctx,
        struct gltf_import *gltf)
{
        /* param check */
        ROA_ASSERT(ctx);
        ROA_ASSERT(gltf);

        /* process the nodes first */
        struct node {
              int is_decal;
        };

        int node_count = gltf->node_count;

        struct node *nodes = ROA_NULL;
        roa_array_create_with_capacity(nodes, node_count);
        roa_array_resize(nodes, node_count);

        memset(nodes, 0, sizeof(nodes[0]) * node_count);

        int nodes_to_create_count = 0;

        int i;
        for (i = 0; i < node_count; ++i)
        {
                /* Mark as decal? */
                if (strcmp(gltf->nodes[i].name, "Decals") == 0) {
                        nodes[i].is_decal = 1;

                        int j;
                        int child_count = gltf->nodes[i].child_count;

                        for (j = 0; j < child_count; ++j) {
                                int node_id = gltf->nodes[i].children[j];
                                nodes[node_id].is_decal = 1;
                        }
                }

                if (nodes[i].is_decal == 0) {
                        nodes_to_create_count += 1;
                }
        }

        /* create meshes */
        struct roa_renderer_mesh_resource *meshes = ROA_NULL;
        roa_array_create_with_capacity(meshes, nodes_to_create_count);

        for (i = 0; i < node_count; ++i) {

                /* skip decals*/
                if (nodes[i].is_decal) {
                        continue;
                }

                struct roa_renderer_mesh_resource mesh_rsrc;
                ROA_MEM_ZERO(mesh_rsrc);

                int mesh_index = gltf->nodes[i].mesh;

                mesh_rsrc.name = gltf->meshes[mesh_index].name;

                /* vertex data */
                int pos = gltf->meshes[mesh_index].primitives[0].attributes.POSITION;

                /* we need position data - bail if none */
                if (pos < 0) {
                        return;
                }

                int pos_view = gltf->accessors[pos].buffer_view;
                int pos_buffer = gltf->buffer_views[pos_view].buffer;
                int pos_offset = gltf->buffer_views[pos_view].byte_offset;

                mesh_rsrc.position_vec3_array = (float*)&gltf->buffers[pos_buffer].uri_data[pos_offset];

                int nor = gltf->meshes[mesh_index].primitives[0].attributes.NORMAL;
                int nor_view = gltf->accessors[nor].buffer_view;
                int nor_buffer = gltf->buffer_views[nor_view].buffer;
                int nor_offset = gltf->buffer_views[nor_view].byte_offset;

                mesh_rsrc.normal_vec3_array = (float*)&gltf->buffers[nor_buffer].uri_data[nor_offset];

                int texc = gltf->meshes[mesh_index].primitives[0].attributes.TEXCOORD_0;
                int texc_view = gltf->accessors[texc].buffer_view;
                int texc_buffer = gltf->buffer_views[texc_view].buffer;
                int texc_offset = gltf->buffer_views[texc_view].byte_offset;

                const unsigned char *buffer = gltf->buffers[texc_buffer].uri_data;
                const unsigned char *texc_rdata = &buffer[texc_offset];
                const float *texc_fdata = (float*)texc_rdata;

                float a = texc_fdata[0];

                mesh_rsrc.texture_coord_vec2_array = texc_fdata;
                mesh_rsrc.vertex_count = gltf->accessors[pos].count;

                /* index data */
                int index = gltf->meshes[mesh_index].primitives[0].indices;
                int index_view = gltf->accessors[index].buffer_view;
                int index_buffer = gltf->buffer_views[index_view].buffer;
                int index_offset = gltf->buffer_views[index_view].byte_offset;

                mesh_rsrc.index_stride = gltf_to_stride(gltf->accessors[index_view].component_type);
                mesh_rsrc.index_array = (void*)&gltf->buffers[index_buffer].uri_data[index_offset];
                mesh_rsrc.index_count = gltf->accessors[index_view].count;

                struct roa_renderer_decal *decals = 0;
                roa_array_create_with_capacity(decals, 32);

                int decal_count = 0;

                int j = 0;
                for(j = 0; j < gltf->nodes[i].child_count; ++j)
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

                mesh_rsrc.decals_lod0 = decals;
                mesh_rsrc.decals_lod0_count = roa_array_size(decals);

                /* push new mesh */
                roa_array_push(meshes, mesh_rsrc);
        }

        /* add mesh */
        int mesh_count = roa_array_size(meshes);
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
        
        int i;
        int m_count = gltf.mesh_count;

        for (i = 0; i < m_count; ++i) {
                //load_mesh(ctx, &gltf, i);
        }

        load(ctx, &gltf);

        /* process */

        int node_count = gltf.node_count;
        //int i;

        for (i = 0; i < node_count; ++i)
        {
                
        }
}
