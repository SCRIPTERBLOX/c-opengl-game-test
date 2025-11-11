#include "geometry.h"

// Triangle vertices - positioned on left side
const GLfloat triangle_verts[] = {
    -0.8f, -0.5f, 0.0f,
    -0.3f, -0.5f, 0.0f,
    -0.55f, 0.5f, 0.0f
};

const GLsizei triangle_vert_count = sizeof(triangle_verts) / sizeof(GLfloat) / 3;

// Cube vertices - using individual triangles for each face
const GLfloat cube_vertices[] = {
    // Front face (2 triangles)
    -1.0f, -1.0f,  1.0f,   // bottom-left
     1.0f, -1.0f,  1.0f,   // bottom-right
     1.0f,  1.0f,  1.0f,   // top-right
    -1.0f, -1.0f,  1.0f,   // bottom-left
     1.0f,  1.0f,  1.0f,   // top-right
    -1.0f,  1.0f,  1.0f,   // top-left
    
    // Back face (2 triangles)
     1.0f, -1.0f, -1.0f,   // bottom-right
    -1.0f, -1.0f, -1.0f,   // bottom-left
    -1.0f,  1.0f, -1.0f,   // top-left
     1.0f, -1.0f, -1.0f,   // bottom-right
    -1.0f,  1.0f, -1.0f,   // top-left
     1.0f,  1.0f, -1.0f,   // top-right
    
    // Right face (2 triangles)
     1.0f, -1.0f,  1.0f,   // bottom-front
     1.0f, -1.0f, -1.0f,   // bottom-back
     1.0f,  1.0f, -1.0f,   // top-back
     1.0f, -1.0f,  1.0f,   // bottom-front
     1.0f,  1.0f, -1.0f,   // top-back
     1.0f,  1.0f,  1.0f,   // top-front
    
    // Left face (2 triangles)
    -1.0f, -1.0f, -1.0f,   // bottom-back
    -1.0f, -1.0f,  1.0f,   // bottom-front
    -1.0f,  1.0f,  1.0f,   // top-front
    -1.0f, -1.0f, -1.0f,   // bottom-back
    -1.0f,  1.0f,  1.0f,   // top-front
    -1.0f,  1.0f, -1.0f,   // top-back
    
    // Top face (2 triangles)
    -1.0f,  1.0f,  1.0f,   // left-front
     1.0f,  1.0f,  1.0f,   // right-front
     1.0f,  1.0f, -1.0f,   // right-back
    -1.0f,  1.0f,  1.0f,   // left-front
     1.0f,  1.0f, -1.0f,   // right-back
    -1.0f,  1.0f, -1.0f,   // left-back
    
    // Bottom face (2 triangles)
    -1.0f, -1.0f, -1.0f,   // left-back
     1.0f, -1.0f, -1.0f,   // right-back
     1.0f, -1.0f,  1.0f,   // right-front
    -1.0f, -1.0f, -1.0f,   // left-back
     1.0f, -1.0f,  1.0f,   // right-front
    -1.0f, -1.0f,  1.0f    // left-front
};

const GLsizei cube_vert_count = sizeof(cube_vertices) / sizeof(GLfloat) / 3;

// Cube colors - matching the new triangle layout
const GLfloat cube_colors[] = {
    // Front face (6 vertices for 2 triangles)
    1.0f, 1.0f, 1.0f,  // white
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    // Back face (6 vertices)
    1.0f, 0.0f, 0.0f,  // red
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    // Right face (6 vertices)
    0.0f, 1.0f, 0.0f,  // green
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    // Left face (6 vertices)
    0.0f, 0.0f, 1.0f,  // blue
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    // Top face (6 vertices)
    1.0f, 1.0f, 0.0f,  // yellow
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    // Bottom face (6 vertices)
    1.0f, 0.0f, 1.0f,  // magenta
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f
};

const GLsizei cube_color_count = sizeof(cube_colors) / sizeof(GLfloat) / 3;