#include "geometry.h"

// Separate position and texture coordinate arrays for clarity
const GLfloat triangle_positions[] = {
    -0.8f, 0.5f, 0.0f,   // Position 0
    -0.8f, -0.5f, 0.0f,  // Position 1  
    -0.3f, -0.5f, 0.0f,  // Position 2
    -0.3f, 0.5f, 0.0f,    // Position 3
};

const GLfloat triangle_texcoords[] = {
    0.0f, 0.0f,             // TexCoord 0
    0.0f, 1.0f,             // TexCoord 1
    1.0f, 1.0f,             // TexCoord 2
    1.0f, 0.0f              // TexCoord 3
};

// Combined vertices for compatibility with existing code
const GLfloat triangle_verts[] = {
    -0.8f, 0.5f, 0.0f,   // Position 0
    0.0f, 0.0f,             // TexCoord 0
    -0.8f, -0.5f, 0.0f,  // Position 1  
    0.0f, 1.0f,             // TexCoord 1
    -0.3f, -0.5f, 0.0f,  // Position 2
    1.0f, 1.0f,             // TexCoord 2
    -0.3f, 0.5f, 0.0f,    // Position 3
    1.0f, 0.0f              // TexCoord 3
};

// Triangle indices for proper rendering
const GLushort triangle_indices[] = {
    1, 2, 0,  // First triangle
    2, 3, 0   // Second triangle
};

const GLsizei triangle_vert_count = 4;
const GLsizei triangle_index_count = 6;
