#include "renderer.h"
#include "shaders.h"
#include "../common/common.h"
#include "ui/ui.h"
#include "../file_reading/texture_loader.h"
#include <stdio.h>
#include <stdlib.h>

// Forward declaration
GLuint load_png_as_texture(const char* filename);

void renderer_init(struct window* window) {
    // Load texture shader from files
    init_texture_shader();
    
    // Initialize OpenGL ES context with texture shader
    init_gl(window, texture_vert_shader, texture_frag_shader);
    
    // Try to load PNG, fallback to simple texture
    window->gl.texture_id = load_png_as_texture("tex.png");
    if (window->gl.texture_id == 0) {
        fprintf(stderr, "Failed to create texture\n");
        exit(1);
    }
    
    // Store sampler location in window.gl
    window->gl.sampler = texture_sampler_loc;
    
    // Enable texturing
    glEnable(GL_TEXTURE_2D);
    
    // Set proper blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void redraw(void* data, struct wl_callback* callback, uint32_t time) {
  struct window* window = data;
  static int i = 0;
  ESMatrix perspective, modelview;

  if (callback) {
    window->callback = NULL;
  }

  if (callback)
    wl_callback_destroy(callback);

  if (!window->configured)
    return;

  // Set viewport and clear
  glViewport(0, 0, window->geometry.width, window->geometry.height);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);  // Enable proper face culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  render_ui(data);

  // Draw triangle (left side)
  //glUseProgram(window->gl.program);
  //glVertexAttribPointer(window->gl.pos, 3, GL_FLOAT, GL_FALSE, 0, triangle_verts);
  //glEnableVertexAttribArray(window->gl.pos);
  //glDrawArrays(GL_TRIANGLES, 0, triangle_vert_count);
  //glDisableVertexAttribArray(window->gl.pos);

  // Set up perspective
  float aspect = (float)window->geometry.width / (float)window->geometry.height;
  esMatrixLoadIdentity(&perspective);
  esPerspective(&perspective, 45.0f, aspect, 1.0f, 20.0f);
  
  // Set up modelview for cube (positioned on the right)
  i++;
  esMatrixLoadIdentity(&modelview);
  esTranslate(&modelview, 2.0f, 0.0f, -6.0f);  // Move to the right
  esRotate(&modelview, 45.0f + (0.25f * i), 1.0f, 0.0f, 0.0f);
  esRotate(&modelview, 45.0f - (0.5f * i), 0.0f, 1.0f, 0.0f);
  esRotate(&modelview, 10.0f + (0.15f * i), 0.0f, 0.0f, 1.0f);

  ESMatrix mvpMatrix;
  esMatrixMultiply(&mvpMatrix, &modelview, &perspective);
  glUniformMatrix4fv(cube_mvp_loc, 1, GL_FALSE, (GLfloat*)&mvpMatrix.m[0][0]);

  window->callback = wl_surface_frame(window->surface);
  wl_callback_add_listener(window->callback, &frame_listener, window);

  eglSwapBuffers(window->display->egl.dpy, window->egl_surface);
}
