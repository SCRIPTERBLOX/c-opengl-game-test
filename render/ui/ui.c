#include "../renderer.h"
#include "../shaders.h"
#include "../geometry.h"
#include "../../common/common.h"
#include "../../file_reading/texture_loader.h"

void render_ui(void* data) {
  struct window* window = data;

  // Use texture shader program
  glUseProgram(window->gl.program);

  // Load vertex data (using hardcoded locations like simple_texture example)
  // Position attribute (location 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), triangle_verts);
  glEnableVertexAttribArray(0);
  
  // Texture coordinate attribute (location 1)
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), triangle_verts + 3);
  glEnableVertexAttribArray(1);

  // Bind texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, window->gl.texture_id);
  // Set the sampler texture unit to 0
  glUniform1i(window->gl.sampler, 0);

  // Pass texture dimensions to shader
  TextureInfo tex_info = get_current_texture_info();
  glUniform2f(texture_texture_size_loc, (float)tex_info.width, (float)tex_info.height);
  
  // Calculate geometry dimensions from vertex positions
  // The quad spans from x=-0.8 to x=-0.3 (width=0.5) and y=-0.5 to y=0.5 (height=1.0)
  glUniform2f(texture_geometry_size_loc, 0.5f, 1.0f);

  // Draw textured quad using indices (like simple_texture example)
  glDrawElements(GL_TRIANGLES, triangle_index_count, GL_UNSIGNED_SHORT, triangle_indices);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
