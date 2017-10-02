#ifndef ASSET_LOADING_INCLUDED_F69B0D91_2909_4DF5_9E54_AF3A8D7AA006
#define ASSET_LOADING_INCLUDED_F69B0D91_2909_4DF5_9E54_AF3A8D7AA006


#include <nil/node.hpp>


namespace Nil {
namespace Assets {


/*!
  Loads an OBJ file and converts it to Nil data types.
  If you pass a valid node, instances will be created and added to this node.
*/
bool
load_obj(Nil_ctx *ctx, Nil::Node node, const char *filename);


/*!
  Loads a GLTF file and converts it to Nil data types.
  If you pass a valid node, instances will be created and added to this node.
*/
bool
load_gltf(Nil_ctx *ctx, Nil::Node node, const char *filename);


} // ns
} // ns


#endif // inc guard
