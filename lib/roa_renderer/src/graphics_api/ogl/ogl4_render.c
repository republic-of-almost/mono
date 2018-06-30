#ifdef ROA_RENDERER_API_GL4


#include <graphics_api/platform.h>
#include <graphics_api/ogl/ogl4_helpers.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>
#include <roa_math/math.h>
#include <GL/gl3w.h>
#include <string.h>
#include <stdio.h>


void
platform_render(roa_renderer_ctx_t ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  struct graphics_api *gfx_api = &ctx->graphics_api;

  /* setup */
  {
    glBindVertexArray(gfx_api->vao);

    GLsizei vp_width = ctx->device_settings.device_viewport[0];
    GLsizei vp_height = ctx->device_settings.device_viewport[1];
    glViewport(0, 0, vp_width, vp_height);
  }

  if(OGL4_ERROR_CHECKS)
  {
    GLuint err = glGetError();
    if(err) {
      ROA_ASSERT(0);
    }
  }

  /* renderpasses */
  int rp_count = roa_array_size(ctx->renderer_desc.camera_ids);
  int i;

  for (i = 0; i < rp_count; ++i)
  {
    struct renderpass *rp = &ctx->renderer_desc.camera_passes[i];
    int dc_count = roa_array_size(rp->draw_calls);

    /* fill buffer */
    {
      char buffer[128];
      memset(buffer, 0, sizeof(buffer));
      sprintf(buffer, "GBuffer:Fill - Cam %d - DCs %d", i, dc_count);

      glrPushMarkerGroup(buffer);

      struct ogl_gbuffer gbuffer = gfx_api->gbuffer;
      struct ogl_gbuffer_fill_pass fill = gfx_api->gbuffer_fill;

      glUseProgram(fill.program);

      glEnable(GL_DEPTH_TEST);
      glEnable(GL_CULL_FACE);

      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gbuffer.fbo);

      GLenum draw_buffers[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3,
      };

      glDrawBuffers(ROA_ARR_COUNT(draw_buffers), draw_buffers);

      glClearColor(1, 0, 1, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      /* draw calls */
      int j;
      for (j = 0; j < dc_count; ++j)
      {
        struct renderpass_draw_call dc = rp->draw_calls[j];
        
        struct ogl_mesh *mesh = &ctx->graphics_api.meshes[dc.mesh_index];
        
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);

        /* input format */
        int k;
        int input_count = ROA_ARR_COUNT(gfx_api->gbuffer_fill.input);
        for(k = 0; k < input_count; ++k)
        {
          struct ogl_vertex_input input = gfx_api->gbuffer_fill.input[k];

          if(input.loc < 0)
          {
            continue;
          }

          glEnableVertexAttribArray(input.loc);
          glVertexAttribPointer(
            input.loc,
            input.component_count,
            GL_FLOAT,
            input.normalize,
            input.size,
            input.ptr);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gfx_api->tex);
        glUniform1i(fill.uni_diffuse, 0);

        glUniformMatrix4fv(fill.uni_wvp, 1, GL_FALSE, dc.wvp);
        glUniformMatrix4fv(fill.uni_world, 1, GL_FALSE, dc.world);


        if(ctx->graphics_api.meshes[0].ibo)
        {
          GLsizei count = ctx->graphics_api.meshes[0].index_count;
          GLenum type = ctx->graphics_api.meshes[0].index_type;

          glDrawElements(GL_TRIANGLES, count, type, 0);
        }
        else
        {
          GLsizei count = ctx->resource_desc.mesh_rsrc_data[0].vertex_count;
          glDrawArrays(GL_TRIANGLES, 0, count);
        }
      }

      roa_array_destroy(rp->draw_calls);

      glrPopMarkerGroup();
    }
    
    if(OGL4_ERROR_CHECKS)
    {
      GLuint err = glGetError();
      if(err) {
        //ROA_ASSERT(0);
      }
    }

    glFinish();

    /* decals */
    {
      glrPushMarkerGroup("Decals");

      glUseProgram(gfx_api->decal.program);

      glDisable(GL_DEPTH_TEST);
      glDisable(GL_STENCIL_TEST);

      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gfx_api->gbuffer.fbo);

      GLenum draw_buffers[] = {
        //GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        //GL_COLOR_ATTACHMENT2,
        //GL_COLOR_ATTACHMENT3,
      };

      glDrawBuffers(ROA_ARR_COUNT(draw_buffers), draw_buffers);

      glBindBuffer(GL_ARRAY_BUFFER, gfx_api->decal.vbo);

      /* input format */
      int k;
      int input_count = ROA_ARR_COUNT(gfx_api->decal.input);
      for(k = 0; k < input_count; ++k)
      {
        struct ogl_vertex_input input = gfx_api->decal.input[k];

        if(input.loc < 0) {
          continue;
        }

        glEnableVertexAttribArray(input.loc);
        glVertexAttribPointer(
          input.loc,
          input.component_count,
          GL_FLOAT,
          input.normalize,
          input.size,
          input.ptr);
      }

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, gfx_api->gbuffer.texture_output[0]);
      glUniform1i(gfx_api->decal.uni_world_pos, 0);

      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, gfx_api->gbuffer.texture_depth);
      glUniform1i(gfx_api->decal.uni_depth, 1);

      int decal_count = roa_array_size(rp->decals);
      struct decal_transform *decals = rp->decals;

      for(k = 0; k < decal_count; ++k)
      {
              if (rp->decal_ids[k] <= 0) {
                      continue;
              }

        struct decal_transform *decal = &decals[k];

        /* sort */
        roa_mat4 world, inv_world;

        roa_mat4_import(&world, decal->world_mat);

        roa_mat4_inverse(&inv_world, &world);

        roa_mat4 view_proj, inv_view_proj, view, proj;
        roa_mat4_import(&view_proj, rp->camera_view_projection);

        roa_mat4_import(&proj, rp->camera_projection);
        roa_mat4_import(&view, rp->camera_view);

        roa_mat4 inv_view, inv_proj;
        roa_mat4_inverse(&inv_view, &view);
        roa_mat4_inverse(&inv_proj, &proj);

        roa_mat4_multiply(&inv_view_proj, &inv_proj, &inv_view);

        roa_mat4 wvp;
        roa_mat4_multiply(&wvp, &world, &view_proj);

        glUniform3fv(gfx_api->decal.uni_color, 1, decal->color);

        glUniformMatrix4fv(gfx_api->decal.uni_view, 1, GL_FALSE, rp->camera_view);
        glUniformMatrix4fv(gfx_api->decal.uni_proj, 1, GL_FALSE, rp->camera_projection);
        glUniformMatrix4fv(gfx_api->decal.uni_world, 1, GL_FALSE, world.data);
        glUniformMatrix4fv(gfx_api->decal.uni_inv_projview, 1, GL_FALSE, inv_view_proj.data);
        glUniformMatrix4fv(gfx_api->decal.uni_inv_world, 1, GL_TRUE, inv_world.data);

        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      glrPopMarkerGroup();
    }
  } /* rps */

  if(OGL4_ERROR_CHECKS)
  {
    GLuint err = glGetError();
    if(err) {
      ROA_ASSERT(0);
    }
  }

  glFinish();

  /* blit to screen */
  {
    glrPushMarkerGroup("Backbuffer:Blit");

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glUseProgram(ctx->graphics_api.blit.program);

    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, gfx_api->blit.fullscreen_triangle);

    /* input format */
    int k;
    int input_count = ROA_ARR_COUNT(gfx_api->blit.input);
    for (k = 0; k < input_count; ++k)
    {
      struct ogl_vertex_input input = gfx_api->blit.input[k];

      if (input.loc < 0) {
        continue;
      }

      glEnableVertexAttribArray(input.loc);
      glVertexAttribPointer(
        input.loc,
        input.component_count,
        GL_FLOAT,
        input.normalize,
        input.size,
        input.ptr);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gfx_api->gbuffer.texture_output[1]);
    glUniform1i(gfx_api->blit.uni_blit_src, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glrPopMarkerGroup();
  }

  if(OGL4_ERROR_CHECKS)
  {
    GLuint err = glGetError();
    if(err) {
      ROA_ASSERT(0);
    }
  }
}

#endif /* ROA_RENDERER_API_GL4 */
