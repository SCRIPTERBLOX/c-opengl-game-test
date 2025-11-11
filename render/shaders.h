#ifndef SHADERS_H
#define SHADERS_H

#include <GLES2/gl2.h>

// Triangle shader sources
extern const char* triangle_vert_shader;
extern const char* triangle_frag_shader;

// Cube shader sources
extern const char* cube_vert_shader;
extern const char* cube_frag_shader;

// Shader utility functions
GLuint create_shader_program(const char* vert_source, const char* frag_source);

// Cube shader program and locations
extern GLuint cube_program;
extern GLint cube_pos_loc;
extern GLint cube_color_loc;
extern GLint cube_mvp_loc;

// Initialize cube shader
void init_cube_shader(void);

#endif // SHADERS_H