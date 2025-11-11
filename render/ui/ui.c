#include "../renderer.h"
#include "../shaders.h"
#include "../geometry.h"
#include "../../common/common.h"

void render_ui(void* data) {
  struct window* window = data;

  glUseProgram(window->gl.program);
  glVertexAttribPointer(window->gl.pos, 3, GL_FLOAT, GL_FALSE, 0, triangle_verts);
  glEnableVertexAttribArray(window->gl.pos);
  glDrawArrays(GL_TRIANGLES, 0, triangle_vert_count);
  glDisableVertexAttribArray(window->gl.pos);
}
