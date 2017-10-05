/*
  This is a prototype thing
*/
#ifndef ROA_DATA_INCLUDED_5D865112_9FA0_4F10_B81B_BA1B557DD53C
#define ROA_DATA_INCLUDED_5D865112_9FA0_4F10_B81B_BA1B557DD53C


#include <roa/object.hpp>
#include <vector.hpp>


namespace ROA {


template<typename T>
class Data
{
public:

  void
  emplace_back(const ROA::Object &obj, const T &data)
  {
    
  }
  

private:

  std::vector<T> data;
  std::vector<ROA::Object> keys;

};


} // ns


#endif // inc guard
