#include <roa_renderer/roa_renderer.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <gltf/gltf.h>
#include <string.h>


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


/*
        Scans the gltf nodes and returns how many aren't decals.
        is_node_a_decal size must be equal to node_count.
*/
int
scan_nodes(
        const struct gltf_import *gltf,
        int *is_node_a_decal)
{
        /* param check */
        ROA_ASSERT(gltf);
        ROA_ASSERT(is_node_a_decal);

        /* process */
        int i;
        int node_count = gltf->node_count;

        /* Find decals and mark */
        for (i = 0; i < node_count; ++i) {
          if (strcmp(gltf->nodes[i].name, "Decals") == 0) {
            is_node_a_decal[i] = 1;

            int j;
            int child_count = gltf->nodes[i].child_count;

            for (j = 0; j < child_count; ++j) {
              int node_id = gltf->nodes[i].children[j];

              ROA_ASSERT(node_id < node_count);
              is_node_a_decal[node_id] = 1;
            }
          }
        }

        /* count */
        int nodes_to_create = 0;

        for (i = 0; i < node_count; ++i) {
                if (!is_node_a_decal[i]) {
                        nodes_to_create += 1;
                }
        }

        return nodes_to_create;
}


/* 
        Gets the vertex data, all types in the format must be found.
        Returns the vertex count.
*/
int
get_vertex_data(
        const struct gltf_import *gltf,
        int *vertex_desc,
        float **vertex_data,
        int vertex_count)
{
        /* param check */
        ROA_ASSERT(gltf);
        ROA_ASSERT(vertex_desc);
        ROA_ASSERT(vertex_data);
        ROA_ASSERT(vertex_count);

        /* process */
        int i;

        for (i = 0; i < vertex_count; ++i) {
                /* missing attribute - is this a decal */
                ROA_ASSERT(vertex_desc[i] >= 0);

                int item = vertex_desc[i];
                int view = gltf->accessors[item].buffer_view;
                ROA_ASSERT(view < gltf->buffer_view_count);

                int buf = gltf->buffer_views[view].buffer;
                ROA_ASSERT(buf < gltf->buffer_count);

                int off = gltf->buffer_views[view].byte_offset;
                float *data = (float*)&gltf->buffers[buf].uri_data[off];
                ROA_ASSERT(data);

                vertex_data[i] = data;
        }

        return gltf->accessors[vertex_desc[0]].count;
}


/* 
        Gets the index data, returns the number of indices.
*/
int
get_index_data(
        const struct gltf_import *gltf,
        int index,
        void **index_data,
        int *index_stride)
{
        /* param check */
        ROA_ASSERT(gltf);
        ROA_ASSERT(index);
        ROA_ASSERT(index_data);
        ROA_ASSERT(index_stride);

        /* process */
        int view = gltf->accessors[index].buffer_view;
        ROA_ASSERT(view < gltf->buffer_view_count);

        int buf = gltf->buffer_views[view].buffer;
        ROA_ASSERT(buf < gltf->buffer_view_count);

        int off = gltf->buffer_views[view].byte_offset;
        ROA_ASSERT(off >= 0);

        int type = gltf->accessors[view].component_type;
        *index_stride = gltf_to_stride(type);
        ROA_ASSERT(*index_stride > 0);

        *index_data = (void*)&gltf->buffers[buf].uri_data[off];

        int count = gltf->accessors[view].count;
        ROA_ASSERT(count > 0);

        return count;
}


int
parse_decals(
        const struct gltf_import *gltf,
        int node_id,
        struct roa_renderer_mesh_resource *out_mesh)
{
        int i, j;

        /* find decal child */
        int decal_child = -1;
        int decal_count = 0;

        int child_count = gltf->nodes[node_id].child_count;

        for(i = 0; i < child_count; ++i) {
                int child_id = gltf->nodes[node_id].children[i];
                const char *child_name = gltf->nodes[child_id].name;

                if(strcmp(child_name, "Decals") == 0) {
                      decal_child = i;
                      decal_count = gltf->nodes[child_id].child_count;
                      break;
                }
        }

        if(decal_child < 0) {
                return 0;
        }

        /* lod counts */
        const char *lod_names[] = {
                "lod0", "lod1", "lod2", "lod3", "lod4",
                "lod5", "lod6", "lod7", "lod8", "lod9",
        };

        int lod_count = ROA_ARR_COUNT(lod_names);

        int lod_counts[ROA_ARR_COUNT(lod_names)];
        memset(lod_counts, 0, sizeof(lod_counts));
        
        for(i = 0; i < decal_count; ++i) {
                int decal_id = gltf->nodes[decal_child].children[i];
                const char *decal_name = gltf->nodes[decal_id].name;

                for(j = 0; j < lod_count; ++j) {
                
                        const char *lod_name = lod_names[j];
                        int len = strlen(lod_name);
                        
                        if(strncmp(lod_name, decal_name, len) == 0) {
                                lod_counts[j] += 1;
                        }
                }
        }

        /* allocate */
        struct roa_renderer_decal *decals[ROA_ARR_COUNT(lod_counts)];
        memset(decals, 0, sizeof(decals[0]) * ROA_ARR_COUNT(lod_counts));

        for(i = 0; i < lod_count; ++i) {
                roa_array_create_with_capacity(decals[i], lod_counts[i]);
        }

        /* get decal information */
        for(i = 0; i < decal_count; ++i) {

                int decal_id = gltf->nodes[decal_child].children[i];
                const char *decal_name = gltf->nodes[decal_id].name;

                for(j = 0; j < lod_count; ++j) {
                        const char *lod_name = lod_names[j];
                        int len = strlen(lod_name);

                        if(strncmp(lod_name, decal_name, len) == 0) {
                                struct roa_renderer_decal decal;

                                memcpy(
                                        decal.position,
                                        gltf->nodes[decal_id].translation,
                                        sizeof(decal.position));

                                memcpy(
                                        decal.rotation,
                                        gltf->nodes[decal_id].rotation,
                                        sizeof(decal.rotation));

                                memcpy(
                                        decal.scale,
                                        gltf->nodes[decal_id].scale,
                                        sizeof(decal.scale));

                                int mesh_id = gltf->nodes[decal_id].mesh;
                                if (mesh_id >= 0) {
                                        int mat_id = gltf->meshes[mesh_id].primitives[0].material;
                                        
                                        decal.color[0] = gltf->materials[mat_id].pbr_metallic_roughness.base_color_factor[0];
                                        decal.color[1] = gltf->materials[mat_id].pbr_metallic_roughness.base_color_factor[1];
                                        decal.color[2] = gltf->materials[mat_id].pbr_metallic_roughness.base_color_factor[2];
                                } 

                                /* projector is a unit cube,
                                   but blenders default a 2x2x2 cube */
                                decal.scale[0] *= 2.f;
                                decal.scale[1] *= 2.f;
                                decal.scale[2] *= 2.f;

                               /* decal.color[0] = 1.f; */

                                roa_array_push(decals[j], decal);
                        }
                }

        }

        /* output lod levels */
        out_mesh->decals_lod0 = decals[0];
        out_mesh->decals_lod0_count = roa_array_size(decals[0]);

        out_mesh->decals_lod1 = decals[1];
        out_mesh->decals_lod1_count = roa_array_size(decals[1]);

        out_mesh->decals_lod2 = decals[2];
        out_mesh->decals_lod2_count = roa_array_size(decals[2]);

        out_mesh->decals_lod3 = decals[3];
        out_mesh->decals_lod3_count = roa_array_size(decals[3]);

        out_mesh->decals_lod4 = decals[4];
        out_mesh->decals_lod4_count = roa_array_size(decals[4]);

        out_mesh->decals_lod5 = decals[5];
        out_mesh->decals_lod5_count = roa_array_size(decals[5]);

        out_mesh->decals_lod6 = decals[6];
        out_mesh->decals_lod6_count = roa_array_size(decals[6]);

        out_mesh->decals_lod7 = decals[7];
        out_mesh->decals_lod7_count = roa_array_size(decals[7]);

        out_mesh->decals_lod8 = decals[8];
        out_mesh->decals_lod8_count = roa_array_size(decals[8]);

        out_mesh->decals_lod9 = decals[9];
        out_mesh->decals_lod9_count = roa_array_size(decals[9]);

        return 1;
}


/*
        Pulls the mesh details for non-decals
        meshes must be the correct size.

        returns number of meshes to generate.
*/
int
generate_meshes(
        const struct gltf_import *gltf,
        struct roa_renderer_mesh_resource *meshes,
        int *is_node_a_decal)
{
        int i;
        int node_count = gltf->node_count;
        int meshes_to_generate = 0;

        for (i = 0; i < node_count; ++i) {

                /* skip decals*/
                if (is_node_a_decal[i]) {
                        continue;
                }

                int mesh_index = gltf->nodes[i].mesh;

                const struct gltf_mesh *curr_mesh =
                        &gltf->meshes[mesh_index];

                struct roa_renderer_mesh_resource *out_mesh =
                        &meshes[meshes_to_generate];

                /* name */
                const char *name = gltf->meshes[mesh_index].name;
                out_mesh->name = name;

                /* vertex data */
                int vertex_desc[3];
                ROA_MEM_ZERO(vertex_desc);
                vertex_desc[0] = curr_mesh->primitives[0].attributes.POSITION;
                vertex_desc[1] = curr_mesh->primitives[0].attributes.NORMAL;
                vertex_desc[2] = curr_mesh->primitives[0].attributes.TEXCOORD_0;

                float *vertex_data[ROA_ARR_COUNT(vertex_desc)];
                ROA_MEM_ZERO(vertex_data);

                out_mesh->vertex_count = get_vertex_data(
                        gltf,
                        ROA_ARR_DATA(vertex_desc),
                        ROA_ARR_DATA(vertex_data),
                        ROA_ARR_COUNT(vertex_desc));

                /* be sure order matches desc */
                out_mesh->position_vec3_array = vertex_data[0];
                out_mesh->normal_vec3_array = vertex_data[1];
                out_mesh->texture_coord_vec2_array = vertex_data[2];
                
                /* index data */
                out_mesh->index_count = get_index_data(
                          gltf,
                          gltf->meshes[mesh_index].primitives[0].indices,
                          &out_mesh->index_array,
                          &out_mesh->index_stride);

                /* decals */
                parse_decals(gltf, i, out_mesh);

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
        int *is_node_a_decal = ROA_NULL;

        /* process the nodes first */
        int nodes_to_create_count = 0;
        {
                roa_array_create_with_capacity(is_node_a_decal, node_count);
                roa_array_resize(is_node_a_decal, node_count);

                int size = sizeof(is_node_a_decal[0]) * node_count;
                memset(is_node_a_decal, 0, size);

                nodes_to_create_count = scan_nodes(gltf, is_node_a_decal);
                ROA_ASSERT(nodes_to_create_count > 0);
        }

        /* create meshes */
        struct roa_renderer_mesh_resource *meshes = ROA_NULL;
        int mesh_count = 0;
        {
                roa_array_create_with_capacity(meshes, nodes_to_create_count);
                roa_array_resize(meshes, nodes_to_create_count);

                memset(meshes, 0, sizeof(meshes[0]) * nodes_to_create_count);

                mesh_count = generate_meshes(gltf, meshes, is_node_a_decal);

                ROA_ASSERT(mesh_count == nodes_to_create_count);
        }

        /* add meshes */
        int m;

        for(m = 0; m < mesh_count; ++m) {
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

        return ROA_TRUE;
}
