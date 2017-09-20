#ifndef CAMERA_INCLUDED_E9FEABA9_7360_4FF1_807A_5FB858855C17
#define CAMERA_INCLUDED_E9FEABA9_7360_4FF1_807A_5FB858855C17


#include <roa/fundamental.hpp>
#include <roa/object.hpp>


namespace ROA {


class Camera : public Object
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --


  explicit          Camera();
                    ~Camera();
  
  
  // --------------------------------------------------------- [ Attributes ] --

  
  int32_t           get_priority() const;
  void              set_priority(const int32_t priority);
  
  float             get_width() const;
  void              set_width(const float width);
  
  float             get_height() const;
  void              set_height(const float height);
  
  float             get_field_of_view() const;
  void              set_field_of_view(const float radians);
  
  float             get_near_plane() const;
  void              set_near_plane(const float near);
  
  float             get_far_plane() const;
  void              set_far_plane(const float far);
  
  Color             get_clear_color() const;
  void              set_clear_color(const Color &color);

};


} // ns


#endif // inc guard
