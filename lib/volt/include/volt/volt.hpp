/*
  'Modern' C++ wrapper
*/
#ifdef __cplusplus
#ifndef VOLT_INCLUDED_9B2178A2_FD9E_4630_8C8E_E3444C63EC0B
#define VOLT_INCLUDED_9B2178A2_FD9E_4630_8C8E_E3444C63EC0B


namespace volt {


/* ------------------------------------------------------ [ Volt Context ] -- */


class context final
{

                context(const context &) = delete;
  context&      operator=(const context &) = delete;

public:

  explicit      context();
                ~context();

                context(context &&) noexcept;
  context&      operator=(const context &&) noexcept;

  void          execute();

private:

  struct impl;
  impl *m_impl;

};


} /* ns */


#endif /* inc guard */
#endif /* c++ guard */