#include <node/node.h>
#include <foundation/assert.h>


/* ------------------------------------------------------------ [ Config ] -- */


#define NODE_ALLOC(bytes)
#define NODE_ASSERT(expr)


/* ------------------------------------------------------------- [ Types ] -- */


struct Node_ctx
{
  uint64_t *ids;
  uint64_t *parents;
};


/* -------------------------------------------------- [ Context Lifetime ] -- */


void
node_ctx_create(Node_ctx **c)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
  }
}


void
node_ctx_destroy(Node_ctx **c)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
  }
}


/* ------------------------------------------------ [ Context Attributes ] -- */


void
node_ctx_info_log_cb(Node_ctx *c, node_ctx_log_fn fn)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
  }
}


void
node_ctx_warn_log_cb(Node_ctx *c, node_ctx_log_fn fn)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
  }
}


void
node_ctx_error_log_cb(Node_ctx *c, node_ctx_log_fn fn)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
  }
}


void
node_ctx_get_stats(Node *c, Node_ctx_stat *out_stats)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
    NODE_ASSERT(out_stats != NULL);
  }
}


/* ----------------------------------------------------- [ Node Lifetime ] -- */


uint64_t
node_create(Node_ctx *c)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
  }
}


void
node_destroy(Node_ctx *c, uint64_t id)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
    NODE_ASSERT(id != 0);
  }
}


/* --------------------------------------------------- [ Node Attributes ] -- */


const char *
node_get_name(Node_ctx *c, uint64_t id)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
    NODE_ASSERT(id != 0);
  }
}


void
node_set_name(Node_ctx *c, uint64_t id, const char *name)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
    NODE_ASSERT(id != 0);
    NODE_ASSERT(name != NULL);
  }
}


uint64_t
node_get_parent(Node_ctx *c, uint64_t id)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
    NODE_ASSERT(id != 0);
  }
}


void
node_set_parent(Node_ctx *c, uint64_t id, uint64_t parent_id)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
    NODE_ASSERT(id != 0);
  }
}

uint64_t
node_get_child_count(Node_ctx *c, uint64_t id)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
    NODE_ASSERT(c != 0);
  }
}


/* --------------------------------------------------------- [ Un-Config ] -- */


#undef NODE_ALLOC
#undef NODE_ASSERT
