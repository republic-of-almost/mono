#ifndef ROA_TASK_INCLUDED_
#define ROA_TASK_INCLUDED_


typedef void(*roa_renderer_task_fn)(void *);


struct roa_renderer_task
{
	void *argument;
	roa_renderer_task_fn function;
};


#endif /* inc guard */
