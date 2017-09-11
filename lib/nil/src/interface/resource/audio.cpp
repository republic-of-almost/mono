#include <nil/resource/audio.hpp>
#include <lib/array.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <lib/key.hpp>
#include <lib/string.hpp>
#include <common/common.hpp>


namespace {


// ------------------------------------------------------------ [ Resources ] --


struct Audio_data {
  lib::array<uint32_t, 128>             keys{};
  lib::array<Nil::Resource::Audio, 128> audio{};
};


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


// -- [ Get Data ] --


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
    LOG_TODO_ONCE("Audio needs to hash names for lookup");
  }
  
  // -- Load Audio -- //
  Audio cpy = in_out;
  {
    // Transfer Ownership //
    /*
     
    */
    {
      bool failed = false;
      
      // Copy Name //
      char *cpy_name = nullptr;
      {
        if(!failed)
        {
          failed = !Nil_detail::copy_data_name(cpy_name, in_out.name, malloc);
        }
      }
      
      // Copy Data //
      void *cpy_data = nullptr;
      {
        if(!failed)
        {
          failed = !Nil_detail::copy_data(cpy_data, (void*)in_out.data, in_out.data_size, malloc);
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
    
      // Set Outputs //
      {
        // Generate New ID
        {
        
        }
      }
    }
  }
  
  
  return true;
}


// ----------------------------------------------------------------- [ Info ] --


const char *
get_type_name(const Audio &)
{
  return audio_type_name;
}


} // ns
} // ns
