#ifndef ROA_RENDERER_CTX_INCLUDED_B747703D_4DBC_4BC2_A813_2D060A864919
#define ROA_RENDERER_CTX_INCLUDED_B747703D_4DBC_4BC2_A813_2D060A864919


#include <roa_lib/fundamental.h>


struct roa_renderer_ctx
{
	/* array */ uint32_t *camera_id;
	/* array */ int *cameras;

	/* array */ uint32_t *renderables_id;
	/* array */ int *renderables;
};


#endif /* inc guard */
