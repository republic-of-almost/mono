#include <roa/audio_source.hpp>
#include <nil/resource/audio.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <lib/string.hpp>


namespace {


constexpr char msg_roa_audio_failed[] = "Failed load audio";


} // ns


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Audio_source::Audio_source()
{
}



//Audio_source::Audio_source(const char *filename)
//: Resource()
//{
//  // -- Param Check -- //
//  const bool has_filename = !!filename;
//  const bool has_length = strlen(filename);
//
//  if(has_filename && has_length)
//  {
//    Nil::Resource::Audio audio{};
//    
//    char name[1024]{};
//    
//    lib::string::filename_from_path(filename, name);
//    audio.name = name;
//    audio.data = (uintptr_t)filename;
//    audio.data_size = strlen(filename) + 1;
//    audio.data_type = Nil::Resource::Audio::FILENAME;
//    Nil::Resource::load(audio);
//    
//    m_id = audio.id;
//  }
//  else
//  {
//    LOG_ERROR(msg_roa_audio_failed);
//  }
//}


} // ns
