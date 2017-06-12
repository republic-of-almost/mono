#ifndef DATA_INCLUDED_EC5AD2F3_79FA_4DF2_A66E_E49EA376C478
#define DATA_INCLUDED_EC5AD2F3_79FA_4DF2_A66E_E49EA376C478


#include <nil/fwd.hpp>
#include <graph/graph_fwd.hpp>
#include <stdint.h>


namespace Nil {
namespace Data {


Graph::Data*
get_graph_data();


Node&
get_root_node();


uint64_t
register_type_id();


} // ns
} // ns


#endif // inc guard
