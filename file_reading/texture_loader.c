#include "texture_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <string.h>

GLuint load_texture_from_png(const char* filename) {
    // Simple, reliable PNG loader that creates RGB data
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Failed to open PNG file: %s\n", filename);
        return 0;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        return 0;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        return 0;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return 0;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    // Force conversion to RGB format
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png);
    }
    // Note: png_get_valid may not be available in all PNG versions
    // if (png_get_valid(png, info, PNG_INFO_tRNS)) {
    //     png_set_tRNS_to_alpha(png);
    // }
    if (bit_depth == 16) {
        png_set_strip_16(png);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png);
    }
    // Always strip alpha to ensure RGB format
    png_set_strip_alpha(png);

    // Update info after transformations
    png_read_update_info(png, info);

    // Get final values
    width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);
    int row_bytes = png_get_rowbytes(png, info);

    // Allocate single buffer for all pixel data (contiguous memory)
    unsigned char* pixel_data = (unsigned char*)malloc(row_bytes * height);
    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    
    // Set row pointers to point into the contiguous buffer
    int y;
    for (y = 0; y < height; y++) {
        row_pointers[y] = pixel_data + (y * row_bytes);
    }

    // Read image into contiguous buffer
    png_read_image(png, row_pointers);
    fclose(fp);

    // Create OpenGL texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Use tightly packed data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Always use RGB format (we stripped alpha)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // Set wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Clean up
    free(row_pointers);
    free(pixel_data);
    png_destroy_read_struct(&png, &info, NULL);

    return texture;
}