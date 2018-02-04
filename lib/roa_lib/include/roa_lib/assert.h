#ifndef ROA_ASSERT_INCLUDED_2EE1722A_3152_4D39_8B20_849C8BE531EB
#define ROA_ASSERT_INCLUDED_2EE1722A_3152_4D39_8B20_849C8BE531EB


#ifdef __cplusplus
extern "C" {
#endif


void
roa_internal_assert(
	const char *expr,
	const char *file,
	unsigned line,
	const char *func);


#ifdef _WIN32
#define ROA_ASSERT_PEDANTIC(expr) if(!(expr)) { roa_internal_assert(#expr, __FILE__, __LINE__, __FUNCTION__); }
#define ROA_ASSERT(expr) if(!(expr)) { roa_internal_assert(#expr, __FILE__, __LINE__, __FUNCTION__); }
#else
#define ROA_ASSERT_PEDANTIC(expr) if(!(expr)) { __builtin_trap(); }
#define ROA_ASSERT(expr) if(!(expr)) { __builtin_trap(); }
#endif


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
