#include <common/context.hpp>
#include <nil/nil.hpp>


namespace {


Nil_ctx *nil_ctx = nullptr;


};


namespace ROA_detail {


Nil_ctx*
get_ctx()
{
  static int init = nil_ctx_initialize(&nil_ctx);
  return nil_ctx;
}


} // ns
