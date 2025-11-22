#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <GLES2/gl2.h>

GLuint load_texture_from_png(const char* filename);
GLuint load_png_as_texture(const char* filename);

#endif // TEXTURE_LOADER_H