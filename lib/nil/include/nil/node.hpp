#ifndef NODE_INCLUDED_83839DB1_FCF2_4E9D_A4A5_D8491B0024C8
#define NODE_INCLUDED_83839DB1_FCF2_4E9D_A4A5_D8491B0024C8


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {


/*!
  Node
  --
  These represent a place in the graph. they have complicated semantics. An owning node that goes out of scope will destroy all its data. A non owning node (ref) will not. You can move an owning node. You cannot move a a ref.
*/
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
  explicit
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
  
  
  // ------------------------------------------------------- [ Names / Tags ] --
  
  
  /*!
    Gets the name of the node.
  */
  const char*
  get_name() const;
  
  
  /*!
    Sets the name of the node.
  */
  void
  set_name(const char *name);
  
  
  /*!
    Add a tag, must 64 or less characters.
  */
  bool
  add_tag(const char *tag);
  
  
  /*!
    Has a tag.
  */
  void
  has_tag(const char *tag) const;
  
  
  /*!
    Remove a tag
  */
  void
  remove_tag(const char *tag);
  
  
  /*!
    Find a tag
  */
  void
  get_tag(const size_t i) const;
  
  /*!
    Get the number of tags
  */
  size_t
  get_tag_count() const;
  
  
  /*!
    Clear tags
  */
  void
  clear_tags();
  
  
  // --------------------------------------------------------------- [ Misc ] --
  
  
  /*!
    Gets the user data
  */
  uintptr_t
  get_user_data() const;
  
  
  /*!
    Sets the user data
  */
  void
  set_user_data(const uintptr_t user_data);
  
  
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
  

private:

  mutable uint32_t        m_node_id;

}; 


} // ns


#endif // inc guard
