#ifndef OBJECT_INCLUDED_EC4C4710_83A5_4AC8_AF58_EAD6699152E9
#define OBJECT_INCLUDED_EC4C4710_83A5_4AC8_AF58_EAD6699152E9


#include <roa/fundamental.hpp>


namespace ROA {


class Object
{

  // ----------------------------------------------------------- [ Lifetime ] --
  

  explicit              Object(const uint32_t instance_id);


public:
  

  explicit              Object();
  explicit              Object(const ROA_nullptr);
  
                        Object(const Object &other);
                        Object(Object &&other);
  
  virtual               ~Object();
  
  Object&               operator=(const Object &other);
  Object&               operator=(const Object &&other);
  
  void                  destroy();
  
  bool                  is_valid() const;
  bool                  is_ref() const;
  
  
  // ------------------------------------------------------ [ Relationships ] --
  
  
  Object                get_parent() const;
  Object                get_child(const size_t child) const;
  size_t                get_child_count() const;
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  const char*           get_name() const;
  void                  set_name(const char *name);
  
  virtual const char*   get_type_name() const;
  virtual uint32_t      get_type_id() const;
  uint32_t              get_instance_id() const;
  
  
  // ---------------------------------------------------- [ Data Components ] --
  
  
  Bounding_box          get_bounding_box() const;
  void                  set_bounding_box(const Bounding_box &in);
  
  Camera                get_camera() const;
  void                  set_camera(const Camera &in);
  
  Logic                 get_logic() const;
  void                  set_logic(const Logic &in);
  
  Transform             get_transform() const;
  void                  set_transform(const Transform &in);
  
  
private:


  mutable uint64_t      m_id;
  
}; // ns


} // ns


#endif // inc guard
