/*
  Model
  --
  Mesh / Material Loading

  Copyright: public-domain 2017 - http://unlicense.org/
*/
#ifndef MODEL_INCLUDED_96BD3307_4311_4351_9AC5_524CB08EC60E
#define MODEL_INCLUDED_96BD3307_4311_4351_9AC5_524CB08EC60E


#include <stddef.h>
#include <stdint.h>


// --------------------------------------------------------- [ Model Config ] --


#define MODEL_ALLOC(type, bytes) (type*)malloc(bytes)
#define MODEL_FREE(ptr) free(ptr)


#ifndef LIB_NS_NAME
#define LIB_NS_NAME lib
#endif


// ------------------------------------------------------ [ Model Interface ] --


namespace LIB_NS_NAME {


struct material
{
  char *name;
  char *texture_01_path;
  char *texture_02_path;
  char *texture_03_path;
};


struct model
{
  char      **name;
  float     **verts;
  float     **normals;
  float     **uvs;
  uint32_t   *vertex_count;
  uint32_t   *triangle_count;
  int32_t    *material_id;
  uint32_t    mesh_count;
  
  material   *mesh_material;
  uint32_t    material_count;
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
#include "string.hpp"


// ----------------------------------------------------------- [ Model Impl ] --


namespace {


constexpr uint32_t lib_null = 1; // decorative text - including null terminator


} // anon ns


namespace LIB_NS_NAME {
namespace model_import {


model
load_obj_from_file(
  const char *filename,
  const size_t size_hint)
{
  LIB_NS_NAME::model model{};

  // Load file
  FILE *obj_file = fopen(filename, "r");
  FILE *mat_file = nullptr;
  
  /*
    Parase the Obj file, if we come accross a Mtl file we will parse it then.
  */
  if(obj_file)
  {
    /*
      Inital Obj run through.
      --
      Parse the file to find out how may meshes and vertexes there are.
      This ables us to reduce the number of allocations we have to make.
    */
    uint32_t mesh_count = 0;
    lib::array<uint32_t, 32> triangle_count;
    lib::array<uint32_t, 32> name_size;
    
    char line[512]{};
    
    while(true)
    {
      memset(line, 0, sizeof(line));
      
      const int result = fscanf(obj_file, "%s", line);
      
      if(result == EOF)
      {
        break;
      }
      
      // -- [ Material ] --
      
      else if(strcmp("mtllib", line) == 0)
      {
        char name[512]{};
        
        fscanf(
          obj_file,
          "%s\n",
          name
        );
      
        std::string path = LIB_NS_NAME::string::get_dir_from_filepath(filename) + std::string(name);
        mat_file = fopen(path.c_str(), "r");
        
        // -- [ Found a Material - Stop and do this ] --
        
        if(mat_file)
        {
          uint32_t mat_count = 0;
          lib::array<uint32_t, 32> mat_name_size;
          lib::array<uint32_t, 32> diffuse_map_name_size;
          
          // -- [ Scan through the material first ] --
          
          while(true)
          {
            char mat_line[512]{};
          
            const int result = fscanf(mat_file, "%s", mat_line);
            
            if(result == EOF)
            {
              break;
            }
            
            else if(strcmp("newmtl", mat_line) == 0)
            {
              mat_count += 1;
              
              char mat_name[512]{};
              
              fscanf(
                mat_file,
                "%s",
                mat_name
              );
              
              const uint32_t str_size = strlen(mat_name) + lib_null;
              mat_name_size.emplace_back(str_size);
              
              diffuse_map_name_size.emplace_back(uint32_t{0});
            }
            else if(strcmp("map_Kd", mat_line) == 0)
            {
              char path[1024]{};
              
              fscanf(
                mat_file,
                "%s\n",
                path
              );
              
              const uint32_t str_size = strlen(path) + lib_null;
              diffuse_map_name_size.back() = str_size;
            }
          }
          
          // -- [ Allocate Memory For Material ] --
          
          {
            const size_t mat_bytes = sizeof(material) * mat_count;
            model.mesh_material = MODEL_ALLOC(material, mat_bytes);
            memset(model.mesh_material, 0, sizeof(material));
            
            for(uint32_t i = 0; i < mat_count; ++i)
            {
              const size_t name_bytes = sizeof(char) * (mat_name_size[i]);
              model.mesh_material[i].name = MODEL_ALLOC(char, name_bytes);
              memset(model.mesh_material[i].name, 0, name_bytes);
              
              if(diffuse_map_name_size[i])
              {
                const size_t map_bytes = sizeof(char) * (diffuse_map_name_size[i]);
                model.mesh_material[i].texture_01_path = MODEL_ALLOC(char, map_bytes);
                memset(model.mesh_material[i].texture_01_path, 0, map_bytes);
              }
            }
            
            model.material_count = mat_count;
          }
          
          // -- [ Read the material in ] --
          
          fseek(mat_file, 0, SEEK_SET);
          int32_t this_mat = -1;
          
          while(true)
          {
            char mat_line[512]{};
            memset(mat_line, 0, sizeof(mat_line));
          
            const int result = fscanf(mat_file, "%s", mat_line);
            
            if(result == EOF)
            {
              break;
            }
            
            else if(strcmp("newmtl", mat_line) == 0)
            {
              this_mat += 1;
              char mat_name[512]{};
              
              fscanf(
                mat_file,
                "%s\n",
                mat_name
              );
              
              strcat(model.mesh_material[this_mat].name, mat_name);
            }
            
            /*
              Diffuse map
            */
            else if(strcmp("map_Kd", mat_line) == 0)
            {
              char path[1024]{};
              
              fscanf(
                mat_file,
                "%s\n",
                path
              );
              
              strcat(model.mesh_material[this_mat].texture_01_path, path);
            }
          }
        } // if mat_file
      }
      
      /*
        New Object
      */
      else if(strcmp("o", line) == 0)
      {
        char name[512]{};
      
        fscanf(
          obj_file,
          "%s\n",
          name
        );
        
        const uint32_t str_size = strlen(name);
        name_size.emplace_back(str_size);
      
        // This might be a new mesh so zero the counters.
        mesh_count += 1;
        triangle_count.emplace_back();
      }
      
      if(strcmp("f", line) == 0)
      {
        LIB_ASSERT(mesh_count > 0);
        triangle_count[mesh_count - 1] += 1;
      }
    }
    
    
    /*
      Setup the return model
    */
    {
      model.mesh_count     = mesh_count;
      
      model.name           = MODEL_ALLOC(char*, sizeof(char*) * mesh_count);
      model.verts          = MODEL_ALLOC(float*, sizeof(float*) * mesh_count);
      model.normals        = MODEL_ALLOC(float*, sizeof(float*) * mesh_count);
      model.uvs            = MODEL_ALLOC(float*, sizeof(float*) * mesh_count);
      model.triangle_count = MODEL_ALLOC(uint32_t, sizeof(uint32_t) * mesh_count);
      model.vertex_count   = MODEL_ALLOC(uint32_t, sizeof(uint32_t) * mesh_count);
      model.material_id    = MODEL_ALLOC(int32_t, sizeof(int32_t) * mesh_count);
      
      for(uint32_t i = 0; i < mesh_count; ++i)
      {
        const uint32_t name_bytes = sizeof(char) * name_size[i] + 1;
        model.name[i] = MODEL_ALLOC(char, name_bytes);
        memset(model.name[i], 0, name_bytes);
        
        const uint32_t vec3_bytes = sizeof(float) * triangle_count[i] * 9;
        model.verts[i] = MODEL_ALLOC(float, vec3_bytes);
        memset(model.verts[i], 0, vec3_bytes);
        
        model.normals[i] = MODEL_ALLOC(float, vec3_bytes);
        memset(model.normals[i], 0, vec3_bytes);
        
        const uint32_t vec2_bytes = sizeof(float) * triangle_count[i] * 6;
        model.uvs[i] = MODEL_ALLOC(float, sizeof(float) * triangle_count[i] * 6);
        memset(model.uvs[i], 0, vec2_bytes);
        
        model.triangle_count[i]  = triangle_count[i];
        model.vertex_count[i]    = triangle_count[i] * 3;
        model.material_id[i]     = -1;
      }
    }
    
    // Fill the mesh buffers
    {
      fseek(obj_file, 0, SEEK_SET);
    
      lib::array<float, 128 * 3> positions;
      positions.resize(3);
      
      lib::array<float, 128 * 3> normals;
      normals.resize(3);
      
      lib::array<float, 128 * 2> uvs;
      uvs.resize(2);
    
      uint32_t this_mesh = 0;
      uint32_t curr_tri = 0;
    
      while(true)
      {
        memset(line, 0, sizeof(line));
        
        const int result = fscanf(obj_file, "%s", line);
        
        /*
          All done.
        */
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
          char name[512]{};
      
          fscanf(
            obj_file,
            "%s\n",
            name
          );
          
          strcat(model.name[this_mesh], name);
        
          curr_tri = 0;
          this_mesh += 1;
        }
        
        /*
          Vertex Position
        */
        else if(strcmp("v", line) == 0)
        {
          const size_t curr_size = positions.size();
        
          positions.emplace_back(0.f);
          positions.emplace_back(0.f);
          positions.emplace_back(0.f);
        
          fscanf(
            obj_file,
            "%f %f %f\n",
            &positions[curr_size + 0],
            &positions[curr_size + 1],
            &positions[curr_size + 2]
          );
        }
        
        /*
          Texture Position
        */
        else if(strcmp("vt", line) == 0)
        {
          const size_t curr_size = uvs.size();
          
          uvs.emplace_back(0.f);
          uvs.emplace_back(0.f);
        
          fscanf(
            obj_file,
            "%f %f\n",
            &uvs[curr_size + 0],
            &uvs[curr_size + 1]
          );
        }
        
        /*
          Normal
        */
        else if(strcmp("vn", line) == 0)
        {
          const size_t curr_size = normals.size();
        
          normals.emplace_back(0.f);
          normals.emplace_back(0.f);
          normals.emplace_back(0.f);
        
          fscanf(
            obj_file,
            "%f %f %f\n",
            &normals[curr_size + 0],
            &normals[curr_size + 1],
            &normals[curr_size + 2]
          );
        }
        
        else if(strcmp("usemtl", line) == 0)
        {
          char mat_name[512];
        
          fscanf(
            obj_file,
            "%s\n",
            mat_name
          );
          
          // Search mats for this
          for(uint32_t i = 0; i < model.material_count; ++i)
          {
            if(strcmp(model.mesh_material[i].name, mat_name) == 0)
            {
              model.material_id[this_mesh - 1] = i;
              break;
            }
          }
        }
        
        /*
          Face - This is the index that makes up the triangle
          This section in the object file needs to come *after* all the meshes
          vertices etc have been parsed.
        */
        else if(strcmp("f", line) == 0)
        {
          uint32_t index_list[9]{};
        
          // Vertex / texture / normal
          
          const long int pos = ftell(obj_file);
        
          int matched = fscanf(
            obj_file,
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
            fseek(obj_file, pos, SEEK_SET);
            
            memset(index_list, 0, sizeof(index_list));
            
            matched = fscanf(
              obj_file,
              "%d//%d %d//%d %d//%d\n",
              &index_list[0],
              &index_list[2],
              
              &index_list[3],
              &index_list[5],
              
              &index_list[6],
              &index_list[8]
            );
            
            if(matched != 6)
            {
              fseek(obj_file, pos, SEEK_SET);
            
              memset(index_list, 0, sizeof(index_list));
            
              matched = fscanf(
                obj_file,
                "%d/%d %d/%d %d/%d\n",
                &index_list[0],
                &index_list[1],
                
                &index_list[3],
                &index_list[4],
                
                &index_list[6],
                &index_list[7]
              );
            }
            
            if(matched != 6)
            {
              fseek(obj_file, pos, SEEK_SET);
            
              memset(index_list, 0, sizeof(index_list));
            
              int matched = fscanf(
                obj_file,
                "%d %d %d\n",
                &index_list[0],
                
                &index_list[3],
                
                &index_list[6]
              );
              
              if(matched != 3)
              {
                LOG_ERROR("Failed to understand face, try exporting with different settings");
                break;
              }
            }
          }
          
          const size_t curr_mesh = this_mesh - 1;
          const size_t vec3_offset = curr_tri * 9;
          const size_t vec2_offset = curr_tri * 6;
          
          // Vertex
          {
            const size_t pos_offset = index_list[0] * 3;
            LIB_ASSERT(pos_offset < (size_t)model.vertex_count);
          
            model.verts[curr_mesh][vec3_offset + 0] = positions[pos_offset + 0];
            model.verts[curr_mesh][vec3_offset + 1] = positions[pos_offset + 1];
            model.verts[curr_mesh][vec3_offset + 2] = positions[pos_offset + 2];
            
            const size_t uv_offset = index_list[1] * 2;
            LIB_ASSERT(uv_offset < (size_t)model.vertex_count);
            
            model.uvs[curr_mesh][vec2_offset + 0] = uvs[uv_offset + 0];
            model.uvs[curr_mesh][vec2_offset + 1] = uvs[uv_offset + 1];
            
            const size_t normal_offset = index_list[2] * 3;
            LIB_ASSERT(normal_offset < (size_t)model.vertex_count);
            
            model.normals[curr_mesh][vec3_offset + 0] = normals[normal_offset + 0];
            model.normals[curr_mesh][vec3_offset + 1] = normals[normal_offset + 1];
            model.normals[curr_mesh][vec3_offset + 2] = normals[normal_offset + 2];
          }
          
          // Vertex
          {
            const size_t pos_offset = index_list[3] * 3;
            LIB_ASSERT(pos_offset < (size_t)model.vertex_count);
          
            model.verts[curr_mesh][vec3_offset + 3] = positions[pos_offset + 0];
            model.verts[curr_mesh][vec3_offset + 4] = positions[pos_offset + 1];
            model.verts[curr_mesh][vec3_offset + 5] = positions[pos_offset + 2];
            
            const size_t uv_offset = index_list[4] * 2;
            LIB_ASSERT(uv_offset < (size_t)model.vertex_count);
            
            model.uvs[curr_mesh][vec2_offset + 2] = uvs[uv_offset + 0];
            model.uvs[curr_mesh][vec2_offset + 3] = uvs[uv_offset + 1];
            
            const size_t normal_offset = index_list[5] * 3;
            LIB_ASSERT(normal_offset < (size_t)model.vertex_count);
            
            model.normals[curr_mesh][vec3_offset + 3] = normals[normal_offset + 0];
            model.normals[curr_mesh][vec3_offset + 4] = normals[normal_offset + 1];
            model.normals[curr_mesh][vec3_offset + 5] = normals[normal_offset + 2];
          }
          
          // Vertex
          {
            const size_t pos_offset = index_list[6] * 3;
            LIB_ASSERT(pos_offset < (size_t)model.vertex_count);
          
            model.verts[curr_mesh][vec3_offset + 6] = positions[pos_offset + 0];
            model.verts[curr_mesh][vec3_offset + 7] = positions[pos_offset + 1];
            model.verts[curr_mesh][vec3_offset + 8] = positions[pos_offset + 2];
            
            const size_t uv_offset = index_list[7] * 2;
            LIB_ASSERT(uv_offset < (size_t)model.vertex_count);
            
            model.uvs[curr_mesh][vec2_offset + 4] = uvs[uv_offset + 0];
            model.uvs[curr_mesh][vec2_offset + 5] = uvs[uv_offset + 1];
            
            const size_t normal_offset = index_list[8] * 3;
            LIB_ASSERT(normal_offset < (size_t)model.vertex_count);
            
            model.normals[curr_mesh][vec3_offset + 6] = normals[normal_offset + 0];
            model.normals[curr_mesh][vec3_offset + 7] = normals[normal_offset + 1];
            model.normals[curr_mesh][vec3_offset + 8] = normals[normal_offset + 2];
          }
        
          curr_tri += 1;
        }
      }
    }
  }
  else
  {
    LOG_ERROR("Failed to open file")
  }
  
  // Close files
  if(obj_file)
  {
    fclose(obj_file);
  }
  
  if(mat_file)
  {
    fclose(mat_file);
  }
  
  return model;
}


} // ns
} // ns


#endif // impl guard
#endif // impl inc
