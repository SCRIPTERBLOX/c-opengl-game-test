#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>

char* load_shader_source(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Failed to open shader file: %s\n", filepath);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* source = malloc(length + 1);
    if (!source) {
        fprintf(stderr, "Failed to allocate memory for shader source\n");
        fclose(file);
        return NULL;
    }
    
    fread(source, 1, length, file);
    source[length] = '\0';
    fclose(file);
    
    return source;
}
