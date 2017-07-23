#ifndef ENVIRONMENT_INCLUDED_A4891E49_A4B3_432F_AC1E_D77CEF7488C2
#define ENVIRONMENT_INCLUDED_A4891E49_A4B3_432F_AC1E_D77CEF7488C2


#include <roa/roa.hpp>


namespace Game {


struct Environment
{
  ROA::Object entity;
};


void
setup(Environment *env);


} // ns


#endif // inc guard
