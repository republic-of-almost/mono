/*
  Node is a generic (boo!) object collection, this maintains the relationships
  between nodes and little else.
*/
#ifndef NODE_INCLUDED_0930FD16_74FB_464B_919E_C26F83AFFCD9
#define NODE_INCLUDED_0930FD16_74FB_464B_919E_C26F83AFFCD9


#include <inttypes.h>


/* -------------------------------------------------- [ Context Lifetime ] -- */
/*
  All nodes exist inside a context you must create a context. When finished
  be sure to destroy the context.
*/


struct Node_ctx;

struct Node_ctx_hints
{
  uint32_t capacity_hint;
};


void          node_ctx_create(Node_ctx **c, const Node_ctx_hints *opt_hints);
void          node_ctx_destroy(Node_ctx **c);


/* ------------------------------------------------ [ Context Attributes ] -- */
/*
  The context has various logging callbacks and you can query the system to see
  what is in the node context.
*/


typedef void(*node_ctx_log_fn)(const char *msg);

void          node_ctx_info_log_cb(Node_ctx *c, node_ctx_log_fn fn);
void          node_ctx_warn_log_cb(Node_ctx *c, node_ctx_log_fn fn);
void          node_ctx_error_log_cb(Node_ctx *c, node_ctx_log_fn fn);


struct Node_ctx_stats
{
  uint32_t count;
};


void          node_ctx_get_stats(Node_ctx *c, Node_ctx_stats *out_stats);


/* ----------------------------------------------------- [ Node Lifetime ] -- */
/*
  Create and destroy nodes. if you destroy a parent node, it will destroy all
  the child nodes as well.
*/


uint32_t      node_create(Node_ctx *c);
void          node_destroy(Node_ctx *c, uint32_t id);


/* ------------------------------------------------ [ Node Relationships ] -- */
/*
  Node can be connected to other nodes, this will alter there world transforms.
*/


uint32_t      node_get_parent(Node_ctx *c, uint32_t id);
void          node_set_parent(Node_ctx *c, uint32_t id, uint32_t parent_id);

uint32_t      node_get_child_count(Node_ctx *c, uint32_t id);


/* --------------------------------------------------- [ Node Attributes ] -- */
/*
  Node can be connected to other nodes, this will alter there world transforms.
*/


struct Transform
{
  float position[3];
  float scale[3];
  float roation[4];
};


struct Bounding_box
{
  float min[3];
  float max[3];
};


const char *  node_get_name(Node_ctx *c, uint32_t id);
void          node_set_name(Node_ctx *c, uint32_t id, const char *name);

uint64_t      node_get_bitfield(Node_ctx *c, uint32_t id);
void          node_set_bitfield(Node_ctx *c, uint32_t id, uint64_t field);

uintptr_t     node_get_user_data(Node_ctx *c, uint32_t id);
void          node_set_user_data(Node_ctx *c, uint32_t id, uintptr_t user_data);

Transform     node_get_world_transform(Node_ctx *c, uint32_t id);
void          node_set_world_transform(Node_ctx *c, uint32_t id, Transform *trans);

Transform     node_get_local_transform(Node_ctx *c, uint32_t id);
void          node_set_local_transform(Node_ctx *c, uint32_t id, Transform *trans);

Bounding_box  node_get_bounding_box(Node_ctx *c, uint32_t id);
void          node_set_bounding_box(Node_ctx *c, uint32_t id, Bounding_box *box);


#endif // inc guard
