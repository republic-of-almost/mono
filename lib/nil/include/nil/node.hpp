#ifndef NODE_INCLUDED_83839DB1_FCF2_4E9D_A4A5_D8491B0024C8
#define NODE_INCLUDED_83839DB1_FCF2_4E9D_A4A5_D8491B0024C8


#include <nil/fwd.hpp>


namespace Nil {


/*!
  Node
  --
  These represent a place in the graph, they have complicated semantics.
  An owning node that goes out of scope will destroy all its data. A non-owning
  node (ref) will not. You can move an owning node. You cannot move a a ref.
*/
class Node final
{
public:

  
  // ----------------------------------------------------------- [ Lifetime ] --
  /*
    Node life time is complex. If a node is owned by another (aka a ref) it will
    not be destroyed on out of scope, it can be destroyed manually by calling
    destroy().
    
    To create an invalid entity Node{nullptr}, this is useful for Node members.
  */
  
  
  explicit          Node();
  explicit          Node(const decltype(nullptr));
  explicit          Node(const uint32_t node_id, const bool is_owning = false);
  
  Node&             operator=(const Node &other);
                    Node(const Node &other);

                    Node(Node &&other) noexcept;
  Node&             operator=(Node &&other) noexcept;
  
                    ~Node();
  bool              destroy();

  
  
  // ---------------------------------------------------------- [ Operators ] --
  /*
    Equality operators test if the node id's are equal.
    operator bool() is alias for is_valid()
  */


  bool              operator==(const Node &other) const;
  bool              operator!=(const Node &other) const;
  
                    operator bool() const;
  

  // -------------------------------------------------------------- [ State ] --
  /*
    Various State checkers
  */


  bool              is_ref() const;
  bool              is_valid() const;
  bool              is_top_level() const;


  // ------------------------------------------------------ [ Relationships ] --
  /*
    A node can have any number of children and one parent. If a node is a child
    of another node, it will no longer be destroyed if Nil::Node object goes out
    of scope.
  */
  
  
  bool              set_parent(const Node &other);
  Node              get_parent() const;
  size_t            get_child_count() const;
  Node              get_child(const size_t i) const;
  size_t            get_descendant_count() const;
  
  
  // -------------------------------------------------------- [ Name / Tags ] --
  /*
    A node can have a Name and can be tagged. Both names and tags are truncated.
    See build.md for details.
    
    The tagging system supports upto 64 tags.
  */
  
  
  const char*       get_name() const;
  void              set_name(const char *name);
  
  bool              add_tag(const char *tag);
  bool              has_tag(const char *tag) const;
  void              remove_tag(const char *tag);
  void              get_tag(const size_t i) const;
  size_t            get_tag_count() const;
  void              clear_tags();
  
  
  // --------------------------------------------------------------- [ Misc ] --
  /*
    General attributes.
  */
  
  
  uintptr_t         get_user_data() const;
  void              set_user_data(const uintptr_t user_data);
  
  uint32_t          get_id() const;
  uint64_t          get_data_type_id() const;
  

private:

  mutable uint32_t  m_node_id;

}; 


} // ns


#endif // inc guard
