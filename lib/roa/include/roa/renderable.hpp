#ifndef RENDERABLES_INCLUDED_F72EDFB7_3245_4DC6_B40C_182978AEDA59
#define RENDERABLES_INCLUDED_F72EDFB7_3245_4DC6_B40C_182978AEDA59


#include <roa/fundamental.hpp>
#include <roa/object.hpp>


namespace ROA {


class Renderable : public Object
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --


  explicit        Renderable();
                  ~Renderable();
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  Mesh            get_mesh() const;
  void            set_mesh(const Mesh &in);
  
  Material        get_material() const;
  void            set_material(const Material &in);

};


} // ns


#endif // inc guard
