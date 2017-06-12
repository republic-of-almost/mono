#ifndef NODE_INCLUDED_83839DB1_FCF2_4E9D_A4A5_D8491B0024C8
#define NODE_INCLUDED_83839DB1_FCF2_4E9D_A4A5_D8491B0024C8


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {


class Node final
{
public:

  /*!
    Creats an entity that is automatically added to the graph.
  */
  explicit
  Node();
  
  /*!
    Create an invalid entity.
  */
  Node(const decltype(nullptr));
  
  /*!
    Creates an entity from an node id.
    Warning, creating an owned entity from this could lead to some unexpcted
    results.
  */
  explicit
  Node(const uint32_t node_id, const bool is_owning = false);
  
  /*!
    If the entity is owning it will destroy it.
  */
  ~Node();
  
  
  // -------------------------------------------------------- [ Copy / Move ] --

  /*!
    Create a new entity via copy/move.
    Move will transfer ownership if it has one.
  */
  Node(const Node &other);
  Node(Node &&other) noexcept;
  
  /*!
    Assginement will destroy the node being assigned to if its an owned node.
  */
  Node& operator=(const Node &other);
  Node& operator=(Node &&other) noexcept;
  
  
  // ---------------------------------------------------------- [ Operators ] --
  
  
  /*!
    Comparision. of id types, a ref to the owner will compare as true if their
    ids are the same.
  */
  bool
  operator==(const Node &other) const;

  /*!
    Comparision. of id types, a ref to the owner will compare as true if their
    ids are the same.
  */
  bool
  operator!=(const Node &other) const;
  
  /*!
    if check if(node), alias for is_valid(node).
  */
  operator bool() const;
  

  // -------------------------------------------------------------- [ State ] --
  

  /*!
   *  Force destroy the node, all dependents will be destroyed as well.
   *  You can destroy nodes through references.
   */
  bool
  destroy();


  /*!
   *  Returns true if the this node is a reference or not. Owned top level
   *  nodes are never references.
   */
  bool
  is_ref() const;


  /*!
   *  Returns true if the node has no parent and is a top level node. All
   *  child nodes become references.
   */
  bool
  is_top_level() const;


  /*!
   *  Returns true if the node is valid, a node can be invalided by a ref
   *  forcing a delete.
   */
  bool
  is_valid() const;


  // ------------------------------------------------------ [ Relationships ] --


  /*!
   *  Set the parent of this node, making this node a child.
   *
   *  Returns false if it failed to add. This will occur if the node isn't
   *  a valid node.
   */
  bool
  set_parent(const Node &other);


  /*!
   * Returns the parent node. If no parent you will get an invalid entity.
   */
  Node
  get_parent() const;


  /*!
   *  Retuns the number of direct children, doesn't included other grand
   *  children etc.
   */
  size_t
  get_child_count() const;
  
  
  /*!
   *  Get the (n)th child.
   */
  Node
  get_child(const size_t i) const;


  /*!
   * Returns all the count of children and grandchildren etc.
   */
  size_t
  get_descendant_count() const;
  
  
  // --------------------------------------------------------------- [ Misc ] --
  
  
  /*!
    Gets the name of the node.
  */
  const char*
  get_name() const;
  
  
  /*!
    Sets the name of the node.
  */
  void
  set_name(const char *);
  
  
  /*!
   * Internal mechanism to get a the node ID.
   */
  uint32_t
  get_id() const;

  
  /*!
   *  Returns the type id of the node data.
   */
  uint64_t
  get_data_type_id() const;
  
  /*!
   * Set the data type id
   */
  void
  set_data_type_id(const uint64_t type_id);
  

private:

  mutable uint32_t        m_node_id;

}; 


} // ns


#endif // inc guard
