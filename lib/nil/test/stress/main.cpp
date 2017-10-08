/*
  Stress
  --
  This has no output.
  Attach a profiler to this program, make changes accordingly.
*/
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <vector>


//#define NIL_STRESS_CREATE_DESTROY
//#define NIL_STRESS_LONG_LINEAR_RELATIONSHIP
#define NIL_STRESS_LONG_SHUFFLE_RELATIONSHIP


int
main()
{
  Nil_ctx *ctx;
  nil_ctx_initialize(&ctx);
  
  
  /*
    Simple create and destroy test.
    Only to find slowness Ctor/Dtors.
  */
  #ifdef NIL_STRESS_CREATE_DESTROY
  {
    constexpr unsigned int test_size = 10000000;
    
    for(uint32_t i = 0; i < test_size; ++i)
    {
      Nil::Node node;
    }
  }
  #endif
  
  
  /*
    Simple linear relationship.
    Adding a new child to the end of a relationship.
  */
  #ifdef NIL_STRESS_LONG_LINEAR_RELATIONSHIP
  {
    constexpr unsigned int test_size = 20000;
  
    Nil::Node parent;
    
    for(uint32_t i = 0; i < test_size; ++i)
    {
      Nil::Node node;
      node.set_parent(parent);
    }
  }
  #endif
  
  
  /*
    Create new parents.
    This test shuffles down all the nodes when we add a new
    parent.
  */
  #ifdef NIL_STRESS_LONG_SHUFFLE_RELATIONSHIP
  {
    constexpr unsigned int test_size = 20000;
  
    std::vector<Nil::Node> nodes;
    nodes.resize(test_size);
    
    for(uint32_t i = 1; i < test_size; ++i)
    {
      nodes[i].set_parent(nodes[i -1]);
    }
  }
  #endif


  nil_ctx_destroy(&ctx);

  return 0;
}
