#ifndef ROA_RENDERER_CTX_INCLUDED_B747703D_4DBC_4BC2_A813_2D060A864919
#define ROA_RENDERER_CTX_INCLUDED_B747703D_4DBC_4BC2_A813_2D060A864919


#include <roa_lib/fundamental.h>
#include <roa_renderer/roa_renderer.h>
#include <volt/volt.h> /* temp thing */

struct roa_renderer_ctx
{
	/* data */

	/* array */ uint32_t *camera_id;
	/* array */ struct roa_renderer_camera *camera;

	/* array */ uint32_t *renderable_id;
	/* array */ struct roa_renderer_renderable *renderable;

	/* job data */

	/* todo: when we start culling etc the data goes here */

	/* temp render data */
	
};


#endif /* inc guard */
