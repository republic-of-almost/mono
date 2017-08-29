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
#include <lib/logging.hpp>
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

    struct gltf_asset
    {
      char copyright[64];
      char generator[64];
      char version[64];
    };
    
    
    struct gltf_buffer_view
    {
      int32_t buffer;
      int32_t byte_length;
      int32_t byte_offset;
      int32_t target;
    };
    
    struct gltf_buffer
    {
      size_t  byte_length;
      uint8_t *buffer;
    };
    
    struct gltf_node
    {
      uint32_t mesh;
      
      char name[64];
      
      float translation[3];
      float scale[3];
      float rotation[4];
    };
    
    
    lib::array<gltf_asset> assets;
    lib::array<gltf_buffer_view> buffer_views;
    lib::array<gltf_buffer> buffers;
    lib::array<gltf_accessor> accessors;
    lib::array<gltf_node> nodes;

    // --

    constexpr uint32_t flags(
      json_parse_flags_default |
      json_parse_flags_allow_simplified_json | // bitsquid json
      json_parse_flags_allow_trailing_comma
    );

    json_value_s *json_root                        = json_parse_ex(buffer, count, flags, 0, 0, 0);
    const json_object_s *json_root_obj             = (json_object_s*)json_root->payload;
    const json_object_element_s *json_gltf_element = json_root_obj->start;

    while(json_gltf_element != nullptr)
    {
      const json_string_s *json_element_name = json_gltf_element->name;
      const json_value_s *json_element_value = json_gltf_element->value;
    
      /*
        Parse Accessors
      */
      if(strcmp(json_element_name->string, "accessors") == 0)
      {
        LIB_ASSERT(json_element_value->type == json_type_array);
      
        const json_array_s *json_accessors        = (json_array_s*)json_element_value->payload;
        const json_array_element_s *json_accessor = json_accessors->start;
        
        while(json_accessor != nullptr)
        {
          LIB_ASSERT(json_accessor);
        
          gltf_accessor accessor{};
          
          const json_value_s *json_value              = json_accessor->value;
          const json_object_s *json_field_obj         = (json_object_s*)json_value->payload;
          const json_object_element_s *json_field_ele = json_field_obj->start;
          
          while(json_field_ele != nullptr)
          {
            LIB_ASSERT(json_field_ele);
          
            const json_string_s *json_field_name = json_field_ele->name;
            const json_value_s *json_field_value = json_field_ele->value;
            
            if(strcmp(json_field_name->string, "bufferView") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_number);
              
              const json_number_s *val = (json_number_s*)json_field_value->payload;
              accessor.buffer_view     = atoi(val->number);
            }
            
            else if(strcmp(json_field_name->string, "componentType") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_number);
            
              const json_number_s *val = (json_number_s*)json_field_value->payload;
              accessor.component_type  = atoi(val->number);
            }
            
            else if(strcmp(json_field_name->string, "count") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_number);
              
              const json_number_s *val = (json_number_s*)json_field_value->payload;
              accessor.count           = atoi(val->number);
            }
            
            else if(strcmp(json_field_name->string, "min") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_array);
            }
            
            else if(strcmp(json_field_name->string, "max") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_array);
            }
            
            else if(strcmp(json_field_name->string, "type") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_string);
              
              const json_string_s *val = (json_string_s*)json_field_value->payload;
              if(strcmp(val->string, "SCALAR") == 0)
              {
                accessor.type = gltf_accessor::SCALAR;
              }
              else if(strcmp(val->string, "VEC2") == 0)
              {
                accessor.type = gltf_accessor::VEC2;
              }
              else if(strcmp(val->string, "VEC3") == 0)
              {
                accessor.type = gltf_accessor::VEC3;
              }
              else if(strcmp(val->string, "VEC4") == 0)
              {
                accessor.type = gltf_accessor::VEC4;
              }
              else
              {
                // Missing something in this impl.
                LIB_ASSERT(false);
              }
            }
            else
            {
              // Missing something in this impl.
              LIB_ASSERT(false);
            }
            
            // Next element
            json_field_ele = json_field_ele->next;
          }
          
          // Next field
          accessors.emplace_back(accessor);
          json_accessor = json_accessor->next;
        }
      } // if accessors
      
      /*
        Parse Buffer Views
      */
      else if(strcmp(json_element_name->string, "bufferViews") == 0)
      {
        LIB_ASSERT(json_element_value->type == json_type_array);
      
        const json_array_s *json_buffer_views        = (json_array_s*)json_element_value->payload;
        const json_array_element_s *json_buffer_view = json_buffer_views->start;
        
        while(json_buffer_view != nullptr)
        {
          LIB_ASSERT(json_buffer_view);
        
          gltf_buffer_view buffer_view{};
          
          const json_value_s *json_value              = json_buffer_view->value;
          const json_object_s *json_field_obj         = (json_object_s*)json_value->payload;
          const json_object_element_s *json_field_ele = json_field_obj->start;
          
          while(json_field_ele != nullptr)
          {
            LIB_ASSERT(json_field_ele);
          
            const json_string_s *json_field_name = json_field_ele->name;
            const json_value_s *json_field_value = json_field_ele->value;
            
            if(strcmp(json_field_name->string, "buffer") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_number);
              
              const json_number_s *val = (json_number_s*)json_field_value->payload;
              buffer_view.buffer       = atoi(val->number);
            }
            else if(strcmp(json_field_name->string, "byteLength") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_number);
              
              const json_number_s *val = (json_number_s*)json_field_value->payload;
              buffer_view.byte_length  = atoi(val->number);
            }
            else if(strcmp(json_field_name->string, "byteOffset") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_number);
              
              const json_number_s *val = (json_number_s*)json_field_value->payload;
              buffer_view.byte_offset  = atoi(val->number);
            }
            else if(strcmp(json_field_name->string, "target") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_number);
              
              const json_number_s *val = (json_number_s*)json_field_value->payload;
              buffer_view.target       = atoi(val->number);
            }
            else
            {
              // Missing something in this impl.
              LIB_ASSERT(false);
            }
            
            // Next element
            json_field_ele = json_field_ele->next;
          }
          
          // Next Field
          buffer_views.emplace_back(buffer_view);
          json_buffer_view = json_buffer_view->next;
        }
      }
      
      /*
        Parse Buffers
      */
      else if(strcmp(json_element_name->string, "buffers") == 0)
      {
        LIB_ASSERT(json_element_value);
      
        const json_array_s *json_buffers        = (json_array_s*)json_element_value->payload;
        const json_array_element_s *json_buffer = json_buffers->start;
        
        while(json_buffer != nullptr)
        {
          LIB_ASSERT(json_buffer);
        
          gltf_buffer buffer{};

          const json_value_s *json_value              = json_buffer->value;
          const json_object_s *json_field_obj         = (json_object_s*)json_value->payload;
          const json_object_element_s *json_field_ele = json_field_obj->start;
          
          while(json_field_ele != nullptr)
          {
            LIB_ASSERT(json_field_ele);
          
            const json_string_s *json_field_name = json_field_ele->name;
            const json_value_s *json_field_value = json_field_ele->value;
            
            if(strcmp(json_field_name->string, "byteLength") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_number);
              
              const json_number_s *val = (json_number_s*)json_field_value->payload;
              buffer.byte_length = atoi(val->number);
            }
            else if(strcmp(json_field_name->string, "uri") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_string);
              
              const json_string_s *val = (json_string_s*)json_field_value->payload;
            
              LOG_TODO_ONCE("Need to get GLTF URI");
            }
            else
            {
              // Missing something from spec.
              LIB_ASSERT(false);
            }

            // Next element
            json_field_ele = json_field_ele->next;
          }
          
          // Next Field
          buffers.emplace_back(buffer);
          json_buffer = json_buffer->next;
        }
      }
      
      /*
        Parse Meshes
      */
      else if(strcmp(json_element_name->string, "meshes") == 0)
      {
        LIB_ASSERT(json_element_value->type == json_type_array);
      
        const json_array_s *json_meshes       = (json_array_s*)json_element_value->payload;
        const json_array_element_s *json_mesh = json_meshes->start;
        
        while(json_mesh != nullptr)
        {
          LIB_ASSERT(json_mesh);
        
//          gltf_buffer_view buffer_view{};
          
          const json_value_s *json_val                = json_mesh->value;
          const json_object_s *json_field_obj         = (json_object_s*)json_val->payload;
          const json_object_element_s *json_field_ele = json_field_obj->start;
          
          while(json_field_ele != nullptr)
          {
            LIB_ASSERT(json_field_ele);
          
            const json_string_s *acc_name = json_field_ele->name;
            const json_value_s *acc_value = json_field_ele->value;
            
            if(strcmp(acc_name->string, "buffer") == 0)
            {
              LIB_ASSERT(acc_value->type == json_type_number);
              
            }

            // Next Element
            json_field_ele = json_field_ele->next;
          }
          
          // Next Field
          //            buffer_views.emplace_back(buffer_view);
          json_mesh = json_mesh->next;
        }
      }
      
      /*
        Parse Nodes
      */
      else if(strcmp(json_element_name->string, "nodes") == 0)
      {
        LIB_ASSERT(json_element_value->type == json_type_array);
      
        const json_array_s *json_nodes = (json_array_s*)json_element_value->payload;
        const json_array_element_s *json_node = json_nodes->start;
        
        while(json_node != nullptr)
        {
          LIB_ASSERT(json_node);
        
          gltf_node node{};
          
          const json_value_s *json_value              = json_node->value;
          const json_object_s *json_field_obj         = (json_object_s*)json_value->payload;
          const json_object_element_s *json_field_ele = json_field_obj->start;
          
          while(json_field_ele != nullptr)
          {
            LIB_ASSERT(json_field_ele);
          
            const json_string_s *json_field_name = json_field_ele->name;
            const json_value_s *json_field_value = json_field_ele->value;
            
            if(strcmp(json_field_name->string, "mesh") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_number);
              
              json_number_s *val = (json_number_s*)json_field_value->payload;
              node.mesh = atoi(val->number);
            }
            else if(strcmp(json_field_name->string, "name") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_string);
              
              json_string_s *val = (json_string_s*)json_field_value->payload;
              const size_t len = val->string_size > 63 ? 63 : val->string_size;
              memcpy(node.name, val->string, len);
            }
            else if(strcmp(json_field_name->string, "rotation") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_array);
            }
            else if(strcmp(json_field_name->string, "translation") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_array);
            }
            else if(strcmp(json_field_name->string, "scale") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_array);
            }
            
            // Next Element
            json_field_ele = json_field_ele->next;
          }
          
          // Next Field
          nodes.emplace_back(node);
          json_node = json_node->next;
        }
      }
      
      /*
        Parse Scene
      */
      else if(strcmp(json_element_name->string, "scene") == 0)
      {
        // Just one scene it seems.
      }
      
      /*
        Parse Scenes
      */
      else if(strcmp(json_element_name->string, "scenes") == 0)
      {
        LIB_ASSERT(json_element_value->type == json_type_array);
      
        const json_array_s *json_scenes        = (json_array_s*)json_element_value->payload;
        const json_array_element_s *json_scene = json_scenes->start;
        
        while(json_scene != nullptr)
        {
          LIB_ASSERT(json_scene);
        
//          gltf_buffer_view buffer_view{};
          
          const json_value_s *json_value              = json_scene->value;
          const json_object_s *json_field_obj         = (json_object_s*)json_value->payload;
          const json_object_element_s *json_field_ele = json_field_obj->start;
          
          while(json_field_ele != nullptr)
          {
            const json_string_s *json_field_name = json_field_ele->name;
            const json_value_s *json_field_value = json_field_ele->value;
            
            if(strcmp(json_field_name->string, "buffer") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_number);
              
            }
            
            // Next Element
            json_field_ele = json_field_ele->next;
          }
          
          // Next Field
          //            buffer_views.emplace_back(buffer_view);
          json_scene = json_scene->next;
        }
      }
      
      json_gltf_element = json_gltf_element->next;
    } // while json_element_value

    const size_t accessor_count = accessors.size();
    const size_t asset_count = assets.size();
    const size_t buffer_view_count = buffer_views.size();
    const size_t node_size = nodes.size();


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
