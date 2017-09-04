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
  
  bool                  is_valid() const;
  bool                  is_ref() const;
  
  
  // ------------------------------------------------------ [ Relationships ] --
  
  
  Object                get_parent() const;
  void                  set_parent(const Object obj);
  
  Object                get_child(const size_t child) const;
  size_t                get_child_count() const;
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  const char*           get_name() const;
  void                  set_name(const char *name);
  
  virtual const char*   get_type_name() const;
  virtual uint32_t      get_type_id() const;
  uint32_t              get_instance_id() const;
  
  uintptr_t             get_user_data() const;
  void                  set_user_data(uintptr_t user_data);
  
  
  // --------------------------------------------------------- [ Components ] --
  
  
  template<typename T>
  bool
  add_component()
  {
    static_assert(T::get_rtti() != 0, "Is this a ROA::Component");
  
    if(!has_component<T>())
    {
      T *obj = new T{};
      const uint32_t instance_id = this->get_instance_id();
      
      return ROA_detail::add_component(instance_id, obj);
    }
    
    return false;
  }
  
  
  template<typename T>
  bool
  has_component()
  {
    static_assert(T::get_rtti() != 0, "Is this a ROA::Component");
    
    const uint32_t rtti = T::get_rtti();
    const uint32_t instance_id = this->get_instance_id();
    
    return ROA_detail::has_component(instance_id, rtti);
  }
  
  
  template<typename T>
  T*
  get_component()
  {
    static_assert(T::get_rtti_id() != 0, "Is this a ROA::Component");
    
    const uint32_t rtti = T::get_rtti();
    const uint32_t instance_id = this->get_instance_id();
  
    return static_cast<T*>(ROA_detail::get_component(instance_id, rtti));
  }
  
  
  // ---------------------------------------------------- [ Data Components ] --
  
  
  const Bounding_box    get_bounding_box() const;
  Bounding_box          get_bounding_box();
  void                  set_bounding_box(const Bounding_box &in);
  
  const Camera          get_camera() const;
  Camera                get_camera();
  void                  set_camera(const Camera &in);
  
  const Light           get_light() const;
  Light                 get_light();
  void                  set_light(const Light &in);
  
  const Logic           get_logic() const;
  Logic                 get_logic();
  void                  set_logic(const Logic &in);
  
  const Transform       get_transform() const;
  Transform             get_transform();
  void                  set_transform(const Transform &in);
  
  const Renderable      get_renderable() const;
  Renderable            get_renderable();
  void                  set_renderable(const Renderable &in);
  
  
private:


  mutable uint64_t      m_id;
  
}; // ns


} // ns


#endif // inc guard
