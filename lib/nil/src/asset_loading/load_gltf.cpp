#include <asset_loading/asset_loading.hpp>
#include <nil/resource/mesh.hpp>
#include <nil/resource/texture.hpp>
#include <nil/resource/directory.hpp>
#include <nil/resource/material.hpp>
#include <nil/data/renderable.hpp>
#include <nil/data/transform.hpp>
#include <nil/data/light.hpp>
#include <lib/string.hpp>
#include <lib/file.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <lib/array.hpp>
#include <lib/color.hpp>
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


/*
  Json Helpers
*/
inline const json_object_element_s*
json_obj_element(const json_value_s *obj)
{
  return ((const json_object_s*)obj->payload)->start;
}

inline bool
json_obj_name(const json_object_element_s *ele, const char *name)
{
  return (strcmp(ele->name->string, name) == 0);
}

inline json_array_element_s*
json_arr_element(const json_object_element_s *ele)
{
  return ((const json_array_s*)ele->value->payload)->start;
}

inline int32_t
json_to_int(const json_value_s *val)
{
  LIB_ASSERT(val->type == json_type_number);
  return (int32_t)atoi(((const json_number_s*)val->payload)->number);
}

inline float
json_to_float(const json_value_s *val)
{
  return atof(((const json_number_s*)val->payload)->number);
}

inline const char *
json_to_string(const json_value_s *val, size_t *data_length = nullptr)
{
  if(data_length)
  {
    *data_length = ((const json_string_s*)val->payload)->string_size;
  }

  return ((json_string_s*)val->payload)->string;
}


} // anon ns



namespace Nil {
namespace Assets {


bool
load_gltf(Nil::Node root_node, const char *path)
{
//  char path[2048]{};
//  strcat(path, lib::dir::exe_path());
//
//  #ifndef LIB_PLATFORM_MAC
//  strcat(path, "assets/mesh/");
//  #else
//  strcat(path, "../Resources/assets/mesh/");
//  #endif
//
//  strcat(path, "test_level.gltf");

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
    int32_t mesh;
    int32_t light;
    
    char name[64];
    
    float translation[3];
    float scale[3];
    float rotation[4];
    
    uint32_t children[128];
    uint32_t child_size;
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
    int32_t diffuse_texture;
    float shininess_factor;
    float specular_factor[3];
  };
  
  struct gltf_material
  {
    char name[64];
    float emissive_factor[3];
    gltf_KHR_materials_cmnBlinnPhong ext_blinn_phong;
  };

  
  struct gltf_sampler
  {
    
  };
  
  struct gltf_texture
  {
    uint32_t sampler;
    uint32_t source;
  };
  
  // -- Extensions -- //
  
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
  
  lib::array<Nil::Data::Light>  lights;

  // --

  constexpr uint32_t flags(
    json_parse_flags_default |
    json_parse_flags_allow_simplified_json | // bitsquid json
    json_parse_flags_allow_trailing_comma
  );

  json_value_s *json_root                        = json_parse_ex(buffer, count, flags, 0, 0, 0);

  
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
            accessor.buffer_view = json_to_int(json_accessor_attr->value);
          }
          
          else if(json_obj_name(json_accessor_attr, "componentType"))
          {
            accessor.component_type = json_to_int(json_accessor_attr->value);
          }
          
          else if(json_obj_name(json_accessor_attr, "count"))
          {
            accessor.count = json_to_int(json_accessor_attr->value);
          }
          
          else if(json_obj_name(json_accessor_attr, "min"))
          {
          }
          
          else if(json_obj_name(json_accessor_attr, "max"))
          {
          }
          
          else if(json_obj_name(json_accessor_attr, "type"))
          {
            const char *name = json_to_string(json_accessor_attr->value);
            
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
            buffer.byte_length = json_to_int(json_buffer_attr->value);
          }
          else if(json_obj_name(json_buffer_attr, "uri"))
          {
            size_t data_length = 0;
            const char *data = json_to_string(json_buffer_attr->value, &data_length);
            
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
            const char *data = json_to_string(json_image_attr->value, &data_length);
            
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
            texture.sampler = json_to_int(json_texture_attr->value);
          }
          else if(json_obj_name(json_texture_attr, "source"))
          {
            texture.source = json_to_int(json_texture_attr->value);
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
        mat.ext_blinn_phong.diffuse_texture = -1;
        
        const json_object_element_s *json_material_attr = json_obj_element(json_material->value);
        
        while(json_material_attr != nullptr)
        {
          if(json_obj_name(json_material_attr, "name"))
          {
            size_t data_length = 0;
            const char *data = json_to_string(json_material_attr->value, &data_length);
            
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
            const json_object_element_s *json_material_ext_attr = json_obj_element(json_material_attr->value);
            
            if(json_obj_name(json_material_ext_attr, "KHR_materials_cmnBlinnPhong"))
            {
              const json_object_element_s *blinn_phong_attr = json_obj_element(json_material_ext_attr->value);
              
              while(blinn_phong_attr != nullptr)
              {
                if(json_obj_name(blinn_phong_attr, "diffuseFactor"))
                {
                  const json_array_element_s *factor_arr = json_arr_element(blinn_phong_attr);
                  
                  size_t index = 0;
                  
                  while(factor_arr != nullptr)
                  {
                    LIB_ASSERT(index < 4);
                    mat.ext_blinn_phong.diffuse_factor[index++] = json_to_float(factor_arr->value);
                  
                    factor_arr = factor_arr->next;
                  }
                }
                
                else if(json_obj_name(blinn_phong_attr, "diffuseTexture"))
                {
                  const json_object_element_s *diff_tex_attr = json_obj_element(blinn_phong_attr->value);
                  
                  while(diff_tex_attr != nullptr)
                  {
                    if(json_obj_name(diff_tex_attr, "index"))
                    {
                      mat.ext_blinn_phong.diffuse_texture = json_to_int(diff_tex_attr->value);
                    }
                    else
                    {
                      LIB_ASSERT("Unknown ");
                    }
                    
                    diff_tex_attr = diff_tex_attr->next;
                  }
                }
                
                else if(json_obj_name(blinn_phong_attr, "shininessFactor"))
                {
                  mat.ext_blinn_phong.shininess_factor = json_to_float(blinn_phong_attr->value);
                }
                
                else if(json_obj_name(blinn_phong_attr, "specularFactor"))
                {
                  const json_array_element_s *spec_arr = json_arr_element(blinn_phong_attr);
                  
                  size_t index = 0;
                  
                  while(spec_arr != nullptr)
                  {
                    LIB_ASSERT(index < 3);
                    mat.ext_blinn_phong.specular_factor[index++] = json_to_float(spec_arr->value);
                  
                    spec_arr = spec_arr->next;
                  }
                }
               
                blinn_phong_attr = blinn_phong_attr->next;
              }

            }
            else
            {
              // Missing something.
              LIB_ASSERT(false);
            }
            
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
        memset(&mesh.primitives, -1, sizeof(mesh.primitives));
        
        const json_object_element_s *json_mesh_attr = json_obj_element(json_mesh->value);
        
        while(json_mesh_attr != nullptr)
        {
          if(json_obj_name(json_mesh_attr, "name"))
          {
            size_t data_length = 0;
            const char *data = json_to_string(json_mesh_attr->value, &data_length);
          
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
                      mesh.primitives.attr_position = json_to_int(atrib_attr->value);
                    }
                    else if(json_obj_name(atrib_attr, "NORMAL"))
                    {
                      mesh.primitives.attr_normal = json_to_int(atrib_attr->value);
                    }
                    else if(json_obj_name(atrib_attr, "TANGENT"))
                    {
                      mesh.primitives.attr_tangent = json_to_int(atrib_attr->value);
                    }
                    else if(json_obj_name(atrib_attr, "TEXCOORD_0"))
                    {
                      mesh.primitives.attr_texcoord_0 = json_to_int(atrib_attr->value);
                    }
                    
                    atrib_attr = atrib_attr->next;
                  }
                }
                else if(json_obj_name(json_prim_attr, "indices"))
                {
                  mesh.primitives.indices = json_to_int(json_prim_attr->value);

                }
                else if(json_obj_name(json_prim_attr, "material"))
                {
                  mesh.primitives.material = json_to_int(json_prim_attr->value);
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
        node.mesh = -1;
        node.light = -1;
        node.scale[0] = 1.f; node.scale[1] = 1.f; node.scale[2] = 1.f;
        node.rotation[0] = 0.f; node.rotation[3] = 0.f; node.rotation[3] = 0.f; node.rotation[3] = 1.f;
        
        const json_object_element_s *json_node_attr = json_obj_element(json_node->value);
        
        while(json_node_attr != nullptr)
        {
          if(json_obj_name(json_node_attr, "mesh"))
          {
            node.mesh = json_to_int(json_node_attr->value);
          }
          else if(json_obj_name(json_node_attr, "name"))
          {
            size_t data_length = 0;
            const char *data = json_to_string(json_node_attr->value, &data_length);
            
            const size_t len = data_length > 63 ? 63 : data_length;
            memcpy(node.name, data, len);
          }
          else if(json_obj_name(json_node_attr, "rotation"))
          {
            const json_array_element_s *json_arr_rot_ele = json_arr_element(json_node_attr);
            
            size_t index = 0;
            
            while(json_arr_rot_ele != nullptr)
            {
              LIB_ASSERT(index < 4);
              node.rotation[index++] = json_to_float(json_arr_rot_ele->value);
            
              json_arr_rot_ele = json_arr_rot_ele->next;
            }
          }
          else if(json_obj_name(json_node_attr, "translation"))
          {
            const json_array_element_s *json_arr_tran_ele = json_arr_element(json_node_attr);
            
            size_t index = 0;
            
            while(json_arr_tran_ele != nullptr)
            {
              LIB_ASSERT(index < 3);
              node.translation[index++] = json_to_float(json_arr_tran_ele->value);
              
              json_arr_tran_ele = json_arr_tran_ele->next;
            }
          }
          else if(json_obj_name(json_node_attr, "scale"))
          {
            const json_array_element_s *json_arr_scale_ele = json_arr_element(json_node_attr);
            
            size_t index = 0;
            
            while(json_arr_scale_ele != nullptr)
            {
              LIB_ASSERT(index < 3);
              node.scale[index++] = json_to_float(json_arr_scale_ele->value);
              
              json_arr_scale_ele = json_arr_scale_ele->next;
            }
          }
          else if(json_obj_name(json_node_attr, "children"))
          {
            const json_array_element_s *json_child_ele = json_arr_element(json_node_attr);
            
            while(json_child_ele != nullptr)
            {
              LIB_ASSERT(node.child_size < 128);
              node.children[node.child_size++] = json_to_int(json_child_ele->value);
              
              json_child_ele = json_child_ele->next;
            }
          }
          else if(json_obj_name(json_node_attr, "extras"))
          {
            const json_object_element_s *json_extras_attr = json_obj_element(json_node_attr->value);
            
            while(json_extras_attr != nullptr)
            {
              if(json_obj_name(json_extras_attr, "tags"))
              {
                
              }
              
              json_extras_attr = json_extras_attr->next;
            }
          }
          else if(json_obj_name(json_node_attr, "extensions"))
          {
            const json_object_element_s *json_ext_attr = json_obj_element(json_node_attr->value);
            
            while(json_ext_attr != nullptr)
            {
              if(json_obj_name(json_ext_attr, "KHR_lights_cmn"))
              {
                const json_object_element_s *json_lights_ext = json_obj_element(json_ext_attr->value);
                
                while(json_lights_ext != nullptr)
                {
                  if(json_obj_name(json_lights_ext, "light"))
                  {
                    node.light = json_to_int(json_lights_ext->value);
                  }
                  else
                  {
                    // missing something.
                    LIB_ASSERT(false);
                  }
                
                  json_lights_ext = json_lights_ext->next;
                }
              }
            
              json_ext_attr = json_ext_attr->next;
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
      Extensions
    */
    else if(json_obj_name(json_gltf_element, "extensions"))
    {
      const json_object_element_s *json_ext_attr = json_obj_element(json_gltf_element->value);
      
      if(json_obj_name(json_ext_attr, "KHR_lights_cmn"))
      {
        const json_object_element_s *json_ext_lights_attr = json_obj_element(json_ext_attr->value);
        
        while(json_ext_lights_attr != nullptr)
        {
          if(json_obj_name(json_ext_lights_attr, "lights"))
          {
            Nil::Data::Light data{};
          
            const json_array_element_s *json_light_ele = json_arr_element(json_ext_lights_attr);
           
            while(json_light_ele != nullptr)
            {
              const json_object_element_s *json_ext_light_attr = json_obj_element(json_light_ele->value);
              
              while(json_ext_light_attr != nullptr)
              {
                if(json_obj_name(json_ext_light_attr, "color"))
                {
                  const json_array_element_s *json_arr_color = json_arr_element(json_ext_light_attr);
              
                  size_t index = 0;
                  float color[4]{1,1,1,1};
                  
                  while(json_arr_color != nullptr)
                  {
                    LIB_ASSERT(index < 3);
                    color[index++] = json_to_float(json_arr_color->value);
                    
                    json_arr_color = json_arr_color->next;
                  }
                  
                  lib::rgba rgba = lib::color::init(color);
                  
                  data.color[0] = lib::color::get_channel_1i(rgba);
                  data.color[1] = lib::color::get_channel_2i(rgba);
                  data.color[2] = lib::color::get_channel_3i(rgba);
                }
                else if(json_obj_name(json_ext_light_attr, "name"))
                {
                }
                else if(json_obj_name(json_ext_light_attr, "positional"))
                {
                  const json_object_element_s *json_pos_attr = json_obj_element(json_ext_light_attr->value);
                  
                  while(json_pos_attr != nullptr)
                  {
                    if(json_obj_name(json_pos_attr, "constantAttenuation"))
                    {
                      data.atten_const = json_to_float(json_pos_attr->value);
                    }
                    else if(json_obj_name(json_pos_attr, "linearAttenuation"))
                    {
                      data.atten_linear = json_to_float(json_pos_attr->value);
                    }
                    else if(json_obj_name(json_pos_attr, "quadraticAttenuation"))
                    {
                      data.atten_exponential = json_to_float(json_pos_attr->value);
                    }
                  
                    json_pos_attr = json_pos_attr->next;
                  }
                }
                else if(json_obj_name(json_ext_light_attr, "type"))
                {
                  const char *type = json_to_string(json_ext_light_attr->value);
                  
                  if(strcmp("point", type) == 0)
                  {
                    data.type = Nil::Data::Light::POINT;
                  }
                  else if(strcmp("directional", type) == 0)
                  {
                    data.type = Nil::Data::Light::DIR;
                  }
                  else if(strcmp("spot", type) == 0)
                  {
                    data.type = Nil::Data::Light::SPOT;
                  }
                }
                else if(json_obj_name(json_ext_light_attr, "profile"))
                {
                
                }
                else
                {
                  LIB_ASSERT(false); // missing something.
                }
                
                json_ext_light_attr = json_ext_light_attr->next;
              }
              
              lights.emplace_back(data);
              json_light_ele = json_light_ele->next;
            }
            
          }
          
          json_ext_lights_attr = json_ext_lights_attr->next;
        }
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
          else if(json_obj_name(json_scene_attr, "extras"))
          {
            LOG_TODO_ONCE("Support scene extras");
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

  lib::array<Nil::Resource::Mesh> internal_meshes;
  lib::array<Nil::Resource::Texture> internal_textures;
  lib::array<Nil::Resource::Material> internal_materials;

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
    
    internal_textures.emplace_back(data);
  }
  
  for(auto &mat : materials)
  {
    Nil::Resource::Material data{};
    
    data.name = mat.name;
    
    if(mat.ext_blinn_phong.diffuse_texture >= 0)
    {
      data.texture_01 = internal_textures[mat.ext_blinn_phong.diffuse_texture].id;
    }
    
    Nil::Resource::load(data);
    
    internal_materials.emplace_back(data);
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
        const size_t index = mesh.primitives.attr_position;
      
        if(index != -1)
        {
          const size_t buffer = buffer_views[index].buffer;
          const size_t offset = buffer_views[index].byte_offset;
          
          data.position_vec3 = (float*)&buffers[buffer].uri[offset];
        }
      }
      
      // Normal
      {
        const size_t index = mesh.primitives.attr_normal;
        
        if(index != -1)
        {
          const size_t buffer = buffer_views[index].buffer;
          const size_t offset = buffer_views[index].byte_offset;
          
          data.normal_vec3 = (float*)&buffers[buffer].uri[offset];
        }
      }
      
      // Texture Coords
      {
        const size_t index = mesh.primitives.attr_texcoord_0;
      
        if(index != -1)
        {
          const size_t buffer = buffer_views[index].buffer;
          const size_t offset = buffer_views[index].byte_offset;
        
          data.texture_coords_vec2 = (float*)&buffers[buffer].uri[offset];
        }
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
      
      internal_meshes.emplace_back(data);
    }
  }
  
  // -- Load scene if we have a node -- //
 
  if(root_node)
  {
    std::vector<Nil::Node> nil_nodes;
    nil_nodes.resize(nodes.size());
    
    for(auto &n : nil_nodes)
    {
      n.set_parent(root_node);
    }
  
    for(uint32_t i = 0; i < nil_nodes.size(); ++i)
    {
      Nil::Node &node = nil_nodes[i];
      node.set_name(nodes[i].name);
      
      Nil::Data::Transform trans;
      memcpy(trans.position, nodes[i].translation, sizeof(trans.position));
      memcpy(trans.rotation, nodes[i].rotation, sizeof(trans.rotation));
      memcpy(trans.scale, nodes[i].scale, sizeof(trans.scale));
      
      Nil::Data::set(node, trans);
      
      const size_t mesh_index = nodes[i].mesh;
      
      if(mesh_index != -1)
      {
        Nil::Data::Renderable renderable{};
        
        renderable.mesh_id = internal_meshes[mesh_index].id;
        
        const size_t mat_id = meshes[mesh_index].primitives.material;
        
        if(mat_id != -1)
        {
          renderable.material_id = internal_materials[mat_id].id;
        }
        
        Nil::Data::set(node, renderable);
        Nil::Data::set(node, internal_meshes[nodes[i].mesh].bounding_box);
      }
      
      if(nodes[i].light != -1)
      {
        const size_t light_index = nodes[i].light;
        const Nil::Data::Light light_data = lights[light_index];
      
        Nil::Data::set(node, light_data);
      }
      
      if(nodes[i].child_size > 0)
      {
        for(uint32_t c = 0; c < nodes[i].child_size; ++c)
        {
          const size_t index = nodes[i].children[c];
          nil_nodes[index].set_parent(node);
        }
      }
    }
  }

  free(json_root);
  
  return true;
}


} // ns
} // ns
