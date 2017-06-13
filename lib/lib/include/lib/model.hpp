#ifndef MODEL_INCLUDED_96BD3307_4311_4351_9AC5_524CB08EC60E
#define MODEL_INCLUDED_96BD3307_4311_4351_9AC5_524CB08EC60E


#include <stddef.h>
#include <stdint.h>


// --------------------------------------------------------- [ Model Config ] --


#define MODEL_ALLOC(bytes) malloc(bytes)
#define MODEL_REALLOC(ptr, bytes) realloc(ptr, bytes)
#define MODEL_FREE(ptr) free(ptr)


#ifndef LIB_NS_NAME
#define LIB_NS_NAME lib
#endif


// ------------------------------------------------------ [ Model Interface ] --


namespace LIB_NS_NAME {


struct model
{
  float     **verts;
  float     **normals;
  float     **uvs;
  uint32_t   *element_count;
  uint32_t    mesh_count;
};


namespace model_import {


model
load_obj_from_file(
  const char *filename,
  const size_t size_hint = 512);


} // ns
} // ns


#endif // inc guard


#ifdef LIB_MODEL_IMPL

#ifndef MODEL_IMPL_INCLUDED_1F2E5258_471F_44A5_A74C_5738F155EA61
#define MODEL_IMPL_INCLUDED_1F2E5258_471F_44A5_A74C_5738F155EA61


#include <stdlib.h>
#include <stdio.h>
#include "logging.hpp"


// ----------------------------------------------------------- [ Model Impl ] --


namespace LIB_NS_NAME {
namespace model_import {


model
load_obj_from_file(
  const char *filename,
  const size_t size_hint)
{
  // Load file
  FILE *file = fopen(filename, "r");
  
  float *pos_list = (float*)MODEL_ALLOC(sizeof(float) * size_hint);
  size_t pos_count = 0;
  
  float *normal_list = (float*)MODEL_ALLOC(sizeof(float) * size_hint);
  size_t normal_count = 0;
  
  float *uv_list = (float*)MODEL_ALLOC(sizeof(float) * size_hint);
  size_t uv_count = 0;
  
  size_t capacity     = size_hint;
  size_t mesh_count   = 0;
  
  size_t vertex_capcity = 0;
  size_t vertex_count = 0;
  
  LIB_NS_NAME::model return_model{};
  
  if(file)
  {
    while(true)
    {
      char line[128]{};
      
      const int result = fscanf(file, "%s", line);
      
      if(result == EOF)
      {
        break;
      }
      
      /*
        Object Name - This is the mesh name
        This needs to come before all the vertex information.
      */
      if(strcmp("o", line) == 0)
      {
        // This might be a new mesh so zero the counters.
        vertex_count = 0;
        normal_count = 0;
        uv_count     = 0;
        
        vertex_count = 0;
        vertex_capcity = 0;
        
        mesh_count += 1;
      }
      
      /*
        Vertex Position
      */
      else if(strcmp("v", line) == 0)
      {
        fscanf(
          file,
          "%f %f %f\n",
          &pos_list[pos_count + 0],
          &pos_list[pos_count + 1],
          &pos_list[pos_count + 2]
        );
        
        vertex_count += 3;
      }
      
      /*
        Texture Position
      */
      else if(strcmp("vt", line) == 0)
      {
        fscanf(
          file,
          "%f %f %f\n",
          &uv_list[uv_count + 0],
          &uv_list[uv_count + 1],
          &uv_list[uv_count + 2]
        );
        
        uv_count += 3;
      }
      
      /*
        Normal
      */
      else if(strcmp("vn", line) == 0)
      {
        fscanf(
          file,
          "%f %f %f\n",
          &normal_list[normal_count + 0],
          &normal_list[normal_count + 1],
          &normal_list[normal_count + 2]
        );
        
        normal_count += 3;
      }
      
      /*
        Face - This is the index that makes up the triangle
        This section in the object file needs to come *after* all the meshes
        vertices etc have been parsed.
      */
      else if(strcmp("f", line) == 0)
      {
        /*
          If this is a new mesh create space for it.
        */
        if(return_model.mesh_count < mesh_count)
        {
          const size_t this_mesh = mesh_count - 1;
        
          // Resize the
          {
            float **vert_ptrs     = (float**)MODEL_ALLOC(sizeof(float**) * mesh_count);
            float **old_vert_ptrs = return_model.verts;
            return_model.verts    = vert_ptrs;
            memcpy(return_model.verts, old_vert_ptrs, sizeof(float**) * this_mesh);
            MODEL_FREE(old_vert_ptrs);
          }
          
          {
            float **norm_ptrs     = (float**)MODEL_ALLOC(sizeof(float**) * mesh_count);
            float **old_norm_ptrs = return_model.normals;
            return_model.normals  = norm_ptrs;
            memcpy(return_model.normals, old_norm_ptrs, sizeof(float**) * this_mesh);
            MODEL_FREE(old_norm_ptrs);
          }
          
          {
            float **uv_ptrs     = (float**)MODEL_ALLOC(sizeof(float**) * mesh_count);
            float **old_uv_ptrs = return_model.uvs;
            return_model.uvs    = uv_ptrs;
            memcpy(return_model.uvs, old_uv_ptrs, sizeof(float**) * this_mesh);
            MODEL_FREE(old_uv_ptrs);
          }
          
          {
            uint32_t *element_count    = (uint32_t*)MODEL_ALLOC(sizeof(uint32_t*) * mesh_count);
            uint32_t *old_elements     = return_model.element_count;
            return_model.element_count = element_count;
            memcpy(return_model.uvs, old_elements, sizeof(uint32_t*) * this_mesh);
            MODEL_FREE(old_elements);
          }
          
          // Allocate hint size space.
          {
            return_model.verts[this_mesh]   = (float*)MODEL_ALLOC(sizeof(float) * 3 * size_hint);
            return_model.normals[this_mesh] = (float*)MODEL_ALLOC(sizeof(float) * 3 * size_hint);
            return_model.uvs[this_mesh]     = (float*)MODEL_ALLOC(sizeof(float) * 2 * size_hint);
            
            vertex_capcity = size_hint;
          }
          
          return_model.mesh_count += 1;
        }
      
        uint32_t index_list[9];
      
        const int matched = fscanf(
          file,
          "%d/%d/%d %d/%d/%d %d/%d/%d\n",
          &index_list[0],
          &index_list[1],
          &index_list[2],
          &index_list[3],
          &index_list[4],
          &index_list[5],
          &index_list[6],
          &index_list[7],
          &index_list[8]
        );
        
        if(matched != 9)
        {
          LOG_ERROR("Importer error, need positions, normals, and tex coords defined, and triangulated");
          break;
        }
        
        const uint32_t curr_mesh = return_model.mesh_count - 1;
        
        // Vert
        
        return_model.verts[curr_mesh][vertex_count * 3 + 0] = pos_list[index_list[0] * 3 + 0];
        return_model.verts[curr_mesh][vertex_count * 3 + 1] = pos_list[index_list[0] * 3 + 1];
        return_model.verts[curr_mesh][vertex_count * 3 + 2] = pos_list[index_list[0] * 3 + 2];

        return_model.normals[curr_mesh][vertex_count * 3 + 0] = normal_list[index_list[1] * 3 + 0];
        return_model.normals[curr_mesh][vertex_count * 3 + 1] = normal_list[index_list[1] * 3 + 1];
        return_model.normals[curr_mesh][vertex_count * 3 + 2] = normal_list[index_list[1] * 3 + 2];
        
        return_model.uvs[curr_mesh][vertex_count * 2 + 0] = uv_list[index_list[2] * 2 + 0];
        return_model.uvs[curr_mesh][vertex_count * 2 + 1] = uv_list[index_list[2] * 2 + 1];
        
        // Vert
        
        return_model.verts[curr_mesh][vertex_count * 3 + 0] = pos_list[index_list[3] * 3 + 0];
        return_model.verts[curr_mesh][vertex_count * 3 + 1] = pos_list[index_list[3] * 3 + 1];
        return_model.verts[curr_mesh][vertex_count * 3 + 2] = pos_list[index_list[3] * 3 + 2];

        return_model.normals[curr_mesh][vertex_count * 3 + 0] = normal_list[index_list[4] * 3 + 0];
        return_model.normals[curr_mesh][vertex_count * 3 + 1] = normal_list[index_list[4] * 3 + 1];
        return_model.normals[curr_mesh][vertex_count * 3 + 2] = normal_list[index_list[4] * 3 + 2];
        
        return_model.uvs[curr_mesh][vertex_count * 2 + 0] = uv_list[index_list[5] * 2 + 0];
        return_model.uvs[curr_mesh][vertex_count * 2 + 1] = uv_list[index_list[5] * 2 + 1];
        
        // Vert
        
        return_model.verts[curr_mesh][vertex_count * 3 + 0] = pos_list[index_list[6] * 3 + 0];
        return_model.verts[curr_mesh][vertex_count * 3 + 1] = pos_list[index_list[6] * 3 + 1];
        return_model.verts[curr_mesh][vertex_count * 3 + 2] = pos_list[index_list[6] * 3 + 2];

        return_model.normals[curr_mesh][vertex_count * 3 + 0] = normal_list[index_list[7] * 3 + 0];
        return_model.normals[curr_mesh][vertex_count * 3 + 1] = normal_list[index_list[7] * 3 + 1];
        return_model.normals[curr_mesh][vertex_count * 3 + 2] = normal_list[index_list[7] * 3 + 2];
        
        return_model.uvs[curr_mesh][vertex_count * 2 + 0] = uv_list[index_list[8] * 2 + 0];
        return_model.uvs[curr_mesh][vertex_count * 2 + 1] = uv_list[index_list[8] * 2 + 1];
      }
    }
  }
  else
  {
    LOG_ERROR("Failed to open file")
  }
  
  return return_model;
}


} // ns
} // ns


#endif // impl guard
#endif // impl inc
