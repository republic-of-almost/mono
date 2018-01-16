#ifndef SPAN_INCLUDED_325D1B4E_99EE_419D_B57A_9AC1BB4DC4A6
#define SPAN_INCLUDED_325D1B4E_99EE_419D_B57A_9AC1BB4DC4A6


namespace volt {


struct char_span
{
  const char *value = nullptr;
  unsigned size = 0;

  char_span();
  char_span(const char *va, unsigned si);
};


} /* ns */


#endif /* inc guard */
