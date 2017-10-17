/*
  Node is a generic object collection, this manages the relationships between
  nodes and little else.
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

void          node_ctx_create(Node_ctx **c);
void          node_ctx_destroy(Node_ctx **c);


/* ------------------------------------------------ [ Context Attributes ] -- */
/*
  The context has various logging callbacks and you can query the system to see
  what is in the node context.
*/


typedef void(node_ctx_log_fn*)(const char *msg);

void          node_ctx_info_log_cb(Node_ctx *c, node_ctx_log_fn fn);
void          node_ctx_warn_log_cb(Node_ctx *c, node_ctx_log_fn fn);
void          node_ctx_error_log_cb(Node_ctx *c, node_ctx_log_fn fn);


struct Node_ctx_stat
{
};


void          node_ctx_get_stats(Node *c, Node_ctx_stat *out_stats);


/* ----------------------------------------------------- [ Node Lifetime ] -- */
/*
  Create and destroy nodes. if you destroy a parent node, it will destroy all
  the child nodes as well.
*/


uint64_t      node_create(Node_ctx *c);
void          node_destroy(Node_ctx *c, uint64_t id);


/* --------------------------------------------------- [ Node Attributes ] -- */
/*
  Nodes can have relationships, and can contain various data such as names etc.
*/


const char *  node_get_name(Node_ctx *c, uint64_t id);
void          node_set_name(Node_ctx *c, uint64_t id, const char *name);

uint64_t      node_get_parent(Node_ctx *c, uint64_t id);
void          node_set_parent(Node_ctx *c, uint64_t id, uint64_t parent_id);

uint64_t      node_get_child_count(Node_ctx *c, uint64_t id);


#endif // inc guard
