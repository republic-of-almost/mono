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


struct material
{
  char *name;
  char *texture_01_path;
  char *texture_02_path;
  char *texture_03_path;
};


struct model
{
  char **name;
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


namespace LIB_NS_NAME {
namespace model_import {


model
load_obj_from_file(
  const char *filename,
  const size_t size_hint)
{
  LIB_NS_NAME::model return_model{};

  // Load file
  FILE *file = fopen(filename, "r");
  FILE *mat_file = nullptr;
  
  if(file)
  {
    /*
      Parse the file to find out how may meshes and vertexes there are.
      This ables us to reduce the number of allocations we have to make.
    */
    uint32_t mesh_count = 0;
    lib::array<uint32_t, 32> triangle_count;
    lib::array<uint32_t, 32> name_size;
    
    while(true)
    {
      char line[128]{};
      
      const int result = fscanf(file, "%s", line);
      
      /*
        All done.
      */
      if(result == EOF)
      {
        break;
      }
      
      /*
        Material
        --
        If there is a material file listed go get its details.
      */
      if(strcmp("mtllib", line) == 0)
      {
        char name[128]{};
        
        fscanf(
          file,
          "%s\n",
          name
        );
      
        std::string path = LIB_NS_NAME::string::get_dir_from_filepath(filename) + std::string(name);
        
        mat_file = fopen(path.c_str(), "r");
        
        if(mat_file)
        {
          uint32_t number_of_mats = 0;
          lib::array<uint32_t, 32> mat_name_size;
          
          /*
            Scan through the
          */
          while(true)
          {
            char mat_line[128]{};
          
            const int result = fscanf(mat_file, "%s", mat_line);
            
            if(result == EOF)
            {
              break;
            }
            
            if(strcmp("newmtl", mat_line) == 0)
            {
              number_of_mats += 1;
              
              char mat_name[128]{};
              
              fscanf(
                mat_file,
                "%s",
                mat_name
              );
              
              mat_name_size.emplace_back((uint32_t)strlen(mat_name));
            }
          }
          
          /*
            Allocate the materials.
          */
          
          {
            return_model.mesh_material = (material*)MODEL_ALLOC(sizeof(material) * number_of_mats);
            for(uint32_t i = 0; i < number_of_mats; ++i)
            {
              return_model.mesh_material[i].name = (char*)MODEL_ALLOC(sizeof(char) * mat_name_size[i]);
            }
            return_model.material_count = number_of_mats;
          }
          
          number_of_mats = 0;
          fseek(mat_file, 0, SEEK_SET);
          
          while(true)
          {
            char mat_line[128]{};
          
            const int result = fscanf(mat_file, "%s", mat_line);
            
            if(result == EOF)
            {
              break;
            }
            
            if(strcmp("newmtl", mat_line) == 0)
            {
              
              char mat_name[128]{};
              
              fscanf(
                mat_file,
                "%s",
                mat_name
              );
              
              strcat(return_model.mesh_material[number_of_mats].name, mat_name);
              
              number_of_mats += 1;
            }
            
            if(strcmp("map_Kd", mat_line) == 0)
            {
              char path[1024]{};
              
              fscanf(
                mat_file,
                "%s",
                path
              );
              
              return_model.mesh_material[number_of_mats - 1].texture_01_path = (char*)MODEL_ALLOC(strlen(path) * sizeof(char));
              memset(return_model.mesh_material[number_of_mats - 1].texture_01_path, 0, sizeof(char) * strlen(path));
              strcat(return_model.mesh_material[number_of_mats - 1].texture_01_path, path);
            }
          }
        }
      }
      
      /*
       
      */
      if(strcmp("o", line) == 0)
      {
        char name[128]{};
      
        fscanf(
          file,
          "%s\n",
          name
        );
        
        const uint32_t str_size = strlen(name);
        name_size.emplace_back(str_size);
      
        // This might be a new mesh so zero the counters.
        mesh_count += 1;
      }
      
      if(strcmp("f", line) == 0)
      {
        LIB_ASSERT(mesh_count > 0);
        triangle_count[mesh_count - 1] += 1;
      }
    }
    
    // Setup return model
    {
      return_model.mesh_count = mesh_count;
      
      return_model.name = (char**)MODEL_ALLOC(sizeof(char*) * mesh_count);
      for(uint32_t i = 0; i < mesh_count; ++i)
      {
        return_model.name[i] = (char*)MODEL_ALLOC(sizeof(char) * name_size[i]);
      }
    
      return_model.verts = (float**)MODEL_ALLOC(sizeof(float*) * mesh_count);
      for(uint32_t i = 0; i < mesh_count; ++i)
      {
        return_model.verts[i] = (float*)MODEL_ALLOC(sizeof(float) * triangle_count[i] * 9);
      }
      
      return_model.normals = (float**)MODEL_ALLOC(sizeof(float*) * mesh_count);
      for(uint32_t i = 0; i < mesh_count; ++i)
      {
        return_model.normals[i] = (float*)MODEL_ALLOC(sizeof(float) * triangle_count[i] * 9);
      }
      
      return_model.uvs = (float**)MODEL_ALLOC(sizeof(float*) * mesh_count);
      for(uint32_t i = 0; i < mesh_count; ++i)
      {
        return_model.uvs[i] = (float*)MODEL_ALLOC(sizeof(float) * triangle_count[i] * 6);
      }
      
      // Tri count
      {
        return_model.triangle_count = (uint32_t*)MODEL_ALLOC(sizeof(uint32_t) * mesh_count);
        
        for(uint32_t i = 0; i < mesh_count; ++i)
        {
          return_model.triangle_count[i] = triangle_count[i];
        }
      }
      
      // Vert count
      {
        return_model.vertex_count = (uint32_t*)MODEL_ALLOC(sizeof(uint32_t) * mesh_count);
        
        for(uint32_t i = 0; i < mesh_count; ++i)
        {
          return_model.vertex_count[i] = triangle_count[i] * 3;
        }
      }
      
      // Material ID
      {
        return_model.material_id = (int32_t*)MODEL_ALLOC(sizeof(int32_t) * mesh_count);
        
        // Memset
        for(uint32_t i = 0; i < mesh_count; ++i)
        {
          return_model.material_id[i] = -1;
        }
      }
    }
    
    // Fill the mesh buffers
    {
      fseek(file, 0, SEEK_SET);
    
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
        char line[128]{};
        
        const int result = fscanf(file, "%s", line);
        
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
          char name[128]{};
      
          fscanf(
            file,
            "%s\n",
            name
          );
          
//          memcpy(return_model.name[this_mesh], name, sizeof(char) * strlen(name));
          strcat(return_model.name[this_mesh], name);
        
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
            file,
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
            file,
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
            file,
            "%f %f %f\n",
            &normals[curr_size + 0],
            &normals[curr_size + 1],
            &normals[curr_size + 2]
          );
        }
        
        else if(strcmp("usemtl", line) == 0)
        {
          char mat_name[128];
        
          fscanf(
            file,
            "%s\n",
            mat_name
          );
          
          // Search mats for this
          for(uint32_t i = 0; i < return_model.material_count; ++i)
          {
            if(strcmp(return_model.mesh_material[i].name, mat_name) == 0)
            {
              return_model.material_id[this_mesh - 1] = i;
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
          uint32_t index_list[9];
        
          // Vertex / texture / normal
        
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
          
          const size_t curr_mesh = this_mesh - 1;
          
          return_model.verts[curr_mesh][(curr_tri * 9) + 0] = positions[(index_list[0] * 3) + 0];
          return_model.verts[curr_mesh][(curr_tri * 9) + 1] = positions[(index_list[0] * 3) + 1];
          return_model.verts[curr_mesh][(curr_tri * 9) + 2] = positions[(index_list[0] * 3) + 2];
          
          return_model.uvs[curr_mesh][(curr_tri * 6) + 0] = uvs[(index_list[1] * 2) + 0];
          return_model.uvs[curr_mesh][(curr_tri * 6) + 1] = uvs[(index_list[1] * 2) + 1];
          
          return_model.normals[curr_mesh][(curr_tri * 9) + 0] = normals[(index_list[2] * 3) + 0];
          return_model.normals[curr_mesh][(curr_tri * 9) + 1] = normals[(index_list[2] * 3) + 1];
          return_model.normals[curr_mesh][(curr_tri * 9) + 2] = normals[(index_list[2] * 3) + 2];
          
          // --

          return_model.verts[curr_mesh][(curr_tri * 9) + 3] = positions[(index_list[3] * 3) + 0];
          return_model.verts[curr_mesh][(curr_tri * 9) + 4] = positions[(index_list[3] * 3) + 1];
          return_model.verts[curr_mesh][(curr_tri * 9) + 5] = positions[(index_list[3] * 3) + 2];
          
          return_model.uvs[curr_mesh][(curr_tri * 6) + 2] = uvs[(index_list[4] * 2) + 0];
          return_model.uvs[curr_mesh][(curr_tri * 6) + 3] = uvs[(index_list[4] * 2) + 1];
          
          return_model.normals[curr_mesh][(curr_tri * 9) + 3] = normals[(index_list[5] * 3) + 0];
          return_model.normals[curr_mesh][(curr_tri * 9) + 4] = normals[(index_list[5] * 3) + 1];
          return_model.normals[curr_mesh][(curr_tri * 9) + 5] = normals[(index_list[5] * 3) + 2];
          
          // --
          
          return_model.verts[curr_mesh][(curr_tri * 9) + 6] = positions[(index_list[6] * 3) + 0];
          return_model.verts[curr_mesh][(curr_tri * 9) + 7] = positions[(index_list[6] * 3) + 1];
          return_model.verts[curr_mesh][(curr_tri * 9) + 8] = positions[(index_list[6] * 3) + 2];
          
          return_model.uvs[curr_mesh][(curr_tri * 6) + 4] = uvs[(index_list[7] * 2) + 0];
          return_model.uvs[curr_mesh][(curr_tri * 6) + 5] = uvs[(index_list[7] * 2) + 1];
          
          return_model.normals[curr_mesh][(curr_tri * 9) + 6] = normals[(index_list[8] * 3) + 0];
          return_model.normals[curr_mesh][(curr_tri * 9) + 7] = normals[(index_list[8] * 3) + 1];
          return_model.normals[curr_mesh][(curr_tri * 9) + 8] = normals[(index_list[8] * 3) + 2];
        
          curr_tri += 1;
        }
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
