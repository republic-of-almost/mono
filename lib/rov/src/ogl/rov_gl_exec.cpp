#include "rov_gl_exec.hpp"
#include "rov_gl.hpp"
#include <lib/bits.hpp>
#include <lib/color.hpp>
#include <lib/bench.hpp>
#include <lib/assert.hpp>
#include <lib/logging.hpp>
#include <math/mat/mat4.hpp>


namespace ROV_Internal {


/*
  OpenGL Exec
  --
  This function takes ROV Data and transforms it to OpenGL calls.
  OpenGL resouces should be created by this point.
*/
void
ogl_exec(
  rovGLData *rov_gl_data,
  rovData *rov_data)
{
  BENCH_SCOPED_CPU(OglExec);

  #ifdef GL_HAS_VAO
  // We only deal with one VAO unless we can definitivly say its worth it. //
  glBindVertexArray(rov_gl_data->vao);
  #endif

  // This should move in to rasterizer settings etc in ROV Data. //
  glUseProgram(0);
  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  
  /*
    For each renderpass.
  */
  const size_t pass_count = rov_data->rov_render_passes.size();
  const rovRenderPass *passes = rov_data->rov_render_passes.data();
  
  for(size_t p = 0; p < pass_count; ++p)
  {
    const rovRenderPass &rp = passes[p];
  
    GLbitfield cl_flags = 0;
    if(rp.clear_flags & rovClearFlag_Color) { cl_flags |= GL_COLOR_BUFFER_BIT; }
    if(rp.clear_flags & rovClearFlag_Depth) { cl_flags |= GL_DEPTH_BUFFER_BIT; }

    const float cl_color[4]
    {
      lib::color::get_channel_1f(rp.clear_color),
      lib::color::get_channel_2f(rp.clear_color),
      lib::color::get_channel_3f(rp.clear_color),
      lib::color::get_channel_4f(rp.clear_color),
    };

    // HACK
    if (rp.render_target == 0)
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glViewport(rp.viewport[0], rp.viewport[1], rp.viewport[2], rp.viewport[3]);
    }
    else
    {
      rovGLFramebuffer fb = rov_gl_data->rov_frame_buffers[rp.render_target - 1];

      cl_flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

      glEnable(GL_MULTISAMPLE);
      glBindFramebuffer(GL_FRAMEBUFFER, fb.fbo);
      glViewport(0, 0, fb.width, fb.height);
    }

    glClearColor(cl_color[0], cl_color[1], cl_color[2], cl_color[3]);
    glClear(cl_flags);
    glEnable(GL_DEPTH_TEST);

    // Should this live in in ROV Data layer?
    const math::mat4 proj = math::mat4_init(rp.proj);
    const math::mat4 view = math::mat4_init(rp.view);
    const math::mat4 view_proj = math::mat4_multiply(view, proj);

    /*
      For each material in the pass.
    */
    size_t dc_index = 0;
    
    BENCH_PUSH_GPU(MeshRender); 
    
    GLuint last_shd = 0;

    const size_t mat_count = rp.materials.size();

    for(size_t k = 0; k < mat_count; ++k)
//    for(auto &mat : rp.materials)
    {
      auto &mat = rp.materials[k];
    
      if(mat.draw_calls == 0)
      {
        continue;
      }
    
      // Pull Material Info Out
      const uint32_t color   = lib::bits::upper32(mat.material);

      const float colorf[4] {
        math::to_float(lib::bits::first8(color)) / 255.f,
        math::to_float(lib::bits::second8(color)) / 255.f,
        math::to_float(lib::bits::third8(color)) / 255.f,
        math::to_float(lib::bits::forth8(color)) / 255.f,
      };

      const uint32_t details = lib::bits::lower32(mat.material);

      const uint8_t shader     = rovShader_Lit; /* We now use the lit for everything */ //lib::bits::first8(details);
      const uint8_t texture_01 = lib::bits::second8(details);
      const uint8_t texture_02 = lib::bits::third8(details);
      const uint8_t texture_03 = lib::bits::forth8(details);
      
      const rovGLMeshProgram shd = rov_gl_data->rov_mesh_programs[shader];
      

      /*
        Bind the shader
      */
      if(shd.program != last_shd)
      {
        last_shd = shd.program;
        glUseProgram(shd.program);
      }
      
      /*
        Load the textures and buffers
      */
      {
        uint32_t texture_slots = 0;
        
        /*
          Texture Maps
        */
        const uint8_t texture_maps[] = { texture_01, texture_02, texture_03 };
        
        for(uint8_t t = 0; t < rov_max_texture_maps; ++t)
        {
          if(texture_maps[t] && shd.uni_tex[t] != -1)
          {
            const size_t texture_index = texture_maps[t]  - 1;

            if(texture_index < rov_gl_data->rov_textures.size())
            {
              const rovGLTexture tex = rov_gl_data->rov_textures[texture_index];
              
              glActiveTexture(GL_TEXTURE0 + texture_slots);
              glBindTexture(GL_TEXTURE_2D, tex.gl_id);
              
              glUniform1i(shd.uni_tex[t], texture_slots);
              
              ++texture_slots;
            }
          }
        }
        
        /*
          Buffers
        */
        if(shd.uni_buffer_01 != -1)
        {
          const GLuint slot = texture_slots + 1; // Unsure why, if I bind to slot 0 we get 1282's.
          
          const size_t l_index = rp.light_buffer - 1;
          const rovGLLightPack light_pack = rov_gl_data->light_buffers[l_index];
          
          glUniform1i(shd.uni_light_count, light_pack.count);
          
          glActiveTexture(GL_TEXTURE0 + slot);
          glBindTexture(GL_TEXTURE_1D, light_pack.gl_id);
          
          glUniform1i(shd.uni_buffer_01, slot);
          
          ++texture_slots;
        }
      }
      
      /*
        For each draw call in the material.
      */
      for(uint32_t i = 0; i < mat.draw_calls; ++i)
      {
        auto &dc = rp.draw_calls[dc_index++];

        rovGLMesh vbo;
        
        if(dc.mesh > 0 && dc.mesh < rov_gl_data->rov_meshes.size())
        {
          vbo = rov_gl_data->rov_meshes[dc.mesh - 1];
          glBindBuffer(GL_ARRAY_BUFFER, vbo.gl_id);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
        {
          vbo = rovGLMesh{};
          glBindBuffer(GL_ARRAY_BUFFER, 0);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
          
          continue;
        }
        
        /*
          Load input format
        */
        {
          size_t pointer = 0;

          if(shd.vs_in_pos >= 0)
          {
            glEnableVertexAttribArray(shd.vs_in_pos);
            glVertexAttribPointer(
              shd.vs_in_pos,
              3,
              GL_FLOAT,
              GL_FALSE,
              8 * sizeof(float),
              (void*)pointer
            );
          }

          pointer += (sizeof(float) * 3);

          if(shd.vs_in_norm >= 0)
          {
            glEnableVertexAttribArray(shd.vs_in_norm);
            glVertexAttribPointer(
              shd.vs_in_norm,
              3,
              GL_FLOAT,
              GL_FALSE,
              8 * sizeof(float),
              (void*)pointer
            );
          }

          pointer += (sizeof(float) * 3);

          if(shd.vs_in_uv)
          {
            glEnableVertexAttribArray(shd.vs_in_uv);
            glVertexAttribPointer(
              shd.vs_in_uv,
              2,
              GL_FLOAT,
              GL_FALSE,
              8 * sizeof(float),
              (void*)pointer
            );
          }
        }
        
        /*
          Apply Matrices and other uniforms.
        */
        {
          const math::mat4 world = math::mat4_init(dc.world);

          const math::mat4 wvp_mat  = math::mat4_multiply(world, view, proj);
//          const math::vec4 pos      = math::mat4_multiply(math::vec4_init(0,0,0,1), math::mat4_get_inverse(view));
          const math::mat4 mv_mat   = math::mat4_multiply(world, view);
          const math::mat4 norm_mat = math::mat4_get_transpose(math::mat4_get_inverse(mv_mat));
  
          glUniformMatrix4fv(shd.uni_wvp,     1, GL_FALSE, math::mat4_get_data(wvp_mat));
          glUniformMatrix4fv(shd.uni_view,    1, GL_FALSE, math::mat4_get_data(view));
          glUniformMatrix4fv(shd.uni_world,   1, GL_FALSE, math::mat4_get_data(world));
          glUniformMatrix4fv(shd.uni_normal,  1, GL_FALSE, math::mat4_get_data(norm_mat));
          glUniformMatrix4fv(shd.uni_wv,      1, GL_FALSE, math::mat4_get_data(mv_mat));
          glUniform3fv(shd.uni_eye,           1, rp.eye_position);
          glUniform4fv(shd.uni_color,         1, colorf);
        }
        
        /*
          Draw
        */
        glDrawArrays(GL_TRIANGLES, 0, vbo.vertex_count);
        
        /*
          Error Check
        */
        const GLuint err = glGetError();
        if(err)
        {
          LOG_ERROR("GL Error - Mesh Drawing")
        }
      }
    } // For amts
    
    BENCH_POP_GPU;

    // Line Renderer
    #ifdef GL_HAS_GEO_SHD
    {
//      BENCH_SCOPED_GPU(DebugLineRender);
    
      glUseProgram(rov_gl_data->rov_line_programs[0].program);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      
      // -- Draw the Batches Of Lines -- //
      const size_t batches = (rp.line_draw_calls.size() / 32) + 1;

      for(size_t i = 0; i < batches; ++i)
      {
        // -- Load Camera -- //
        glUniformMatrix4fv(
          rov_gl_data->rov_line_programs[0].uni_wvp,
          1,
          GL_FALSE,
          math::mat4_get_data(view_proj)
        );

        const size_t count(
          ((i + 1) == batches) ?
            rp.line_draw_calls.size() % 32 : 32
        );

        const uint32_t batch_start = i * 32;

        glUniform3fv(
          rov_gl_data->rov_line_programs[0].uni_line_buffer,
          count * 3,
          (float*)&rp.line_draw_calls[batch_start]
        );

        glDrawArrays(GL_POINTS, 0, count);
      }
    }
    #endif

    if (rp.render_target != 0)
    {
      const rovGLFramebuffer fb = rov_gl_data->rov_frame_buffers[rp.render_target - 1];

      glDisable(GL_MULTISAMPLE);

      glBindFramebuffer(GL_READ_FRAMEBUFFER, fb.fbo);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.resolve_fbo);

      glBlitFramebuffer(
        0, 0,
        fb.width, fb.height,
        0, 0,
        fb.width, fb.height,
        GL_COLOR_BUFFER_BIT,
        GL_LINEAR
      );

      glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
  }

  #ifdef GL_HAS_VAO
  glBindVertexArray(0);
  #endif
}


} // ns
