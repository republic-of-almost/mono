#include <data/asset_loader.hpp>
#include <nil/resource/texture.hpp>
#include <nil/resource/mesh.hpp>
#include <tinydir/tinydir.h>
#include <lib/platform.hpp>
#include <lib/assert.hpp>
#include <lib/model.hpp>
#include <lib/directory.hpp>
#include <lib/string.hpp>
#include <lib/array.hpp>
#include <json/json.h>


namespace Nil {
namespace Data {


void
load_assets()
{
  // Test GLTF //
  /*
    This is experimental
  */
  {
    char path[2048]{};
    strcat(path, lib::dir::exe_path());

    #ifndef LIB_PLATFORM_MAC
    strcat(path, "assets/mesh/");
    #else
    strcat(path, "../Resources/assets/mesh/");
    #endif

    strcat(path, "test_level.gltf");

    const size_t count = lib::file::bytes(path);
    char *buffer = (char*)malloc(count); 
    memset(buffer, 0, sizeof(count));
    

    lib::file::get_contents(path, buffer, count);

    // GLTF info

    struct gltf_accessor
    {
      int32_t buffer_view;
      int32_t component_type;
      int32_t size;
      float min[4];
      float max[4];
      enum {SCALAR, VEC2, VEC3, VEC4, } type;
      int32_t count;
    };

    lib::array<gltf_accessor> accessors;
    
    struct gltf_asset
    {
      char copyright[64];
      char generator[64];
      char version[64];
    };
    
    lib::array<gltf_asset> assets;
    
    struct gltf_buffer_view
    {
      int32_t buffer;
      int32_t buffer_length;
      int32_t byte_offset;
      int32_t target;
    };
    
    lib::array<gltf_buffer_view> buffer_views;

    // --

    constexpr uint32_t flags(
      json_parse_flags_default |
      json_parse_flags_allow_simplified_json |
      json_parse_flags_allow_trailing_comma
    );

    json_value_s *json_root = json_parse_ex(buffer, count, flags, 0, 0, 0);
    json_object_s *json_root_obj = (json_object_s*)json_root->payload;

    json_object_element_s *json_gltf_element = json_root_obj->start;

    json_string_s *json_element_name = json_gltf_element->name;
    json_value_s *json_element_value = json_gltf_element->value;
    
    
    while(json_gltf_element != nullptr)
    {
      /*
        Parse Accessors
      */
      if(strcmp(json_element_name->string, "accessors") == 0)
      {
        json_array_s *gltf_accessors = (json_array_s*)json_element_value->payload;
        json_array_element_s *gltf_buf = gltf_accessors->start;
        
        for(size_t i = 0; i < gltf_accessors->length; ++i)
        {
          gltf_accessor accessor{};
          
          json_value_s *val = gltf_buf->value;
          json_object_s *acc_obj = (json_object_s*)val->payload;
          json_object_element_s *acc_ele = acc_obj->start;
          
          while(acc_ele != nullptr)
          {
            json_string_s *acc_name = acc_ele->name;
            json_value_s *acc_value = acc_ele->value;
            
            if(strcmp(acc_name->string, "bufferView") == 0)
            {
              LIB_ASSERT(acc_value->type == json_type_number);
              
              json_number_s *buffer_view_val = (json_number_s*)acc_value->payload;
              accessor.buffer_view = atoi(buffer_view_val->number);
            }
            
            else if(strcmp(acc_name->string, "componentType") == 0)
            {
              LIB_ASSERT(acc_value->type == json_type_number);
            
              json_number_s *buffer_view_val = (json_number_s*)acc_value->payload;
              accessor.component_type = atoi(buffer_view_val->number);
            }
            
            else if(strcmp(acc_name->string, "count") == 0)
            {
              LIB_ASSERT(acc_value->type == json_type_number);
              
              json_number_s *buffer_view_val = (json_number_s*)acc_value->payload;
              accessor.count = atoi(buffer_view_val->number);
            }
            
            else if(strcmp(acc_name->string, "min") == 0)
            {
              LIB_ASSERT(acc_value->type == json_type_array);
            }
            
            else if(strcmp(acc_name->string, "max") == 0)
            {
              LIB_ASSERT(acc_value->type == json_type_array);
            }
            
            else if(strcmp(acc_name->string, "type") == 0)
            {
              LIB_ASSERT(acc_value->type == json_type_string);
              
              json_string_s *buffer_view_val = (json_string_s*)acc_value->payload;
              if(strcmp(buffer_view_val->string, "SCALAR") == 0)
              {
                accessor.type = gltf_accessor::SCALAR;
              }
              else if(strcmp(buffer_view_val->string, "VEC2") == 0)
              {
                accessor.type = gltf_accessor::VEC2;
              }
              else if(strcmp(buffer_view_val->string, "VEC3") == 0)
              {
                accessor.type = gltf_accessor::VEC3;
              }
              else if(strcmp(buffer_view_val->string, "VEC4") == 0)
              {
                accessor.type = gltf_accessor::VEC4;
              }
              else
              {
                // Something in this implimentation is missnig from the spec.
                LIB_ASSERT(false);
              }
            }
            
            // Next
            acc_ele = acc_ele->next;
          }
          
          accessors.emplace_back(accessor);
          gltf_buf = gltf_buf->next;
        }
      } // if accessors
      
      /*
        Parse Buffer Views
      */
      else if(strcmp(json_element_name->string, "bufferViews"))
      {
        json_array_s *gltf_buffer_views = (json_array_s*)json_element_value->payload;
        json_array_element_s *gltf_buf = gltf_buffer_views->start;
        
        for(size_t i = 0; i < gltf_buffer_views->length; ++i)
        {
          gltf_buffer_view buffer_view{};
          
          json_value_s *val = gltf_buf->value;
          json_object_s *acc_obj = (json_object_s*)val->payload;
          json_object_element_s *acc_ele = acc_obj->start;
          
          while(acc_ele != nullptr)
          {
            json_string_s *acc_name = acc_ele->name;
            json_value_s *acc_value = acc_ele->value;
            
            if(strcmp(acc_name->string, "buffer") == 0)
            {
              LIB_ASSERT(acc_value->type == json_type_number);
              
              
            }

          
            buffer_views.emplace_back(buffer_view);
            gltf_buf = gltf_buf->next;
          }
        }
      }
      
      /*
        Parse Buffers
      */
      else if(strcmp(json_element_name->string, "buffers"))
      {
        json_array_s *gltf_buffer_views = (json_array_s*)json_element_value->payload;
        json_array_element_s *gltf_buf = gltf_buffer_views->start;
        
        for(size_t i = 0; i < gltf_buffer_views->length; ++i)
        {
//          gltf_buffer_view buffer_view{};
          
          json_value_s *val = gltf_buf->value;
          json_object_s *acc_obj = (json_object_s*)val->payload;
          json_object_element_s *acc_ele = acc_obj->start;
          
          while(acc_ele != nullptr)
          {
            json_string_s *acc_name = acc_ele->name;
            json_value_s *acc_value = acc_ele->value;
            
            if(strcmp(acc_name->string, "buffer") == 0)
            {
              LIB_ASSERT(acc_value->type == json_type_number);
              
              
            }

          
//            buffer_views.emplace_back(buffer_view);
            gltf_buf = gltf_buf->next;
          }
        }
      }
      
      /*
        Parse Meshes
      */
      else if(strcmp(json_element_name->string, "meshes"))
      {
        json_array_s *gltf_buffer_views = (json_array_s*)json_element_value->payload;
        json_array_element_s *gltf_buf = gltf_buffer_views->start;
        
        for(size_t i = 0; i < gltf_buffer_views->length; ++i)
        {
//          gltf_buffer_view buffer_view{};
          
          json_value_s *val = gltf_buf->value;
          json_object_s *acc_obj = (json_object_s*)val->payload;
          json_object_element_s *acc_ele = acc_obj->start;
          
          while(acc_ele != nullptr)
          {
            json_string_s *acc_name = acc_ele->name;
            json_value_s *acc_value = acc_ele->value;
            
            if(strcmp(acc_name->string, "buffer") == 0)
            {
              LIB_ASSERT(acc_value->type == json_type_number);
              
              
            }

          
//            buffer_views.emplace_back(buffer_view);
            gltf_buf = gltf_buf->next;
          }
        }
      }
      
      /*
        Parse Nodes
      */
      else if(strcmp(json_element_name->string, "nodes"))
      {
        json_array_s *gltf_buffer_views = (json_array_s*)json_element_value->payload;
        json_array_element_s *gltf_buf = gltf_buffer_views->start;
        
        for(size_t i = 0; i < gltf_buffer_views->length; ++i)
        {
//          gltf_buffer_view buffer_view{};
          
          json_value_s *val = gltf_buf->value;
          json_object_s *acc_obj = (json_object_s*)val->payload;
          json_object_element_s *acc_ele = acc_obj->start;
          
          while(acc_ele != nullptr)
          {
            json_string_s *acc_name = acc_ele->name;
            json_value_s *acc_value = acc_ele->value;
            
            if(strcmp(acc_name->string, "buffer") == 0)
            {
              LIB_ASSERT(acc_value->type == json_type_number);
              
              
            }

          
//            buffer_views.emplace_back(buffer_view);
            gltf_buf = gltf_buf->next;
          }
        }
      }
      
      /*
        Parse Scene
      */
      else if(strcmp(json_element_name->string, "scene"))
      {
        json_array_s *gltf_buffer_views = (json_array_s*)json_element_value->payload;
        json_array_element_s *gltf_buf = gltf_buffer_views->start;
        
        for(size_t i = 0; i < gltf_buffer_views->length; ++i)
        {
//          gltf_buffer_view buffer_view{};
          
          json_value_s *val = gltf_buf->value;
          json_object_s *acc_obj = (json_object_s*)val->payload;
          json_object_element_s *acc_ele = acc_obj->start;
          
          while(acc_ele != nullptr)
          {
            json_string_s *acc_name = acc_ele->name;
            json_value_s *acc_value = acc_ele->value;
            
            if(strcmp(acc_name->string, "buffer") == 0)
            {
              LIB_ASSERT(acc_value->type == json_type_number);
              
              
            }

          
//            buffer_views.emplace_back(buffer_view);
            gltf_buf = gltf_buf->next;
          }
        }
      }
      
      /*
        Parse Scenes
      */
      else if(strcmp(json_element_name->string, "scenes"))
      {
        json_array_s *gltf_buffer_views = (json_array_s*)json_element_value->payload;
        json_array_element_s *gltf_buf = gltf_buffer_views->start;
        
        for(size_t i = 0; i < gltf_buffer_views->length; ++i)
        {
//          gltf_buffer_view buffer_view{};
          
          json_value_s *val = gltf_buf->value;
          json_object_s *acc_obj = (json_object_s*)val->payload;
          json_object_element_s *acc_ele = acc_obj->start;
          
          while(acc_ele != nullptr)
          {
            json_string_s *acc_name = acc_ele->name;
            json_value_s *acc_value = acc_ele->value;
            
            if(strcmp(acc_name->string, "buffer") == 0)
            {
              LIB_ASSERT(acc_value->type == json_type_number);
              
              
            }

          
//            buffer_views.emplace_back(buffer_view);
            gltf_buf = gltf_buf->next;
          }
        }
      }
      
      
    } // while json_element_value

  
    const size_t accessor_count = accessors.size();


    free(json_root);
  }


  // Textures //
  {
    char path[2048]{};
    strcat(path, lib::dir::exe_path());
    
    #ifndef LIB_PLATFORM_MAC
    strcat(path, "assets/texture");
    #else
    strcat(path, "../Resources/assets/texture");
    #endif

    _tinydir_char_t wpath[2048]{};
    
    #ifdef LIB_PLATFORM_WIN
    mbstowcs(wpath, path, 2048);
    #else
    memcpy(wpath, path, sizeof(path));
    #endif

    _tinydir_char_t dir_path[2048]{};
    _tinydir_strcat(dir_path, wpath);

    tinydir_dir dir;
    tinydir_open(&dir, dir_path);

    while (dir.has_next)
    {
      tinydir_file file;
      tinydir_readfile(&dir, &file);
      
      // We only load up png's right now.
      _tinydir_char_t *ext = TINYDIR_STRING("png");

      if (!file.is_dir)
      {
        if(_tinydir_strcmp(ext, file.extension) == 0)
        {
          char c_name[2048]{};
          
          #ifdef LIB_PLATFORM_WIN
          wcstombs(c_name, file.path, _tinydir_strlen(file.path));
          #else
          memcpy(c_name, file.path, _tinydir_strlen(file.path));
          #endif
        
          char load_name[2048]{};
          lib::string::filename_from_path(c_name, load_name, sizeof(load_name), true);

          Nil::Resource::Texture tex{};
          tex.name      = load_name;
          tex.data      = (uintptr_t)c_name;
          tex.data_size = strlen(c_name) + 1;
          tex.data_type = Nil::Resource::Texture::FILENAME;

          Nil::Resource::load(tex);
        }
      }

      tinydir_next(&dir);
    }

    tinydir_close(&dir);
  }

  
  // Meshes //
  {
    char path[2048]{};
    strcat(path, lib::dir::exe_path());
    
    #ifndef LIB_PLATFORM_MAC
    strcat(path, "assets/texture");
    #else
    strcat(path, "../Resources/assets/mesh");
    #endif

    _tinydir_char_t wpath[2048]{};
    
    #ifdef LIB_PLATFORM_WIN
    mbstowcs(wpath, path, 2048);
    #else
    memcpy(wpath, path, sizeof(path));
    #endif

    _tinydir_char_t dir_path[2048]{};
    _tinydir_strcat(dir_path, wpath);

    tinydir_dir dir;
    tinydir_open(&dir, dir_path);

    while (dir.has_next)
    {
      tinydir_file file;
      tinydir_readfile(&dir, &file);
      
      // We only load up png's right now.
      _tinydir_char_t *ext = TINYDIR_STRING("obj");

      if (!file.is_dir)
      {
        if(_tinydir_strcmp(ext, file.extension) == 0)
        {
          char c_name[2048]{};
          
          #ifdef LIB_PLATFORM_WIN
          wcstombs(c_name, file.path, _tinydir_strlen(file.path));
          #else
          memcpy(c_name, file.path, _tinydir_strlen(file.path));
          #endif
        
          lib::model m = lib::model_import::load_obj_from_file(c_name);
          
          for(size_t i = 0; i < m.mesh_count; ++i)
          {
            char load_name[2048]{};
            lib::string::filename_from_path(c_name, load_name, sizeof(load_name), true);
            
            Nil::Resource::Mesh mesh{};
            
            if(!Nil::Resource::find_by_name(load_name, mesh))
            {
              mesh.name                = load_name;
              mesh.position_vec3       = m.verts[i];
              mesh.normal_vec3         = m.normals[i];
              mesh.color_vec4          = nullptr;
              mesh.texture_coords_vec2 = m.uvs[i];
              mesh.triangle_count      = m.triangle_count[i];

              const bool loaded = Nil::Resource::load(mesh);
              LIB_ASSERT(loaded);
            }
          }
        }
      }

      tinydir_next(&dir);
    }

    tinydir_close(&dir);
  }
}


} // ns
} // ns
