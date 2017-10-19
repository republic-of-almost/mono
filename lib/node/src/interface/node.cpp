#include <node/node.h>
#include <string.h>


/* ------------------------------------------------------------ [ Config ] -- */


#ifndef NODE_ALLOC
#include <stdlib.h>
#define NODE_ALLOC(bytes) malloc(bytes)
#endif


#ifndef NODE_FREE
/* assumes stblib include by above */
#define NODE_FREE(addr) free(addr);
#endif


#ifndef NODE_ASSERT
#include <assert.h>
#define NODE_ASSERT(expr) assert(expr)
#endif


/* ------------------------------------------------------------- [ Types ] -- */


struct Node_ctx
{
  /* state */
  uint32_t current_id;

  /* data */
  uint32_t *ids;
  uint32_t *parent_ids;
  uint64_t *bitfields;
  const char **names;

  uint32_t size;
  uint32_t capacity;

  /* log callbacks */
  node_ctx_log_fn info_cb;
  node_ctx_log_fn warn_cb;
  node_ctx_log_fn err_cb;
};


/* -------------------------------------------------- [ Context Lifetime ] -- */


void
node_ctx_create(Node_ctx **c, const Node_ctx_hints *hints)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
  }

  const uint32_t cap_hint = hints ? hints->capacity_hint : (1 << 12);

  /* allocate a new context */
  Node_ctx *new_ctx = NULL;
  {
    int bytes = sizeof(Node_ctx);
    new_ctx = (Node_ctx*)NODE_ALLOC(bytes);
    memset(new_ctx, 0, bytes);

    /* allocate space in arrays */
    {
      const uint32_t id_bytes = sizeof(uint32_t) * cap_hint;
      new_ctx->ids        = (uint32_t*)NODE_ALLOC(id_bytes);
      new_ctx->parent_ids = (uint32_t*)NODE_ALLOC(id_bytes);

      const uint32_t bitfield_bytes = sizeof(uint64_t) * cap_hint;
      new_ctx->bitfields = (uint64_t*)NODE_ALLOC(bitfield_bytes);

      const uint32_t names_bytes = sizeof(const char *) * cap_hint;
      new_ctx->names = (const char**)NODE_ALLOC(names_bytes);
      memset(new_ctx->names, 0, sizeof(const char *) * cap_hint);

      new_ctx->size = 0;
      new_ctx->capacity = cap_hint;
    }
  }

  *c = new_ctx;
}


void
node_ctx_destroy(Node_ctx **c)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
  }

  /* free mem */
  if(*c != NULL)
  {
    NODE_FREE((*c)->ids);
    NODE_FREE((*c)->parent_ids);
    NODE_FREE((*c)->bitfields);

    for(uint32_t i = 0; i < (*c)->capacity; ++i)
    {
      if((*c)->names[i])
      {
        NODE_FREE(&(*c)->names[i]);
      }
    }

    NODE_FREE((*c)->names);
    NODE_FREE(*c);

    *c = NULL;
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

  c->info_cb = fn;
}


void
node_ctx_warn_log_cb(Node_ctx *c, node_ctx_log_fn fn)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
  }

  c->warn_cb = fn;
}


void
node_ctx_error_log_cb(Node_ctx *c, node_ctx_log_fn fn)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
  }

  c->err_cb = fn;
}


void
node_ctx_get_stats(Node_ctx *c, Node_ctx_stats *out_stats)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
    NODE_ASSERT(out_stats != NULL);
  }

  out_stats->count = c->size;
}


/* ----------------------------------------------------- [ Node Lifetime ] -- */


uint32_t
node_create(Node_ctx *c)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
  }
}


void
node_destroy(Node_ctx *c, uint32_t id)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
    NODE_ASSERT(id != 0);
  }
}


/* --------------------------------------------------- [ Node Attributes ] -- */


const char *
node_get_name(Node_ctx *c, uint32_t id)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
    NODE_ASSERT(id != 0);
  }
}


void
node_set_name(Node_ctx *c, uint32_t id, const char *name)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
    NODE_ASSERT(id != 0);
    NODE_ASSERT(name != NULL);
  }
}


uint32_t
node_get_parent(Node_ctx *c, uint32_t id)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
    NODE_ASSERT(id != 0);
  }
}


void
node_set_parent(Node_ctx *c, uint32_t id, uint32_t parent_id)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
    NODE_ASSERT(id != 0);
  }
}

uint32_t
node_get_child_count(Node_ctx *c, uint32_t id)
{
  /* param check */
  {
    NODE_ASSERT(c != NULL);
    NODE_ASSERT(c != 0);
  }
}


/* --------------------------------------------------------- [ Un-Config ] -- */


#undef NODE_ALLOC
#undef NODE_FREE
#undef NODE_ASSERT
