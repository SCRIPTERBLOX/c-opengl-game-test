#include "shaders.h"
#include <stdio.h>
#include <stdlib.h>
#include "../file_reading/read_shader.h"

char* triangle_vert_shader;
char* triangle_frag_shader;

void init_triangle_shader() {
    triangle_vert_shader = load_shader_source("shaders/triangle_vert.vs");
    triangle_frag_shader = load_shader_source("shaders/triangle_frag.fs");
    
    if (!triangle_vert_shader || !triangle_frag_shader) {
        fprintf(stderr, "Failed to load triangle shaders\n");
        exit(1);
    }
}

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
