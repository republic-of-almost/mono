#ifndef GRAPH_DATA_INCLUDED_C75347EC_83FC_47B2_89DF_8F641A0DD327
#define GRAPH_DATA_INCLUDED_C75347EC_83FC_47B2_89DF_8F641A0DD327


#include <roa_lib/fundamental.h>
#include <roa_math/math.h>
#include <roa_lib/atomic.h>


#define ROA_GRAPH_SHORT_STRING 16


struct short_string
{
  char data[ROA_GRAPH_SHORT_STRING];
};


struct node_data
{
  void *user_data;
  uint64_t node_type_id;
  uint64_t tags;
  struct short_string name;
};


/* -------------------------------------------------------------- [ Data ] -- */


struct roa_graph_ctx
{
  /* array */ uint32_t					*node_id;
  /* array */ uint64_t					*parent_depth_data;
  /* array */ roa_transform			*local_transform;
  /* array */ roa_transform			*world_transform;
  /* array */ struct node_data	*data;


  /* -- Other -- */
  uint32_t                    instance_counter;

};



#endif /* inc guard */
