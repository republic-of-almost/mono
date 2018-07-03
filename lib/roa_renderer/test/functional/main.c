#include <roa_renderer/roa_renderer.h>
#include <roa_ctx/roa_ctx.h>
#include <roa_math/math.h>
#include <roa_lib/dir.h>
#include <scratch/geometry.h>
#include <scratch/textures.h>
#include <gltf/gltf.h>
#include <stb/stb_image.h>
#include <stdio.h>
#include <string.h>


#define LOGGING 0


/* --------------------------------------------------- [ Func Test Ctx's ] -- */


struct app
{
        roa_ctx_t hw_ctx;
        roa_renderer_ctx_t renderer_ctx;

        uint32_t object_id_counter;
        int renderable_count;

        float spin;
        float pitch;
        float cam_radius;
} app;


/* --------------------------------------------------------- [ Func Test ] -- */


void
app_tick(struct app *app)
{
        static float time = 0.f;
        time += 0.1f;

        /* set camera */
        struct roa_ctx_mouse_desc ms_desc;
        ROA_MEM_ZERO(ms_desc);
        roa_ctx_mouse_get_desc(app->hw_ctx, &ms_desc);

        app->spin += (ms_desc.x_delta * 0.001f);
        app->pitch += (ms_desc.y_delta * 0.001f);
        app->cam_radius += (ms_desc.y_scroll * 2.2f);

        app->pitch = roa_float_clamp(app->pitch, -ROA_QUART_TAU, +ROA_QUART_TAU);

        roa_quaternion yaw_rot = roa_quaternion_from_axis_angle(
                roa_transform_world_up(),
                app->spin);

        roa_quaternion pitch_rot = roa_quaternion_from_axis_angle(
                roa_transform_world_fwd(),
                app->pitch);

        roa_quaternion final_rot = roa_quaternion_multiply(yaw_rot, pitch_rot);

        roa_float3 rot_pos = roa_quaternion_rotate_vector(
                final_rot,
                roa_transform_world_left());
        rot_pos = roa_float3_scale(rot_pos, app->cam_radius);

        struct roa_renderer_camera camera;
        ROA_MEM_ZERO(camera);
        roa_renderer_camera_get(app->renderer_ctx, &camera, 1);

        roa_float3 at   = roa_float3_fill_with_value(0.f);
        roa_float3 up   = roa_quaternion_rotate_vector(
        final_rot, roa_transform_world_up());

        roa_float3_export(rot_pos, camera.position);
        roa_float3_export(at, camera.lookat);
        roa_float3_export(up, camera.up);

        roa_renderer_camera_set(app->renderer_ctx, &camera, 1);

        /* pump renderer events */
        while(1) {
              struct roa_renderer_task *tasks = ROA_NULL;
              unsigned task_count = 0;

              task_count = roa_renderer_task_pump(app->renderer_ctx, &tasks);

              if (task_count == 0) {
                      /* no more tasks */
                      break;
              }

              unsigned i;
              for(i = 0; i < task_count; ++i) {
                      roa_renderer_task_run(app->renderer_ctx, &tasks[i]);
              }
        }

        roa_renderer_ctx_execute(app->renderer_ctx);
}


void setup_app(struct app *app);
void load_resources(struct app *app);
void load_objects(struct app *app);


int
main()
{
        ROA_MEM_ZERO(app);
        app.cam_radius = 50.f;

        setup_app(&app);
        load_resources(&app);
        load_objects(&app);


        /* process the scene */
        while (roa_ctx_new_frame(app.hw_ctx)) {
                /* quit if escape pressed */
                struct roa_ctx_keyboard_desc kb_desc;
                roa_ctx_keyboard_get_desc(app.hw_ctx, &kb_desc);

                if(kb_desc.kb_keys[ROA_KB_ESC]) {
                        break;
                }
          
                /* set resolution */
                struct roa_ctx_window_desc win_desc;
                ROA_MEM_ZERO(win_desc);

                roa_ctx_get_window_desc(app.hw_ctx, &win_desc);

                struct roa_renderer_device_viewport vp;
                ROA_MEM_ZERO(vp);

                vp.width = win_desc.width;
                vp.height = win_desc.height;

                roa_renderer_set_device_viewport(app.renderer_ctx, &vp);
          
                app_tick(&app);
        }


        /* time to die */
        roa_renderer_ctx_destroy(&app.renderer_ctx);
        roa_ctx_destroy(&app.hw_ctx);

        return 0;
}


/* ----------------------------------------------------- [ App Functions ] -- */


void
load_objects(struct app *app)
{
        /* camera */
        struct roa_renderer_camera camera;
        ROA_MEM_ZERO(camera);
        camera.near_plane     = 0.1f;
        camera.far_plane      = 400.f;
        camera.field_of_view  = ROA_QUART_TAU * 0.125f;
        camera.position[2]    = +3.f;

        uint32_t camera_id = ++app->object_id_counter;

        roa_renderer_camera_set(app->renderer_ctx, &camera, camera_id);
  
        /* skip renderable load */
        return;

        /* renderable */
        int i;
        roa_renderer_mesh_list(app->renderer_ctx, ROA_NULL, &app->renderable_count);

        for(i = 0; i < app->renderable_count; ++i) {
                uint32_t obj_id = ++app->object_id_counter;

                roa_renderer_mesh_renderable_create(app->renderer_ctx, obj_id);

                float pos[3]; ROA_MEM_ZERO(pos);
                float scale[3]; scale[0] = 1.f; scale[1] = 1.f; scale[2] = 1.f;
                float rot[4]; ROA_MEM_ZERO(rot); rot[3] = 1.f;

                roa_renderer_mesh_renderable_transform_set(
                        app->renderer_ctx,
                        obj_id,
                        pos,
                        scale,
                        rot);
        }
}


void
load_resources(struct app *app)
{
        char import_file[2048];
        ROA_MEM_ZERO(import_file);
  
        //const char *path = "assets/renderer_test/cube.gltf";
        //const char *path = "assets/renderer_test/plane_trainer.gltf";
        const char *path = "assets/renderer_test/column_room.gltf";

        strcat(import_file, roa_exe_dir());
        strcat(import_file, path);

        roa_renderer_load(app->renderer_ctx, import_file, ROA_TRUE);

        /* test texture */
        struct roa_renderer_texture_resource texture_rsrc;
        ROA_MEM_ZERO(texture_rsrc);
        texture_rsrc.name = "TestTexture";

        unsigned bytes = 0;
        unsigned char *raw_png = texture_png_data_blender_1(&bytes);

        stbi_set_flip_vertically_on_load(1);
        texture_rsrc.data = stbi_load_from_memory(
                raw_png,
                bytes,
                &texture_rsrc.width,
                &texture_rsrc.height,
                &texture_rsrc.components,
                3);

        roa_renderer_texture_resource_add(
                app->renderer_ctx,
                &texture_rsrc);
}


void
setup_app(struct app *app)
{
        roa_ctx_create(&app->hw_ctx);
        roa_renderer_ctx_create(&app->renderer_ctx, ROA_NULL);

        struct roa_ctx_window_desc win_desc;
        ROA_MEM_ZERO(win_desc);
        win_desc.width = 1280;
        win_desc.height        = 720;
        win_desc.title         = "ROA Renderer Func Test";
        win_desc.capture_mouse = ROA_TRUE;

        roa_ctx_set_window_desc(app->hw_ctx, &win_desc);
}


