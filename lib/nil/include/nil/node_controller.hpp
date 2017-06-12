#ifndef NODE_CONTROLLER_INCLUDED_7646DF34_DFB4_446D_99FA_E30741D6FDA6
#define NODE_CONTROLLER_INCLUDED_7646DF34_DFB4_446D_99FA_E30741D6FDA6


#include <stdint.h>
#include <stddef.h>
#include <nil/fwd.hpp>


namespace Nil {


//  ------------------------------------------------------------ [ Node List] --

/*
  A object that returns a temp list of nodes. This list is valid for one frame
  only.
*/


class Node_list final
{
public:

  explicit  Node_list(Node *start, Node *end);
            ~Node_list();
  
            Node_list(const Node_list &other);
            Node_list &operator=(const Node_list &other);

  Node      operator[](const size_t i);
  
  
  
  size_t    size() const;
  Node*     begin() const;
  Node*     end() const;
  
private:

  struct Impl;
  Impl *m_impl;

};


//  ----------------------------------------------------- [ Node Controller ] --

/*
 This class is a helper for aspects to deal with event nodes without having to
 write alot of duplicated code.
 
 The aspect can use this to keep a track of the nodes its interested in.
*/


class Node_controller final
{

  Node_controller(const Node_list &) = delete;
  Node_controller &operator=(const Node_list &) = delete;

public:

  explicit          Node_controller(const uint64_t data_types = 0);
                    ~Node_controller();
  
                    Node_controller(Node_controller &&other) noexcept;
  Node_controller&  operator=(Node_controller &&other) noexcept;
  
  void              process(Event_list &evts);
  Node_list         added() const;
  Node_list         updated() const;
  Node_list         updated_and_added() const;
  Node_list         removed() const;
  Node_list         all() const;

private:

  struct Impl;
  Impl *m_impl;

};


} // ns


#endif // inc guard
