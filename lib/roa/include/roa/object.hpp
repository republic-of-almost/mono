#ifndef OBJECT_INCLUDED_EC4C4710_83A5_4AC8_AF58_EAD6699152E9
#define OBJECT_INCLUDED_EC4C4710_83A5_4AC8_AF58_EAD6699152E9


#include <roa/fundamental.hpp>
#include <roa/detail/component_interface.hpp>


namespace ROA {


namespace Data {
enum ENUM : uint64_t {

  BOUNDING_BOX    = 1 << 0,
  CAMERA          = 1 << 1,
  LOGIC           = 1 << 2,
  RENDERABLE      = 1 << 3,

};
} // ns


class Object
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --
  /*
    Methods that relate to an objects lifetime.
    To create invalid objects call Object{nullptr}.
  */
  

  explicit              Object();
  explicit              Object(const uint32_t instance_id);
  explicit              Object(const uint64_t data_bitfield);
  explicit              Object(const ROA_nullptr);
  
                        Object(const Object &other) noexcept;
                        Object(Object &&other) noexcept;
  
  virtual               ~Object();
  
  Object&               operator=(const Object &other) noexcept;
  Object&               operator=(Object &&other) noexcept;
  
  void                  destroy();


  // -------------------------------------------------------------- [ State ] --
  /*
    Various state checks.
  */

  bool                  is_valid() const;
  bool                  is_ref() const;
  
  
  // ------------------------------------------------------ [ Relationships ] --
  /*
    An object can have many children and one parent.
  */
  
  
  Object                get_parent() const;
  void                  set_parent(const Object obj);
  
  Object                get_child(const size_t child) const;
  size_t                get_child_count() const;
  
  
  // -------------------------------------------------------- [ Name / Tags ] --
  /*
    You can tag and name nodes.
  */
  const char*           get_name() const;
  void                  set_name(const char *name);
  
  
  // --------------------------------------------------------- [ Attributes ] --
  /*
    Various attributes of an object
  */
  
  virtual const char*   get_type_name() const;
  virtual uint32_t      get_type_id() const;
  uint32_t              get_instance_id() const;
  
  uintptr_t             get_user_data() const;
  void                  set_user_data(uintptr_t user_data);
  
  
  // --------------------------------------------------------- [ Components ] --
  /*
    Components are custom bits of logic that can be added to a node.
    You can add as many components to node as you wish.
  */
  
  
  template<typename T>
  bool
  add_component()
  {
    static_assert(T::get_rtti() != 0, "Is this a ROA::Component");
    const bool has_comp = has_component<T>();
  
    if(!has_comp)
    {
      T *comp                    = new T{};
      comp->m_obj                = *this;
      const uint32_t instance_id = this->get_instance_id();
      
      return ROA_detail::add_component(instance_id, comp);
    }
    
    return false;
  }
  
  
  template<typename T>
  bool
  has_component()
  {
    static_assert(T::get_rtti() != 0, "Is this a ROA::Component");
    
    const uint32_t rtti        = T::get_rtti();
    const uint32_t instance_id = this->get_instance_id();
    
    return ROA_detail::has_component(instance_id, rtti);
  }
  
  
  template<typename T>
  T*
  get_component()
  {
    static_assert(T::get_rtti() != 0, "Is this a ROA::Component");
    
    const uint32_t rtti        = T::get_rtti();
    const uint32_t instance_id = this->get_instance_id();
  
    return static_cast<T*>(ROA_detail::get_component(instance_id, rtti));
  }
  
  
  // ---------------------------------------------------- [ Data Components ] --
  /*
    Data can be added to a node, you need to first call set before get returns
    valid data.
  */
  
  template<typename T>
  void
  set_data(const T &t)
  {
    const uint32_t instance_id = this->get_instance_id();
    
    ROA_detail::set_node_data(instance_id, t);
  }
  
  template<typename T>
  T
  get_data()
  {
    T t(nullptr);
    
    const uint32_t instance_id = this->get_instance_id();
    
    ROA_detail::get_node_data(instance_id, t);
    
    return t;
  }
  
  template<typename T>
  const T
  get_data() const
  {
    T t(nullptr);
    
    const uint32_t instance_id = this->get_instance_id();
    
    ROA_detail::get_node_data(instance_id, t);
    
    return t;
  };
  
  template<typename T>
  bool
  has_data() const
  {
    return false;
  }
  
private:

  mutable uint64_t      m_id;
  
}; // ns


} // ns


#endif // inc guard
