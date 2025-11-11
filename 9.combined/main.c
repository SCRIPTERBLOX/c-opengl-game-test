/*
 * Copyright © 2011 Benjamin Franzke
 * Copyright © 2017 Joone Hur
 * Copyright © 2018 Joone Hur
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/common.h"
#include "../common/window.h"

// Triangle shader (simple)
const char* triangle_vert_shader =
    "attribute vec4 pos;\n"
    "void main() {\n"
    "  gl_Position = pos;\n"
    "}\n";

const char* triangle_frag_shader =
    "precision mediump float;\n"
    "void main() {\n"
    "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";

// Cube shader (with MVP matrix) - ES 2.0 compatible
const char* cube_vert_shader =
    "uniform mat4 u_mvpMatrix;                  \n"
    "attribute vec4 pos;                         \n"
    "attribute vec4 color;                       \n"
    "varying vec4 vVaryingColor;                \n"
    "void main()                                \n"
    "{                                          \n"
    "  gl_Position = u_mvpMatrix * pos;         \n"
    "  vVaryingColor = color;                   \n"
    "}";

const char* cube_frag_shader =
    "precision mediump float;                        \n"
    "varying vec4 vVaryingColor;                    \n"
    "void main()                                     \n"
    "{                                               \n"
    "   gl_FragColor = vVaryingColor;                \n"
    "}";

// Triangle vertices
static const GLfloat triangle_verts[] = {
    -0.8f, -0.5f, 0.0f,
    -0.3f, -0.5f, 0.0f,
    -0.55f, 0.5f, 0.0f
};

// Cube vertices
static const GLfloat cube_vertices[] = {
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

// Cube colors
static const GLfloat cube_colors[] = {
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
    // bottom: magenta
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f
};

GLuint cube_program;
GLint cube_pos_loc, cube_color_loc, cube_mvp_loc;

static GLuint create_shader_program(const char* vert_source, const char* frag_source) {
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

void init_cube_shader() {
    // Create cube shader program
    cube_program = create_shader_program(cube_vert_shader, cube_frag_shader);
    cube_pos_loc = glGetAttribLocation(cube_program, "pos");
    cube_color_loc = glGetAttribLocation(cube_program, "color");
    cube_mvp_loc = glGetUniformLocation(cube_program, "u_mvpMatrix");
}

void redraw(void* data, struct wl_callback* callback, uint32_t time) {
    struct window* window = data;
    static int i = 0;
    ESMatrix perspective, modelview;

    if (callback) {
        window->callback = NULL;
    }

    if (callback)
        wl_callback_destroy(callback);

    if (!window->configured)
        return;

    // Set viewport and clear
    glViewport(0, 0, window->geometry.width, window->geometry.height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Draw triangle (left side)
    glUseProgram(window->gl.program);
    glVertexAttribPointer(window->gl.pos, 3, GL_FLOAT, GL_FALSE, 0, triangle_verts);
    glEnableVertexAttribArray(window->gl.pos);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(window->gl.pos);

    // Draw cube (right side)
    glUseProgram(cube_program);
    
    // Set up perspective
    float aspect = (float)window->geometry.width / (float)window->geometry.height;
    esMatrixLoadIdentity(&perspective);
    esPerspective(&perspective, 45.0f, aspect, 1.0f, 20.0f);
    
    // Set up modelview for cube (positioned on the right)
    i++;
    esMatrixLoadIdentity(&modelview);
    esTranslate(&modelview, 2.0f, 0.0f, -6.0f);  // Move to the right
    esRotate(&modelview, 45.0f + (0.25f * i), 1.0f, 0.0f, 0.0f);
    esRotate(&modelview, 45.0f - (0.5f * i), 0.0f, 1.0f, 0.0f);
    esRotate(&modelview, 10.0f + (0.15f * i), 0.0f, 0.0f, 1.0f);

    ESMatrix mvpMatrix;
    esMatrixMultiply(&mvpMatrix, &modelview, &perspective);
    glUniformMatrix4fv(cube_mvp_loc, 1, GL_FALSE, (GLfloat*)&mvpMatrix.m[0][0]);

    // Set cube vertex attributes
    glVertexAttribPointer(cube_pos_loc, 3, GL_FLOAT, GL_FALSE, 0, cube_vertices);
    glEnableVertexAttribArray(cube_pos_loc);
    glVertexAttribPointer(cube_color_loc, 3, GL_FLOAT, GL_FALSE, 0, cube_colors);
    glEnableVertexAttribArray(cube_color_loc);

    // Draw cube faces
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

    glDisableVertexAttribArray(cube_pos_loc);
    glDisableVertexAttribArray(cube_color_loc);

    window->callback = wl_surface_frame(window->surface);
    wl_callback_add_listener(window->callback, &frame_listener, window);

    eglSwapBuffers(window->display->egl.dpy, window->egl_surface);
}

int main(int argc, char** argv) {
    struct sigaction sigint;
    struct display display = {0};
    struct window window = {0};
    int ret = 0;

    window.display = &display;
    display.window = &window;
    window.window_size.width = 800;
    window.window_size.height = 600;

    display.display = wl_display_connect(NULL);
    if (!display.display) {
        fprintf(stderr, "Failed to connect to Wayland display\n");
        return 1;
    }

    display.registry = wl_display_get_registry(display.display);
    wl_registry_add_listener(display.registry, &registry_listener, &display);

    wl_display_dispatch(display.display);

    init_egl(&display, window.opaque);
    create_surface(&window);
    
    // Initialize OpenGL ES context for triangle
    init_gl(&window, triangle_vert_shader, triangle_frag_shader);
    init_cube_shader();

    display.cursor_surface = wl_compositor_create_surface(display.compositor);

    sigint.sa_handler = signal_int;
    sigemptyset(&sigint.sa_mask);
    sigint.sa_flags = SA_RESETHAND;
    sigaction(SIGINT, &sigint, NULL);

    while (!window.configured && running && ret != -1) {
        ret = wl_display_dispatch(display.display);
    }

    if (window.configured && running) {
        redraw(&window, NULL, 0);
        window.callback = wl_surface_frame(window.surface);
        wl_callback_add_listener(window.callback, &frame_listener, &window);
    }

    while (running && ret != -1) {
        ret = wl_display_dispatch(display.display);
        wl_display_flush(display.display);
    }

    destroy_surface(&window);
    fini_egl(&display);

    wl_surface_destroy(display.cursor_surface);
    if (display.cursor_theme)
        wl_cursor_theme_destroy(display.cursor_theme);

    if (display.xdg_wm_base)
        xdg_wm_base_destroy(display.xdg_wm_base);

    if (display.compositor)
        wl_compositor_destroy(display.compositor);

    wl_registry_destroy(display.registry);
    wl_display_flush(display.display);
    wl_display_disconnect(display.display);

    return 0;
}