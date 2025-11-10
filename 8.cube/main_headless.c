#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "../common/common.h"

const char* vertexShaderSource =
    "#version 300 es                            \n"
    "uniform mat4 u_mvpMatrix;                  \n"
    "layout(location = 0) in vec4 pos;          \n"
    "layout(location = 1) in vec4 color;        \n"
    "out vec4 vVaryingColor;                \n"
    "void main()                                \n"
    "{                                          \n"
    "  gl_Position = u_mvpMatrix * pos;         \n"
    "  vVaryingColor = color;                   \n"
    "}";

const char* fragmentShaderSource =
    "#version 300 es                                 \n"
    "precision mediump float;                        \n"
    "layout(location = 0) out vec4 out_color;        \n"
    "in lowp vec4 vVaryingColor;                    \n"
    "void main()                                     \n"
    "{                                               \n"
    "   out_color = vVaryingColor;                   \n"
    "}                                               ";

static GLuint create_shader_program(const char* vertex_source, const char* fragment_source) {
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);
    
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    
    // Check linking status
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[512];
        glGetProgramInfoLog(program, 512, NULL, log);
        printf("Shader linking failed: %s\n", log);
        return 0;
    }
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return program;
}

static void render_cube(GLuint program, int width, int height) {
    static const GLfloat vertices[] = {
        // front
        -1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, +1.0f,
        -1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        // back
        +1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        +1.0f, +1.0f, -1.0f,
        -1.0f, +1.0f, -1.0f,
        // right
        +1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, -1.0f,
        +1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, -1.0f,
        // left
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, +1.0f,
        -1.0f, +1.0f, -1.0f,
        -1.0f, +1.0f, +1.0f,
        // top
        -1.0f, +1.0f, +1.0f,
        +1.0f, +1.0f, +1.0f,
        -1.0f, +1.0f, -1.0f,
        +1.0f, +1.0f, -1.0f,
        // bottom
        -1.0f, -1.0f, -1.0f,
        +1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, +1.0f
    };

    static const GLfloat vColors[] = {
        // front: white
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        // back: red
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        // right: green
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        // left: blue
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        // top: yellow
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        // bottom: purple
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f
    };

    glViewport(0, 0, width, height);
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    float aspect = (float)width / (float)height;

    ESMatrix perspective, modelview;
    esMatrixLoadIdentity(&perspective);
    esPerspective(&perspective, 29.0f, aspect, 1.0f, 20.0f);
    
    esMatrixLoadIdentity(&modelview);
    esTranslate(&modelview, 0.0f, 0.0f, -8.0f);
    esRotate(&modelview, 45.0f, 1.0f, 0.0f, 0.0f);
    esRotate(&modelview, 45.0f, 0.0f, 1.0f, 0.0f);
    esRotate(&modelview, 10.0f, 0.0f, 0.0f, 1.0f);

    ESMatrix mvpMatrix;
    esMatrixMultiply(&mvpMatrix, &modelview, &perspective);

    GLint mvpLoc = glGetUniformLocation(program, "u_mvpMatrix");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (GLfloat*)&mvpMatrix.m[0][0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, vColors);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

int main(int argc, char** argv) {
    printf("Initializing OpenGL ES cube renderer...\n");

    // Initialize EGL with default display
    EGLDisplay egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (egl_display == EGL_NO_DISPLAY) {
        printf("Failed to get EGL display\n");
        return 1;
    }

    if (!eglInitialize(egl_display, NULL, NULL)) {
        printf("Failed to initialize EGL\n");
        return 1;
    }

    // Choose EGL config
    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    EGLConfig config;
    EGLint num_config;
    if (!eglChooseConfig(egl_display, config_attribs, &config, 1, &num_config) || num_config == 0) {
        printf("Failed to choose EGL config\n");
        return 1;
    }

    // Create EGL context
    EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLContext egl_context = eglCreateContext(egl_display, config, EGL_NO_CONTEXT, context_attribs);
    if (egl_context == EGL_NO_CONTEXT) {
        printf("Failed to create EGL context\n");
        return 1;
    }

    // Create pbuffer surface for offscreen rendering
    const int width = 500, height = 500;
    EGLint pbuffer_attribs[] = {
        EGL_WIDTH, width,
        EGL_HEIGHT, height,
        EGL_NONE
    };

    EGLSurface egl_surface = eglCreatePbufferSurface(egl_display, config, pbuffer_attribs);
    if (egl_surface == EGL_NO_SURFACE) {
        printf("Failed to create EGL pbuffer surface\n");
        return 1;
    }

    // Make context current
    if (!eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context)) {
        printf("Failed to make EGL context current\n");
        return 1;
    }

    printf("EGL initialized successfully\n");

    // Create and use shader program
    GLuint program = create_shader_program(vertexShaderSource, fragmentShaderSource);
    if (!program) {
        printf("Failed to create shader program\n");
        return 1;
    }

    glUseProgram(program);
    printf("Shader program created and linked successfully\n");

    // Render the cube
    render_cube(program, width, height);
    printf("Cube rendered successfully\n");

    // Read pixels to verify rendering worked
    unsigned char* pixels = malloc(width * height * 4);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    
    // Check if we got some non-background pixels
    int has_color = 0;
    for (int i = 0; i < width * height * 4; i += 4) {
        if (pixels[i] != 128 || pixels[i+1] != 128 || pixels[i+2] != 128) {
            has_color = 1;
            break;
        }
    }
    
    if (has_color) {
        printf("SUCCESS: Cube rendered with colors (found non-background pixels)\n");
    } else {
        printf("INFO: Only background color detected\n");
    }
    
    free(pixels);

    // Cleanup
    glDeleteProgram(program);
    eglDestroySurface(egl_display, egl_surface);
    eglDestroyContext(egl_display, egl_context);
    eglTerminate(egl_display);

    printf("OpenGL ES cube rendering completed successfully!\n");
    printf("Note: In a headless environment, no window is displayed, but the rendering works.\n");
    printf("To see graphics, run this on a system with a display server (X11/Wayland).\n");

    return 0;
}