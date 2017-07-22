#ifndef COLLECTION_INCLUDED_239746E1_A856_40BB_A04A_E8A60DAE1850
#define COLLECTION_INCLUDED_239746E1_A856_40BB_A04A_E8A60DAE1850


#include <roa/fundamental.hpp>


namespace ROA {


class Collection final
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --


  explicit            Collection();
  explicit            Collection(const Object *obj, const size_t count);
                      ~Collection();
  
                      Collection(const Collection &other);
                      Collection(Collection &&other);
  
  Collection&         operator=(const Collection &other);
  Collection&         operator=(Collection &&other);
  
  
  // ------------------------------------------------------------- [ Access ] --


  const Object*       begin() const;
  Object*             begin();
  
  const Object*       end() const;
  Object*             end();
  
  const Object*       data() const;
  Object*             data();
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  bool                empty() const;
  size_t              size() const;
  
  
private:


  struct Impl;
  Impl *m_impl;

};


} // ns


#endif // inc guard
