#include <data/asset_loader.hpp>
#include <nil/resource/texture.hpp>
#include <nil/resource/mesh.hpp>
#include <nil/data/transform.hpp>
#include <nil/data/renderable.hpp>
#include <nil/node.hpp>
#include <tinydir/tinydir.h>
#include <lib/platform.hpp>
#include <lib/assert.hpp>
#include <lib/model.hpp>
#include <lib/directory.hpp>
#include <lib/string.hpp>
#include <lib/array.hpp>
#include <lib/logging.hpp>
#include <json/json.h>



namespace {


static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};


void build_decoding_table() {

    decoding_table = (char*)malloc(256);

    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}


char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {

    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = (char*)malloc(*output_length);
    if (encoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    return encoded_data;
}


unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {

    if (decoding_table == NULL) { build_decoding_table(); }

    if (input_length % 4 != 0) return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = (unsigned char*)malloc(*output_length);
    if (decoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[(int)data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[(int)data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[(int)data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[(int)data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return decoded_data;
}



void base64_cleanup() {
    free(decoding_table);
}


} // anon ns


namespace Nil {
namespace Data {


void
load_assets()
{
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

//              const bool loaded = Nil::Resource::load(mesh);
//              LIB_ASSERT(loaded);
            }
          }
        }
      }

      tinydir_next(&dir);
    }

    tinydir_close(&dir);
  }
  
  
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
      enum { SCALAR, VEC2, VEC3, VEC4, } type;
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
    
    struct gltf_primitives
    {
      int32_t attr_normal;
      int32_t attr_position;
      int32_t attr_tangent;
      int32_t attr_texcoord_0;
      int32_t indices;
      int32_t material;
    };
    
    struct gltf_mesh
    {
      char name[64];
      gltf_primitives primitives;
    };
    
    
    struct gltf_scene
    {
      char name[64];
      uint32_t *nodes;
      size_t node_count;
    };
    
    struct gltf_image
    {
      uint8_t *uri;
    };
    
    struct gltf_sampler
    {
    };
    
    struct gltf_texture
    {
      uint32_t sampler;
      uint32_t source;
    };
    
    
    lib::array<gltf_asset>        assets;
    lib::array<gltf_buffer_view>  buffer_views;
    lib::array<gltf_buffer>       buffers;
    lib::array<gltf_accessor>     accessors;
    lib::array<gltf_node>         nodes;
    lib::array<gltf_mesh>         meshes;
    lib::array<gltf_scene>        scenes;
    lib::array<gltf_image>        images;
    lib::array<gltf_sampler>      samplers;
    lib::array<gltf_texture>      textures;

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
              
              const char *starts_with = "data:application/octet-stream;base64,";
              const char *ends_width = ".bin"; // impl
              
              size_t length;
              buffer.buffer = (uint8_t*)base64_decode(&val->string[strlen(starts_with)], val->string_size - strlen(starts_with), &length);
              
              
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
        Parse Images
      */
      else if(strcmp(json_element_name->string, "images") == 0)
      {
        LOG_TODO_ONCE("Add image parsing support");
      }
      
      /*
        Parse Textures
      */
      else if(strcmp(json_element_name->string, "textures") == 0)
      {
        LOG_TODO_ONCE("Add texture parsing support");
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
        
          gltf_mesh mesh{};
          
          const json_value_s *json_val                = json_mesh->value;
          const json_object_s *json_field_obj         = (json_object_s*)json_val->payload;
          const json_object_element_s *json_field_ele = json_field_obj->start;
          
          while(json_field_ele != nullptr)
          {
            LIB_ASSERT(json_field_ele);
          
            const json_string_s *json_field_name = json_field_ele->name;
            const json_value_s *json_field_value = json_field_ele->value;
            
            if(strcmp(json_field_name->string, "name") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_string);
              
              const json_string_s *val = (json_string_s*)json_field_value->payload;
              const size_t len = val->string_size > 64 ? 63 : val->string_size;
              memcpy(mesh.name, val->string, len);
            }
            else if(strcmp(json_field_name->string, "primitives") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_array);
              
              const json_array_s *json_prim_array       = (json_array_s*)json_field_value->payload;
              const json_array_element_s *json_prim_ele = json_prim_array->start;
              
              while(json_prim_ele != nullptr)
              {
                LIB_ASSERT(json_prim_ele);
                
                const json_value_s *json_prim_val  = json_prim_ele->value;
                const json_object_s *json_prim_obj = (json_object_s*)json_prim_val->payload;
                const json_object_element_s *json_prim = json_prim_obj->start;
                
                while(json_prim != nullptr)
                {
                  LIB_ASSERT(json_prim);
                
                  const json_string_s *json_prim_name = json_prim->name;
                  const json_value_s *json_prim_value = json_prim->value;

                  if(strcmp(json_prim_name->string, "attributes") == 0)
                  {
                    LIB_ASSERT(json_prim_value->type == json_type_object);
                    
                    const json_object_s *attr_obj = (json_object_s*)json_prim_value->payload;
                    const json_object_element_s *attr_ele = attr_obj->start;
                    
                    while(attr_ele != nullptr)
                    {
                      const json_value_s *attr_val = attr_ele->value;
                      const json_string_s *attr_name = attr_ele->name;
                      
                      if(strcmp(attr_name->string, "POSITION") == 0)
                      {
                        LIB_ASSERT(attr_val->type == json_type_number);
                        
                        const json_number_s *val = (json_number_s*)attr_val->payload;
                        mesh.primitives.attr_position = atoi(val->number);
                      }
                      else if(strcmp(attr_name->string, "NORMAL") == 0)
                      {
                        LIB_ASSERT(attr_val->type == json_type_number);
                      
                        const json_number_s *val = (json_number_s*)attr_val->payload;
                        mesh.primitives.attr_normal = atoi(val->number);
                      }
                      else if(strcmp(attr_name->string, "TANGENT") == 0)
                      {
                        LIB_ASSERT(attr_val->type == json_type_number);
                        
                        const json_number_s *val = (json_number_s*)attr_val->payload;
                        mesh.primitives.attr_tangent = atoi(val->number);
                      }
                      else if(strcmp(attr_name->string, "TEXCOORD_0") == 0)
                      {
                        LIB_ASSERT(attr_val->type == json_type_number);
                        
                        const json_number_s *val = (json_number_s*)attr_val->payload;
                        mesh.primitives.attr_texcoord_0 = atoi(val->number);
                      }
                      
                      attr_ele = attr_ele->next;
                    }
                  }
                  else if(strcmp(json_prim_name->string, "indices") == 0)
                  {
                    LIB_ASSERT(json_prim_value->type == json_type_number);
                    
                    const json_number_s *val = (json_number_s*)json_prim_value->payload;
                    mesh.primitives.indices = atoi(val->number);

                  }
                  else if(strcmp(json_prim_name->string, "material") == 0)
                  {
                    LIB_ASSERT(json_prim_value->type == json_type_number);
                    
                    const json_number_s *val = (json_number_s*)json_prim_value->payload;
                    mesh.primitives.material = atoi(val->number);
                  }
                  else
                  {
                    // Missing something.
                    LIB_ASSERT(false);
                  }
                  
                  json_prim = json_prim->next;
                }
              
                json_prim_ele = json_prim_ele->next;
              }
            }
            else
            {
              // Missing something.
              LIB_ASSERT(false);
            }

            // Next Element
            json_field_ele = json_field_ele->next;
          }
          
          // Next Field
          meshes.emplace_back(mesh);
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
          node.scale[0] = 1.f; node.scale[1] = 1.f; node.scale[2] = 1.f;
          node.rotation[0] = 0.f; node.rotation[3] = 0.f; node.rotation[3] = 0.f; node.rotation[3] = 1.f;
          
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
              
              const json_array_s *json_arr_rot = (json_array_s*)json_field_value->payload;
              const json_array_element_s *json_arr_rot_ele = json_arr_rot->start;
              
              if(json_arr_rot_ele)
              {
                const json_value_s *json_rot_value = json_arr_rot_ele->value;
                LIB_ASSERT(json_rot_value->type == json_type_number);
                
                const json_number_s *json_rot = (json_number_s*)json_rot_value->payload;
                node.rotation[0] = atof(json_rot->number);
              }
              
              json_arr_rot_ele = json_arr_rot_ele->next;

              if(json_arr_rot_ele)
              {
                const json_value_s *json_rot_value = json_arr_rot_ele->value;
                LIB_ASSERT(json_rot_value->type == json_type_number);
                
                const json_number_s *json_rot = (json_number_s*)json_rot_value->payload;
                node.rotation[1] = atof(json_rot->number);
              }

              json_arr_rot_ele = json_arr_rot_ele->next;

              if(json_arr_rot_ele)
              {
                const json_value_s *json_rot_value = json_arr_rot_ele->value;
                LIB_ASSERT(json_rot_value->type == json_type_number);
                
                const json_number_s *json_rot = (json_number_s*)json_rot_value->payload;
                node.rotation[2] = atof(json_rot->number);
              }
              
              json_arr_rot_ele = json_arr_rot_ele->next;

              if(json_arr_rot_ele)
              {
                const json_value_s *json_rot_value = json_arr_rot_ele->value;
                LIB_ASSERT(json_rot_value->type == json_type_number);
                
                const json_number_s *json_rot = (json_number_s*)json_rot_value->payload;
                node.rotation[3] = atof(json_rot->number);
              }
            }
            else if(strcmp(json_field_name->string, "translation") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_array);
              
              const json_array_s *json_arr_tran = (json_array_s*)json_field_value->payload;
              const json_array_element_s *json_arr_tran_ele = json_arr_tran->start;
              
              if(json_arr_tran_ele)
              {
                const json_value_s *json_tran_value = json_arr_tran_ele->value;
                LIB_ASSERT(json_tran_value->type == json_type_number);
                
                const json_number_s *json_tran = (json_number_s*)json_tran_value->payload;
                node.translation[0] = atof(json_tran->number);
              }
              
              json_arr_tran_ele = json_arr_tran_ele->next;

              if(json_arr_tran_ele)
              {
                const json_value_s *json_tran_value = json_arr_tran_ele->value;
                LIB_ASSERT(json_tran_value->type == json_type_number);
                
                const json_number_s *json_tran = (json_number_s*)json_tran_value->payload;
                node.translation[1] = atof(json_tran->number);
              }

              json_arr_tran_ele = json_arr_tran_ele->next;

              if(json_arr_tran_ele)
              {
                const json_value_s *json_tran_value = json_arr_tran_ele->value;
                LIB_ASSERT(json_tran_value->type == json_type_number);
                
                const json_number_s *json_tran = (json_number_s*)json_tran_value->payload;
                node.translation[2] = atof(json_tran->number);
              }
            }
            else if(strcmp(json_field_name->string, "scale") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_array);
              
              const json_array_s *json_arr_scale = (json_array_s*)json_field_value->payload;
              const json_array_element_s *json_arr_scale_ele = json_arr_scale->start;
              
              if(json_arr_scale_ele)
              {
                const json_value_s *json_scale_value = json_arr_scale_ele->value;
                LIB_ASSERT(json_scale_value->type == json_type_number);
                
                const json_number_s *json_scale = (json_number_s*)json_scale_value->payload;
                node.scale[0] = atof(json_scale->number);
              }
              
              json_arr_scale_ele = json_arr_scale_ele->next;

              if(json_arr_scale_ele)
              {
                const json_value_s *json_scale_value = json_arr_scale_ele->value;
                LIB_ASSERT(json_scale_value->type == json_type_number);
                
                const json_number_s *json_scale = (json_number_s*)json_scale_value->payload;
                node.scale[1] = atof(json_scale->number);
              }

              json_arr_scale_ele = json_arr_scale_ele->next;

              if(json_arr_scale_ele)
              {
                const json_value_s *json_scale_value = json_arr_scale_ele->value;
                LIB_ASSERT(json_scale_value->type == json_type_number);
                
                const json_number_s *json_scale = (json_number_s*)json_scale_value->payload;
                node.scale[2] = atof(json_scale->number);
              }
            }
            else
            {
              // Missing somethign
              LIB_ASSERT(false);
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
        
          gltf_scene scene{};
          
          const json_value_s *json_value              = json_scene->value;
          const json_object_s *json_field_obj         = (json_object_s*)json_value->payload;
          const json_object_element_s *json_field_ele = json_field_obj->start;
          
          while(json_field_ele != nullptr)
          {
            const json_string_s *json_field_name = json_field_ele->name;
            const json_value_s *json_field_value = json_field_ele->value;
            
            if(strcmp(json_field_name->string, "name") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_string);
            }
            else if(strcmp(json_field_name->string, "nodes") == 0)
            {
              LIB_ASSERT(json_field_value->type == json_type_array);
            }
            else if(strcmp(json_field_name->string, "extensions") == 0)
            {
              LOG_TODO_ONCE("Support scene extensions");
            }
            else
            {
              // Missing something.
              LIB_ASSERT(false);
            }
            
            // Next Element
            json_field_ele = json_field_ele->next;
          }
          
          // Next Field
          scenes.emplace_back(scene);
          json_scene = json_scene->next;
        }
      }
      
      json_gltf_element = json_gltf_element->next;
    } // while json_element_value

    const size_t accessor_count = accessors.size();
    const size_t asset_count = assets.size();
    const size_t buffer_view_count = buffer_views.size();
    const size_t node_size = nodes.size();
    const size_t meshes_size = meshes.size();
    const size_t scene_size = scenes.size();
    const size_t buffer_size = buffers.size();

    lib::array<uint32_t> internal_mesh_ids;

    // Load up Nil Resources //
    for(auto &mesh : meshes)
    {
      Nil::Resource::Mesh data{};
      
      if(!Nil::Resource::find_by_name(mesh.name, data))
      {
        data.name = mesh.name;
        
        // Position
        {
          const size_t buffer = buffer_views[mesh.primitives.attr_position].buffer;
          const size_t offset = buffer_views[mesh.primitives.attr_position].byte_offset;
          
          data.position_vec3 = (float*)&buffers[buffer].buffer[offset];
        }
        
        // Normal
        {
          const size_t buffer = buffer_views[mesh.primitives.attr_normal].buffer;
          const size_t offset = buffer_views[mesh.primitives.attr_normal].byte_offset;
          
          data.normal_vec3 = (float*)&buffers[buffer].buffer[offset];
        }
        
        // Texture Coords
        {
          const size_t buffer = buffer_views[mesh.primitives.attr_texcoord_0].buffer;
          const size_t offset = buffer_views[mesh.primitives.attr_texcoord_0].byte_offset;
        
          data.texture_coords_vec2 = (float*)&buffers[buffer].buffer[offset];
        }
        
        // Index
        {
          const size_t buffer = buffer_views[mesh.primitives.indices].buffer;
          const size_t offset = buffer_views[mesh.primitives.indices].byte_offset;
          const size_t count  = accessors[mesh.primitives.indices].count;
          
          // Currently only support uint32_t index.
          LIB_ASSERT(accessors[mesh.primitives.indices].component_type == 5125);
          
          //data.index       = (uint32_t*)&buffers[buffer].buffer[offset];
          //data.index_count = count;
        }
        
        data.triangle_count = accessors[mesh.primitives.attr_position].count;
        
        Nil::Resource::load(data);
        
        internal_mesh_ids.emplace_back(data.id);
      }
    }
    
    static Nil::Node root_node;
    
    for(auto &n : nodes)
    {
      Nil::Node node;
      
      Nil::Data::Transform trans;
      memcpy(trans.position, n.translation, sizeof(trans.position));
      memcpy(trans.rotation, n.rotation, sizeof(trans.rotation));
      memcpy(trans.scale, n.scale, sizeof(trans.scale));
      
      Nil::Data::set(node, trans);
      
      Nil::Data::Renderable renderable;
      renderable.mesh_id = internal_mesh_ids[n.mesh];
      renderable.material_id = 1;
      
      Nil::Data::set(node, renderable);
      
      node.set_parent(root_node);
    }

    free(json_root);
  }
}


} // ns
} // ns
