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

    struct gltfBuffer
    {
      uint32_t view;
      uint32_t component_type;
      uint32_t size;
      uint32_t type;
    };

    lib::array<int> buffers;

    // --

    constexpr uint32_t flags(
      json_parse_flags_default |
      json_parse_flags_allow_simplified_json |
      json_parse_flags_allow_trailing_comma
    );

    json_value_s *gltf_root = json_parse_ex(buffer, count, flags, 0, 0, 0);
    json_object_s *gltf_root_obj = (json_object_s*)gltf_root->payload;

    json_object_element_s *gltf_ele = gltf_root_obj->start;

    json_string_s *gltf_ele_name = gltf_ele->name;
    json_value_s *gltf_ele_value = gltf_ele->value;

    json_array_s *accessors = (json_array_s*)gltf_ele_value->payload;
    json_array_element_s *gltf_buf = accessors->start;

    for(size_t i = 0; i < accessors->length; ++i)
    {
       json_value_s *val = gltf_buf->value;

       json_string_s *str = (json_string_s*)val->payload;

       int j = 0;
       
       gltf_buf->next;
    }

    json_object_s *obj2 = (json_object_s*)gltf_ele_value->payload;

    json_object_element_s *ele2 = obj2->start;

    json_string_s *n2 = ele2->name;
    json_value_s *v2 = ele2->value;


    int foo = 0;

    free(gltf_root);
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
