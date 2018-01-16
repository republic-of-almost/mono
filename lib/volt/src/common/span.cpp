#include <common/span.hpp>


namespace volt {


char_span::char_span()
: char_span(nullptr, 0)
{
}


char_span::char_span(const char *va, unsigned si)
: value(va)
, size(si)
{

}


} /* ns */