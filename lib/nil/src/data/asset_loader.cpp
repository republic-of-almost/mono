#include <data/asset_loader.hpp>
#include <nil/resource/texture.hpp>
#include <nil/resource/mesh.hpp>
#include <tinydir/tinydir.h>
#include <lib/platform.hpp>
#include <lib/assert.hpp>
#include <lib/model.hpp>
#include <lib/directory.hpp>
#include <lib/string.hpp>


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
        
          lib::model m = lib::model_import::load_obj_from_file(file.path);
          
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
