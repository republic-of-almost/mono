#ifndef BOUNDING_BOX_INCLUDED_AA48A4A2_B0B4_4900_B7FE_49890753B80A
#define BOUNDING_BOX_INCLUDED_AA48A4A2_B0B4_4900_B7FE_49890753B80A


#include <roa/fundamental.hpp>
#include <roa/object.hpp>


namespace ROA {


class Bounding_box : public Object
{
public:

  explicit      Bounding_box();
  
  Vector3       get_min() const;
  void          set_min(const Vector3 &in);
  
  Vector3       get_max() const;
  void          set_max(const Vector3 &in);

};


} // ns


#endif // inc guard