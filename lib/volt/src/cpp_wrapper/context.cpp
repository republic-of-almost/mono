#include <volt/volt.hpp>
#include <volt/volt.h>


namespace volt {


struct context::impl
{
  volt_ctx *ctx;
};


context::context()
: m_impl{new impl{}}
{

}
  

context::~context()
{

}


context::context(context &&) noexcept
{

}


context&
context::operator=(const context &&) noexcept
{
  return *this;
}


void
context::execute()
{

}


} /* ns */
