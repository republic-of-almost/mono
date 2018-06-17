#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/alloc.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/log.h>
#include <roa_lib/atomic.h>
#include <roa_lib/spin_lock.h>
#include <graphics_api/platform.h>
#include <common/alloc.h>


/* -------------------------------------------- [ Create/Destroy Helpers ] -- */


void
setup_and_lock(
        struct roa_renderer_ctx *new_ctx)
{
        ROA_ASSERT(new_ctx);

        roa_spin_lock_init(&new_ctx->device_settings.lock);
        roa_spin_lock_aquire(&new_ctx->device_settings.lock);

        roa_spin_lock_init(&new_ctx->renderer_desc.lock);
        roa_spin_lock_aquire(&new_ctx->renderer_desc.lock);

        roa_spin_lock_init(&new_ctx->device_settings.lock);
        roa_spin_lock_aquire(&new_ctx->device_settings.lock);

        roa_spin_lock_init(&new_ctx->renderpass.lock);
        roa_spin_lock_aquire(&new_ctx->renderpass.lock);

        roa_spin_lock_init(&new_ctx->resource_desc.lock);
        roa_spin_lock_aquire(&new_ctx->resource_desc.lock);
}


void
setup_and_release_device_settings(
        struct device_setting_data *dev_set)
{
        /* param check */
        ROA_ASSERT(dev_set);

        roa_spin_lock_init(&dev_set->lock);

        dev_set->device_viewport[0] = 800;
        dev_set->device_viewport[1] = 480;

        roa_spin_lock_release(&dev_set->lock);
}


void
lock_and_destroy_device_settings(
        struct device_setting_data *dev_set)
{
        /* param check */
        ROA_ASSERT(dev_set);

        roa_spin_lock_aquire(&dev_set->lock);
}


void
setup_and_release_renderer_desc(
        struct renderer_data_desc *rdr_data)
{
        /* param check */
        ROA_ASSERT(rdr_data);

        /* create */
        roa_array_create_with_capacity(rdr_data->camera_ids, 5);
        roa_array_create_with_capacity(rdr_data->camera_descs, 5);

        roa_array_create_with_capacity(rdr_data->mesh_rdr_ids, 256);
        roa_array_create_with_capacity(rdr_data->mesh_rdr_descs, 256);

        roa_spin_lock_release(&rdr_data->lock);
}


void
lock_and_destroy_renderer_desc(
        struct renderer_data_desc *rdr_data)
{
        /* param check */
        ROA_ASSERT(rdr_data);

        roa_spin_lock_aquire(&rdr_data->lock);

        /* destroy */
        roa_array_destroy(rdr_data->camera_ids);
        roa_array_destroy(rdr_data->camera_descs);

        roa_array_destroy(rdr_data->mesh_rdr_ids);
        roa_array_destroy(rdr_data->mesh_rdr_descs);
}


void
setup_and_release_resource_desc(
        struct renderer_resource_data_desc *rsrc_data)
{
        /* param check */
        ROA_ASSERT(rsrc_data);

        /* create */
        roa_array_create_with_capacity(rsrc_data->material_ids, 32);
        roa_array_create_with_capacity(rsrc_data->mat_descs, 32);

        roa_array_create_with_capacity(rsrc_data->mesh_ids, 32);
        roa_array_create_with_capacity(rsrc_data->mesh_rsrc_data, 32);

        roa_array_create_with_capacity(rsrc_data->mesh_pending_ids, 32);
        roa_array_create_with_capacity(rsrc_data->mesh_rsrc_pending_data, 32);

        roa_array_create_with_capacity(rsrc_data->texture_ids, 32);
        roa_array_create_with_capacity(rsrc_data->texture_rsrc_data, 32);

        roa_array_create_with_capacity(rsrc_data->texture_pending_ids, 32);
        roa_array_create_with_capacity(rsrc_data->texture_rsrc_pending_data, 32);

        roa_spin_lock_release(&rsrc_data->lock);
}


void
lock_and_destroy_resource_desc(
        struct renderer_resource_data_desc *rsrc_data)
{
        /* param check */
        ROA_ASSERT(rsrc_data);

        roa_spin_lock_aquire(&rsrc_data->lock);

        /* destroy */
        roa_array_destroy(rsrc_data->material_ids);
        roa_array_destroy(rsrc_data->mat_descs);

        roa_array_destroy(rsrc_data->mesh_ids);
        roa_array_destroy(rsrc_data->mesh_rsrc_data);

        roa_array_destroy(rsrc_data->mesh_pending_ids);
        roa_array_destroy(rsrc_data->mesh_rsrc_pending_data);

        roa_array_destroy(rsrc_data->texture_ids);
        roa_array_destroy(rsrc_data->texture_rsrc_data);

        roa_array_destroy(rsrc_data->texture_pending_ids);
        roa_array_destroy(rsrc_data->texture_rsrc_pending_data);
}


void
setup_and_release_renderpass(
        struct renderer_renderpass_data *rps)
{
        /* param check */
        ROA_ASSERT(rps);

        /* create */
        roa_array_create_with_capacity(rps->rps, 4);

roa_spin_lock_release(&rps->lock);
}


void
lock_and_destroy_renderpass(
  struct renderer_renderpass_data *rps)
{
        /* param check */
        ROA_ASSERT(rps);

        roa_spin_lock_aquire(&rps->lock);

        /* destroy */
        roa_array_destroy(rps->rps);
}


/* ---------------------------------------------------------- [ Lifetime ] -- */


ROA_BOOL
roa_renderer_ctx_create(
        roa_renderer_ctx_t *ctx,
        struct roa_renderer_ctx_desc *desc)
{
        (void)desc;

        /* param check */
        ROA_ASSERT(ctx);

        /* new ctx */
        struct roa_renderer_ctx *new_ctx = rdr_alloc(sizeof(*new_ctx));
        ROA_ASSERT(new_ctx);

        if(!new_ctx) {
                return ROA_FALSE;
        }

        /* create internal data */
        setup_and_lock(new_ctx);
        setup_and_release_device_settings(&new_ctx->device_settings);
        setup_and_release_renderer_desc(&new_ctx->renderer_desc);
        setup_and_release_resource_desc(&new_ctx->resource_desc);
        setup_and_release_renderpass(&new_ctx->renderpass);

        /* platform */
        platform_setup(new_ctx);

        *ctx = new_ctx;

        return ROA_TRUE;
}


void
roa_renderer_ctx_execute(
        roa_renderer_ctx_t ctx)
{
        platform_update(ctx);
        platform_render(ctx);
}


void
roa_renderer_ctx_destroy(
        roa_renderer_ctx_t *ctx)
{
        /* param check */
        ROA_ASSERT(ctx);
        ROA_ASSERT(*ctx);

        /* ctx to kill */
        struct roa_renderer_ctx *kill_ctx = *ctx;

        /* destroy internal data */
        lock_and_destroy_resource_desc(&kill_ctx->resource_desc);
        lock_and_destroy_renderer_desc(&kill_ctx->renderer_desc);
        lock_and_destroy_device_settings(&kill_ctx->device_settings);

        rdr_free(kill_ctx);
        *ctx = ROA_NULL;
}

