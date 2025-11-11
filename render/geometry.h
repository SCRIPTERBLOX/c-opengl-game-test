#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <GLES2/gl2.h>

// Triangle vertices - positioned on left side
extern const GLfloat triangle_verts[];
extern const GLsizei triangle_vert_count;

// Cube vertices - using individual triangles for each face
extern const GLfloat cube_vertices[];
extern const GLsizei cube_vert_count;

// Cube colors - matching the triangle layout
extern const GLfloat cube_colors[];
extern const GLsizei cube_color_count;

#endif // GEOMETRY_H