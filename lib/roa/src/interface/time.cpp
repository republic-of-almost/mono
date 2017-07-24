#include <roa/time.hpp>
#include <common/common.hpp>
#include <nil/nil.hpp>
#include <lib/timer.hpp>


namespace ROA {
namespace Time {


float
get_delta_time()
{
  return ROA_detail::get_engine().get_delta_time();
}


uint64_t
get_milliseconds()
{
  return static_cast<uint64_t>(lib::timer::get_current_time());
}


} // ns
} // ns
