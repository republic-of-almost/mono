#include <roa/color.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Color::Color()
: Color(0xFFFFFFFF)
{
}


Color::Color(const uint32_t hex)
: m_data(hex)
{
}


} // ns
