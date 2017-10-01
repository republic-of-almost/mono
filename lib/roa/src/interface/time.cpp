#include <roa/time.hpp>
#include <common/context.hpp>
#include <nil/nil.hpp>
#include <lib/timer.hpp>


namespace ROA {
namespace Time {


float
get_delta_time()
{
  Nil_ctx *ctx = ROA_detail::get_ctx();
  return nil_ctx_get_delta_time(ctx);
}


uint64_t
get_milliseconds()
{
  return static_cast<uint64_t>(lib::timer::get_current_time());
}


} // ns
} // ns
