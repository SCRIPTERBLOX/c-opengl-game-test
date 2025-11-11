#include "shaders.h"
#include <stdio.h>
#include <stdlib.h>

// Triangle shader (simple)
const char* triangle_vert_shader =
    "attribute vec4 pos;\n"
    "varying vec4 vPosition;\n"
    "void main() {\n"
    "  gl_Position = pos;\n"
    "  vPosition = pos;\n"
    "}\n";

const char* triangle_frag_shader =
    "precision mediump float;\n"
    "varying vec4 vPosition;\n"
    "void main() {\n"
    "  // Use position to influence color\n"
    "  float r = (vPosition.x + 1.0) * 0.5;  // Map -1 to 1 -> 0 to 1\n"
    "  float g = (vPosition.y + 1.0) * 0.5;  // Map -1 to 1 -> 0 to 1\n"
    "  float b = (vPosition.z + 1.0) * 0.5;  // Map -1 to 1 -> 0 to 1\n"
    "  gl_FragColor = vec4(r, g, b, 1.0);\n"
    "}\n";

// Cube shader (with MVP matrix) - ES 2.0 compatible
const char* cube_vert_shader =
    "uniform mat4 u_mvpMatrix;                  \n"
    "attribute vec4 pos;                         \n"
    "attribute vec4 color;                       \n"
    "varying vec4 vVaryingColor;                \n"
    "void main() {                               \n"
    "  gl_Position = u_mvpMatrix * pos;         \n"
    "  vVaryingColor = color;                   \n"
    "}";

const char* cube_frag_shader =
    "precision mediump float;                        \n"
    "varying vec4 vVaryingColor;                    \n"
    "void main() {                                  \n"
    "   gl_FragColor = vVaryingColor;                \n"
    "}";

// Cube shader program and locations
GLuint cube_program;
GLint cube_pos_loc, cube_color_loc, cube_mvp_loc;

GLuint create_shader_program(const char* vert_source, const char* frag_source) {
    GLuint vert_shader, frag_shader, program;
    GLint status;
    
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_source, NULL);
    glCompileShader(vert_shader);
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char log[1000];
        glGetShaderInfoLog(vert_shader, 1000, NULL, log);
        fprintf(stderr, "Vertex shader compile error: %s\n", log);
        exit(1);
    }
    
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_source, NULL);
    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char log[1000];
        glGetShaderInfoLog(frag_shader, 1000, NULL, log);
        fprintf(stderr, "Fragment shader compile error: %s\n", log);
        exit(1);
    }
    
    program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        char log[1000];
        glGetProgramInfoLog(program, 1000, NULL, log);
        fprintf(stderr, "Program link error: %s\n", log);
        exit(1);
    }
    
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    
    return program;
}

void init_cube_shader(void) {
    // Create cube shader program
    cube_program = create_shader_program(cube_vert_shader, cube_frag_shader);
    cube_pos_loc = glGetAttribLocation(cube_program, "pos");
    cube_color_loc = glGetAttribLocation(cube_program, "color");
    cube_mvp_loc = glGetUniformLocation(cube_program, "u_mvpMatrix");
}