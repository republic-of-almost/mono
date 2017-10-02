#include <nil/resource/audio.hpp>
#include <internal_data/resources/audio_src.hpp>
#include <lib/array.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <lib/key.hpp>
#include <lib/string.hpp>
#include <common/common.hpp>


namespace {


// ------------------------------------------------------------ [ Resources ] --


Audio_data&
get_audio_data()
{
  static Audio_data aud_data;
  return aud_data;
}

	
// ------------------------------------------------------------- [ Messages ] --


constexpr char msg_audio_no_name[]      = "Loading an Audio file - must have a name";
constexpr char msg_audio_no_data[]      = "Audio file %s has no data";
constexpr char msg_audio_name_exists[]  = "Audio file with name %s already exists";
constexpr char msg_audio_failed[]       = "Failed to add Audio file %s";


// ---------------------------------------------------------- [ Identifiers ] --


constexpr char audio_type_name[] = "Audio";


} // anon ns


namespace Nil {
namespace Resource {


// ------------------------------------------------------------- [ Get Data ] --


bool
find_by_name(const char *name, Audio &out)
{
  const uint32_t find_key = lib::string_pool::find(name);
  
  for(size_t i = 0; i < get_audio_data().keys.size(); ++i)
  {
    if(get_audio_data().keys[i] == find_key)
    {
      out = get_audio_data().audio[i];
      return true;
    }
  }
  
  return false;
}


void
get(size_t *count, Audio **out)
{
  *count = get_audio_data().audio.size();
  *out = get_audio_data().audio.data();
}


// ----------------------------------------------------------------- [ Load ] --


bool
load(Audio &in_out)
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
      LOG_ERROR(msg_audio_no_name);
      return false;
    }
    
    if(!has_data || !has_data_len)
    {
      LOG_ERROR(msg_audio_no_data, in_out.name);
      return false;
    }
  }
  
  // -- Check if file is loaded -- //
  {
    const uint32_t check_key = lib::string_pool::find(in_out.name);

    if(check_key)
    {
      size_t index = 0;
    
      if (lib::key::linear_search(check_key, get_audio_data().keys.data(), get_audio_data().keys.size(), &index))
      {
        LOG_WARNING(msg_audio_name_exists, in_out.name);
        return false;
      }
    }
  }
  
  // -- Load Audio -- //
  Audio cpy = in_out;
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
        
          LOG_ERROR(msg_audio_failed, in_out.name);
        
          LIB_ASSERT(false);
          return false;
        }
      }
    }
  }
  
  // Generate new id //
  {
    const uint32_t new_id = get_audio_data().keys.size();
    in_out.id = new_id;
    cpy.id = in_out.id;
  }
  
  // -- Save new Mesh -- //
  {
    const uint32_t key = lib::string_pool::add(cpy.name);
    get_audio_data().keys.emplace_back(key);
    get_audio_data().audio.emplace_back(cpy);
  }
  
  return true;
}


// ----------------------------------------------------------------- [ Info ] --


const char *
get_type_name(const Audio &)
{
  return audio_type_name;
}


size_t
audio_count()
{
  return get_audio_data().keys.size();
}


} // ns
} // ns
