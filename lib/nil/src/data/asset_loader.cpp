#include <data/asset_loader.hpp>
#include <nil/resource/texture.hpp>
#include <nil/resource/mesh.hpp>
#include <nil/resource/material.hpp>
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
      uint8_t *uri;
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
      size_t size;
      uint8_t *uri;
    };
    
    struct gltf_KHR_materials_cmnBlinnPhong
    {
      float diffuse_factor[4];
      uint32_t diffuse_texture;
      float shininess_factor;
      float specular_factor[3];
    };
    
    struct gltf_material
    {
      char name[64];
      float emissive_factor[3];
      gltf_KHR_materials_cmnBlinnPhong blinn_extension;
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
    lib::array<gltf_material>     materials;

    // --

    constexpr uint32_t flags(
      json_parse_flags_default |
      json_parse_flags_allow_simplified_json | // bitsquid json
      json_parse_flags_allow_trailing_comma
    );

    json_value_s *json_root                        = json_parse_ex(buffer, count, flags, 0, 0, 0);
    
    
    auto json_obj_element = [](const json_value_s *obj)
    {
      return ((const json_object_s*)obj->payload)->start;
    };
    
    auto json_obj_name = [](const json_object_element_s *ele, const char *name)
    {
      return (strcmp(ele->name->string, name) == 0);
    };
    
    auto json_arr_element = [](const json_object_element_s *ele)
    {
      return ((const json_array_s*)ele->value->payload)->start;
    };
    
    auto json_to_int = [](const json_object_element_s *ele)
    {
      return atoi(((const json_number_s*)ele->value->payload)->number);
    };

    auto json_to_float = [](const json_value_s *val)
    {
      return atof(((const json_number_s*)val->payload)->number);
    };
    
    auto json_to_string = [](const json_object_element_s *ele, size_t *data_length = nullptr)
    {
      if(data_length)
      {
        *data_length = ((const json_string_s*)ele->value->payload)->string_size;
      }
    
      return ((json_string_s*)ele->value->payload)->string;
    };
    
    // -- //
    
    const json_object_element_s *json_gltf_element = json_obj_element(json_root);
    
    while(json_gltf_element != nullptr)
    {
      /*
        Parse Accessors
      */
      if(json_obj_name(json_gltf_element, "accessors"))
      {
        const json_array_element_s *json_accessor = json_arr_element(json_gltf_element);
        
        while(json_accessor != nullptr)
        {
          gltf_accessor accessor{};
          
          const json_object_element_s *json_accessor_attr = json_obj_element(json_accessor->value);
          
          while(json_accessor_attr != nullptr)
          {
            if(json_obj_name(json_accessor_attr, "bufferView"))
            {
              accessor.buffer_view = json_to_int(json_accessor_attr);
            }
            
            else if(json_obj_name(json_accessor_attr, "componentType"))
            {
              accessor.component_type = json_to_int(json_accessor_attr);
            }
            
            else if(json_obj_name(json_accessor_attr, "count"))
            {
              accessor.count = json_to_int(json_accessor_attr);
            }
            
            else if(json_obj_name(json_accessor_attr, "min"))
            {
            }
            
            else if(json_obj_name(json_accessor_attr, "max"))
            {
            }
            
            else if(json_obj_name(json_accessor_attr, "type"))
            {
              const char *name = json_to_string(json_accessor_attr);
              
              if(strcmp(name, "SCALAR") == 0)
              {
                accessor.type = gltf_accessor::SCALAR;
              }
              else if(strcmp(name, "VEC2") == 0)
              {
                accessor.type = gltf_accessor::VEC2;
              }
              else if(strcmp(name, "VEC3") == 0)
              {
                accessor.type = gltf_accessor::VEC3;
              }
              else if(strcmp(name, "VEC4") == 0)
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
            json_accessor_attr = json_accessor_attr->next;
          }
          
          // Next field
          accessors.emplace_back(accessor);
          json_accessor = json_accessor->next;
        }
      } // if accessors
      
      /*
        Parse Buffer Views
      */
      else if(json_obj_name(json_gltf_element, "bufferViews"))
      {
        const json_array_element_s *json_buffer_view = json_arr_element(json_gltf_element);
        
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
      else if(json_obj_name(json_gltf_element, "buffers"))
      {
        const json_array_element_s *json_buffer = json_arr_element(json_gltf_element);
        
        while(json_buffer != nullptr)
        {
          gltf_buffer buffer{};

          const json_object_element_s *json_buffer_attr = json_obj_element(json_buffer->value);
          
          while(json_buffer_attr != nullptr)
          {
            if(json_obj_name(json_buffer_attr, "byteLength"))
            {
              buffer.byte_length = json_to_int(json_buffer_attr);
            }
            else if(json_obj_name(json_buffer_attr, "uri"))
            {
              size_t data_length = 0;
              const char *data = json_to_string(json_buffer_attr, &data_length);
              
              const char *starts_with = "data:application/octet-stream;base64,";
              const char *ends_width = ".bin"; // impl
              
              size_t length;
              buffer.uri = (uint8_t*)base64_decode(&data[strlen(starts_with)], data_length - strlen(starts_with), &length);
            }
            else
            {
              // Missing something from spec.
              LIB_ASSERT(false);
            }

            // Next element
            json_buffer_attr = json_buffer_attr->next;
          }
          
          // Next Field
          buffers.emplace_back(buffer);
          json_buffer = json_buffer->next;
        }
      }
      
      /*
        Parse Images
      */
      else if(json_obj_name(json_gltf_element, "images"))
      {
        const json_array_element_s *json_image = json_arr_element(json_gltf_element);
        
        while(json_image != nullptr)
        {
          gltf_image img{};
          
          const json_object_element_s *json_image_attr = json_obj_element(json_image->value);
          
          while(json_image_attr != nullptr)
          {
            if(json_obj_name(json_image_attr, "uri"))
            {
              size_t data_length = 0;
              const char *data = json_to_string(json_image_attr, &data_length);
              
              const char *starts_with = "data:image/png;base64,";
              const char *ends_width  = ".bin"; // impl
              
              size_t length;
              img.uri = (uint8_t*)base64_decode(&data[strlen(starts_with)], data_length - strlen(starts_with), &length);
              img.size = length;
            }
            else
            {
              // Missing something from spec.
              LIB_ASSERT(false);
            }
            
            json_image_attr = json_image_attr->next;
          }
          
          images.emplace_back(img);
          json_image = json_image->next;
        }
      }
      
      /*
        Parse Textures
      */
      else if(json_obj_name(json_gltf_element, "textures"))
      {
        const json_array_element_s *json_texture = json_arr_element(json_gltf_element);
        
        while(json_texture != nullptr)
        {
          gltf_texture texture{};
          
          const json_object_element_s *json_texture_attr = json_obj_element(json_texture->value);
          
          while(json_texture_attr != nullptr)
          {
            if(json_obj_name(json_texture_attr, "sampler"))
            {
              texture.sampler = json_to_int(json_texture_attr);
            }
            else if(json_obj_name(json_texture_attr, "source"))
            {
              texture.source = json_to_int(json_texture_attr);
            }
            
            json_texture_attr = json_texture_attr->next;
          }

          textures.emplace_back(texture);        
          json_texture = json_texture->next;
        }
      }
      
      /*
        Parse Materials
      */
      else if(json_obj_name(json_gltf_element, "materials"))
      {
        const json_array_element_s *json_material = json_arr_element(json_gltf_element);
        
        while(json_material != nullptr)
        {
          gltf_material mat{};
          
          const json_object_element_s *json_material_attr = json_obj_element(json_material->value);
          
          while(json_material_attr != nullptr)
          {
            if(json_obj_name(json_material_attr, "name"))
            {
              size_t data_length = 0;
              const char *data = json_to_string(json_material_attr, &data_length);
              
              const size_t len = data_length > 64 ? 63 : data_length;
              memcpy(mat.name, data, len);
            }
            else if(json_obj_name(json_material_attr, "emissiveFactor"))
            {
              LOG_TODO_ONCE("emissiveFactor not supported in material");
            }
            else if(json_obj_name(json_material_attr, "pbrMetallicRoughness"))
            {
              LOG_TODO_ONCE("pbrMetallicRoughness not supported in material");
            }
            else if(json_obj_name(json_material_attr, "metallicFactor"))
            {
              LOG_TODO_ONCE("metallicFactor not supported in material");
            }
            else if(json_obj_name(json_material_attr, "extensions"))
            {

              //"KHR_materials_cmnBlinnPhong"
            }
            else
            {
              // Something missing from the parser
              LIB_ASSERT(false);
            }
          
            json_material_attr = json_material_attr->next;
          }
          
          materials.emplace_back(mat);
          json_material = json_material->next;
        }
      }
      
      /*
        Parse Meshes
      */
      else if(json_obj_name(json_gltf_element, "meshes"))
      {
        const json_array_element_s *json_mesh = json_arr_element(json_gltf_element);
        
        while(json_mesh != nullptr)
        {
          gltf_mesh mesh{};
          
          const json_object_element_s *json_mesh_attr = json_obj_element(json_mesh->value);
          
          while(json_mesh_attr != nullptr)
          {
            if(json_obj_name(json_mesh_attr, "name"))
            {
              size_t data_length = 0;
              const char *data = json_to_string(json_mesh_attr, &data_length);
            
              const size_t len = data_length > 64 ? 63 : data_length;
              memcpy(mesh.name, data, len);
            }
            else if(json_obj_name(json_mesh_attr, "primitives"))
            {
              const json_array_element_s *json_prim_ele = json_arr_element(json_mesh_attr);
              
              while(json_prim_ele != nullptr)
              {
                const json_object_element_s *json_prim_attr = json_obj_element(json_prim_ele->value);
                
                while(json_prim_attr != nullptr)
                {
                  if(json_obj_name(json_prim_attr, "attributes"))
                  {
                    const json_object_element_s *atrib_attr = json_obj_element(json_prim_attr->value);
                    
                    while(atrib_attr != nullptr)
                    {
                      if(json_obj_name(atrib_attr, "POSITION"))
                      {
                        mesh.primitives.attr_position = json_to_int(atrib_attr);
                      }
                      else if(json_obj_name(atrib_attr, "NORMAL"))
                      {
                        mesh.primitives.attr_normal = json_to_int(atrib_attr);
                      }
                      else if(json_obj_name(atrib_attr, "TANGENT"))
                      {
                        mesh.primitives.attr_tangent = json_to_int(atrib_attr);
                      }
                      else if(json_obj_name(atrib_attr, "TEXCOORD_0"))
                      {
                        mesh.primitives.attr_texcoord_0 = json_to_int(atrib_attr);
                      }
                      
                      atrib_attr = atrib_attr->next;
                    }
                  }
                  else if(json_obj_name(json_prim_attr, "indices"))
                  {
                    mesh.primitives.indices = json_to_int(json_prim_attr);

                  }
                  else if(json_obj_name(json_prim_attr, "material"))
                  {
                    mesh.primitives.material = json_to_int(json_prim_attr);
                  }
                  else
                  {
                    // Missing something.
                    LIB_ASSERT(false);
                  }
                  
                  json_prim_attr = json_prim_attr->next;
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
            json_mesh_attr = json_mesh_attr->next;
          }
          
          // Next Field
          meshes.emplace_back(mesh);
          json_mesh = json_mesh->next;
        }
      }
      
      /*
        Parse Nodes
      */
      else if(json_obj_name(json_gltf_element, "nodes"))
      {
        const json_array_element_s *json_node = json_arr_element(json_gltf_element);
        
        while(json_node != nullptr)
        {
          gltf_node node{};
          node.scale[0] = 1.f; node.scale[1] = 1.f; node.scale[2] = 1.f;
          node.rotation[0] = 0.f; node.rotation[3] = 0.f; node.rotation[3] = 0.f; node.rotation[3] = 1.f;
          
          const json_object_element_s *json_node_attr = json_obj_element(json_node->value);
          
          while(json_node_attr != nullptr)
          {
            if(json_obj_name(json_node_attr, "mesh"))
            {
              node.mesh = json_to_int(json_node_attr);
            }
            else if(json_obj_name(json_node_attr, "name"))
            {
              size_t data_length = 0;
              const char *data = json_to_string(json_node_attr, &data_length);
              
              const size_t len = data_length > 63 ? 63 : data_length;
              memcpy(node.name, data, len);
            }
            else if(json_obj_name(json_node_attr, "rotation"))
            {
              const json_array_element_s *json_arr_rot_ele = json_arr_element(json_node_attr);
              
              if(json_arr_rot_ele)
              {
                node.rotation[0] = json_to_float(json_arr_rot_ele->value);
              }
              
              json_arr_rot_ele = json_arr_rot_ele->next;

              if(json_arr_rot_ele)
              {
                node.rotation[1] = json_to_float(json_arr_rot_ele->value);
              }

              json_arr_rot_ele = json_arr_rot_ele->next;

              if(json_arr_rot_ele)
              {
                node.rotation[2] = json_to_float(json_arr_rot_ele->value);
              }
              
              json_arr_rot_ele = json_arr_rot_ele->next;

              if(json_arr_rot_ele)
              {
                node.rotation[3] = json_to_float(json_arr_rot_ele->value);
              }
            }
            else if(json_obj_name(json_node_attr, "translation"))
            {
              const json_array_element_s *json_arr_tran_ele = json_arr_element(json_node_attr);
              
              if(json_arr_tran_ele)
              {
                node.translation[0] = json_to_float(json_arr_tran_ele->value);
              }
              
              json_arr_tran_ele = json_arr_tran_ele->next;

              if(json_arr_tran_ele)
              {
                node.translation[1] = json_to_float(json_arr_tran_ele->value);
              }

              json_arr_tran_ele = json_arr_tran_ele->next;

              if(json_arr_tran_ele)
              {
                node.translation[2] = json_to_float(json_arr_tran_ele->value);
              }
            }
            else if(json_obj_name(json_node_attr, "scale"))
            {
              const json_array_element_s *json_arr_scale_ele = json_arr_element(json_node_attr);
              
              if(json_arr_scale_ele)
              {
                node.scale[0] = json_to_float(json_arr_scale_ele->value);
              }
              
              json_arr_scale_ele = json_arr_scale_ele->next;

              if(json_arr_scale_ele)
              {
                node.scale[1] = json_to_float(json_arr_scale_ele->value);
              }

              json_arr_scale_ele = json_arr_scale_ele->next;

              if(json_arr_scale_ele)
              {
                node.scale[2] = json_to_float(json_arr_scale_ele->value);
              }
            }
            else
            {
              // Missing somethign
              LIB_ASSERT(false);
            }
            
            // Next Element
            json_node_attr = json_node_attr->next;
          }
          
          // Next Field
          nodes.emplace_back(node);
          json_node = json_node->next;
        }
      }
      
      /*
        Parse Scene
      */
      else if(json_obj_name(json_gltf_element, "scene"))
      {
        // Just one scene it seems.
      }
      
      /*
        Parse Scenes
      */
      else if(json_obj_name(json_gltf_element, "scenes"))
      {
        const json_array_element_s *json_scene = json_arr_element(json_gltf_element);
        
        while(json_scene != nullptr)
        {
          gltf_scene scene{};

          const json_object_element_s *json_scene_attr = json_obj_element(json_scene->value);
          
          while(json_scene_attr != nullptr)
          {
            if(json_obj_name(json_scene_attr, "name"))
            {
            }
            else if(json_obj_name(json_scene_attr, "nodes"))
            {
            }
            else if(json_obj_name(json_scene_attr, "extensions"))
            {
              LOG_TODO_ONCE("Support scene extensions");
            }
            else
            {
              // Missing something.
              LIB_ASSERT(false);
            }
            
            // Next Element
            json_scene_attr = json_scene_attr->next;
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
    lib::array<uint32_t> internal_texture_ids;
    lib::array<uint32_t> internal_material_ids;

    for(auto &tex : textures)
    {
      Nil::Resource::Texture data{};
      data.data_type = Nil::Resource::Texture::DATA;
      data.data = (uintptr_t)images[tex.source].uri;
      data.data_size = images[tex.source].size;
      
      static int i = 0;
      ++i;
      
      char import_name[128]{};
      sprintf(import_name, "GLTF Tex %d", i);
      
      data.name = import_name;
      
      Nil::Resource::load(data);
      
      internal_texture_ids.emplace_back(data.id);
    }
    
    for(auto &mat : materials)
    {
      Nil::Resource::Material data{};
      
      data.name = mat.name;
      
      data.texture_01 = 1;
      
      Nil::Resource::load(data);
      
      internal_material_ids.emplace_back(data.id);
    }

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
          
          data.position_vec3 = (float*)&buffers[buffer].uri[offset];
        }
        
        // Normal
        {
          const size_t buffer = buffer_views[mesh.primitives.attr_normal].buffer;
          const size_t offset = buffer_views[mesh.primitives.attr_normal].byte_offset;
          
          data.normal_vec3 = (float*)&buffers[buffer].uri[offset];
        }
        
        // Texture Coords
        {
          const size_t buffer = buffer_views[mesh.primitives.attr_texcoord_0].buffer;
          const size_t offset = buffer_views[mesh.primitives.attr_texcoord_0].byte_offset;
        
          data.texture_coords_vec2 = (float*)&buffers[buffer].uri[offset];
        }
        
        // Index
        {
          const size_t buffer = buffer_views[mesh.primitives.indices].buffer;
          const size_t offset = buffer_views[mesh.primitives.indices].byte_offset;
          const size_t count  = accessors[mesh.primitives.indices].count;
          
          // Currently only support uint32_t index.
          LIB_ASSERT(accessors[mesh.primitives.indices].component_type == 5125);
          
          data.index       = (uint32_t*)&buffers[buffer].uri[offset];
          data.index_count = count;
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
      node.set_name(n.name);
      
      Nil::Data::Transform trans;
      memcpy(trans.position, n.translation, sizeof(trans.position));
      memcpy(trans.rotation, n.rotation, sizeof(trans.rotation));
      memcpy(trans.scale, n.scale, sizeof(trans.scale));
      
      Nil::Data::set(node, trans);
      
      Nil::Data::Renderable renderable;
      renderable.mesh_id = internal_mesh_ids[n.mesh];
      renderable.material_id = internal_material_ids[meshes[n.mesh].primitives.material];
      
      Nil::Data::set(node, renderable);
      
      node.set_parent(root_node);
    }

    free(json_root);
  }
}


} // ns
} // ns
