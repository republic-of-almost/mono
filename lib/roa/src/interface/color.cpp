#include <roa/color.hpp>
#include <lib/color.hpp>


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


Color::Color(const float rgba[])
: Color(lib::color::init(rgba))
{
}


Color::Color(const uint8_t rgba[])
: Color(lib::color::init(rgba))
{
}



} // ns
