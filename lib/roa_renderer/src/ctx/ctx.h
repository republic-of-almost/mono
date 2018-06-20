#ifndef ROA_RENDERER_CTX_INCLUDED_B747703D_4DBC_4BC2_A813_2D060A864919
#define ROA_RENDERER_CTX_INCLUDED_B747703D_4DBC_4BC2_A813_2D060A864919


#include <roa_lib/fundamental.h>
#include <roa_renderer/roa_renderer.h>
#include <roa_lib/atomic.h>


#ifdef ROA_RENDERER_API_NOOP
#include <graphics_api/noop/noop.h>
#elif defined ROA_RENDERER_API_GL4
#include <graphics_api/ogl/ogl4.h>
#endif


/* ----------------------------------------------- [ Internal Structures ] -- */


struct renderer_mesh_renderable
{
        float position[3];
        float scale[3];
        float rotation[4];
  
        uint64_t mesh_id;
};


struct renderer_camera
{
        int i;
};


/* -- [ Renderpasses ] -- */


struct renderpass_draw_call {
        uint32_t object_id;
        float world[16];
        float wvp[16];
};


struct decal_transform {
        uint32_t object_id;
        float world_mat[16];
        float color[3];
};


struct renderpass {
        struct roa_renderer_camera camera;

        float camera_view[16];
        float camera_projection[16];
        float camera_view_projection[16];

        struct renderpass_draw_call *draw_calls;
        struct decal_transform *decals;
};



/* ---------------------------------------------------------- [ Raw Data ] -- */


struct renderer_data_desc {
        roa_atomic_int lock;

        /* array */ uint32_t *camera_ids;
        /* array */ struct renderpass *camera_passes;

        /* array */ uint32_t *mesh_rdr_ids;
        /* array */ struct renderer_mesh_renderable *mesh_rdr_descs;

        /* array */ uint32_t *light_ids;
        /* array */ struct roa_renderer_light *light_descs;
};


struct renderer_resource_data_desc {
        roa_atomic_int lock;

        /* array */ uint64_t *mesh_ids;
        /* array */ struct roa_renderer_mesh_resource *mesh_rsrc_data;

        /* array */ uint64_t *mesh_pending_ids;
        /* array */ struct roa_renderer_mesh_resource *mesh_rsrc_pending_data;

        /* array */ uint64_t *texture_ids;
        /* array */ struct roa_renderer_texture_resource *texture_rsrc_data;

        /* array */ uint64_t *texture_pending_ids;
        /* array */ struct roa_renderer_texture_resource *texture_rsrc_pending_data;

        /* array */ uint64_t *material_ids;
        /* array */ struct roa_renderer_material_resource *mat_descs;
};


/* -- [ Device ] -- */


struct device_setting_data {
        roa_atomic_int lock;

        float device_viewport[2];
};


/* -- [ Context ] -- */


struct roa_renderer_ctx {
        struct device_setting_data device_settings;
        struct renderer_data_desc renderer_desc;
        struct renderer_resource_data_desc resource_desc;
        struct graphics_api graphics_api;
};


#endif /* inc guard */
