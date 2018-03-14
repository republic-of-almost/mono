#ifndef ROA_TASK_INCLUDED_56435A9E_2011_4D0D_B76D_854777D6DDDB
#define ROA_TASK_INCLUDED_56435A9E_2011_4D0D_B76D_854777D6DDDB


typedef void(*roa_renderer_task_fn)(void *);


struct roa_renderer_task
{
	void *argument;
	roa_renderer_task_fn function;
};


#endif /* inc guard */
