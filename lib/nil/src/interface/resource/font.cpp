#include <nil/resource/font.hpp>
#include <internal_data/internal_data.hpp>
#include <internal_data/resources/font_data.hpp>
#include <lib/array.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <lib/key.hpp>
#include <lib/string.hpp>
#include <common/common.hpp>


namespace {


// ------------------------------------------------------------ [ Resources ] --


Font_data&
get_font_data()
{
  static Font_data font_data;
  return font_data;
}


// ------------------------------------------------------------- [ Messages ] --


constexpr char msg_font_no_name[]       = "Loading a Font file - must have a name";
constexpr char msg_font_no_data[]       = "Font file %s has no data";
constexpr char msg_font_name_exists[]   = "Font file with name %s already exists";
constexpr char msg_font_failed[]        = "Failed to add Font file %s";


// ---------------------------------------------------------- [ Identifiers ] --


constexpr char font_type_name[] = "Font";


} // anon ns



namespace Nil {
namespace Resource {



// ------------------------------------------------------------- [ Get Data ] --


bool
find_by_name(const char *name, Font &out)
{
  return false;
}


void
get(size_t *count, Font **out)
{
  *count = get_font_data().font.size();
  *out = get_font_data().font.data();
}


// ----------------------------------------------------------------- [ Load ] --


bool
load(Font &in_out)
{
  // -- Param Check -- //
  {
    const bool has_name     = !!in_out.name;
    const bool has_length   = strlen(in_out.name);
    const bool has_data     = !!in_out.data;
    const bool has_data_len = !!in_out.data_size;
    
    LIB_ASSERT(has_name);
    LIB_ASSERT(has_length);
    LIB_ASSERT(has_data);
    
    if(!has_name || !has_length)
    {
      LOG_ERROR(msg_font_no_name);
      return false;
    }
    
    if(!has_data || !has_data_len)
    {
      LOG_ERROR(msg_font_no_data, in_out.name);
      return false;
    }
  }
  
  // -- Check if file is loaded -- //
  {
    const uint32_t check_key = lib::string_pool::find(in_out.name);

    if(check_key)
    {
      size_t index = 0;
      
      const uint32_t *data = get_font_data().keys.data();
      const size_t data_count = get_font_data().keys.size();
    
      if (lib::key::linear_search(check_key, data, data_count, &index))
      {
        LOG_WARNING(msg_font_name_exists, in_out.name);
        return false;
      }
    }
  }
  
  // -- Load Audio -- //
  Font cpy = in_out;
  {
    // Transfer Ownership //
    /*
      Do this first incase it fails we can cleanup.
    */
    {
      bool failed = false;
      
      // Copy Name //
      char *cpy_name = nullptr;
      {
        if(!failed)
        {
          failed = !Nil_detail::copy_data_name(&cpy_name, in_out.name, malloc);
        }
      }
      
      // Copy Data //
      void *cpy_data = nullptr;
      {
        if(!failed)
        {
          failed = !Nil_detail::copy_data(&cpy_data, (void*)in_out.data, in_out.data_size, malloc);
        }
      }
    
      // Set Data //
      {
        if(!failed)
        {
          cpy.name = cpy_name;
          cpy.data = (uintptr_t)cpy_data;
        }
        else
        {
          if(cpy_data) { free((void*)cpy_data); }
          if(cpy_name) { free(cpy_name);        }
        
          LOG_ERROR(msg_font_failed, in_out.name);
        
          LIB_ASSERT(false);
          return false;
        }
      }
    }
  }
  
  // Generate new id //
  {
    const uint32_t new_id = get_font_data().keys.size();
    in_out.id = new_id;
    cpy.id = in_out.id;
  }
  
  // -- Save new Mesh -- //
  {
    const uint32_t key = lib::string_pool::add(cpy.name);
    get_font_data().keys.emplace_back(key);
    get_font_data().font.emplace_back(cpy);
  }
  
  return true;
}


// ----------------------------------------------------------------- [ Info ] --
/*
  Various information about Font data.
*/


const char *
get_type_name(const Font &in)
{
  return font_type_name;
}


size_t
font_count()
{
  return get_font_data().keys.size();
}


} // ns
} // ns

