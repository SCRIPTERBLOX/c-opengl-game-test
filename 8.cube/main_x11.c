#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "../common/common.h"

struct x11_display {
    Display* x_display;
    Window x_window;
    EGLDisplay egl_display;
    EGLContext egl_context;
    EGLSurface egl_surface;
    Atom wm_delete_window;
    int running;
};

static struct x11_display x11 = {0};

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

static void handle_signal(int signum) {
    x11.running = 0;
}

static int init_x11_window(int width, int height) {
    x11.x_display = XOpenDisplay(NULL);
    if (!x11.x_display) {
        fprintf(stderr, "Failed to open X display\n");
        return 0;
    }

    int screen = DefaultScreen(x11.x_display);
    Window root = RootWindow(x11.x_display, screen);

    XSetWindowAttributes swa;
    swa.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;
    x11.x_window = XCreateWindow(
        x11.x_display, root,
        0, 0, width, height, 0,
        CopyFromParent, InputOutput,
        CopyFromParent, CWEventMask, &swa);

    XSetWindowAttributes xattr;
    xattr.background_pixel = 0;
    XChangeWindowAttributes(x11.x_display, x11.x_window, CWBackPixel, &xattr);

    XStoreName(x11.x_display, x11.x_window, "OpenGL Cube");
    XMapWindow(x11.x_display, x11.x_window);

    x11.wm_delete_window = XInternAtom(x11.x_display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(x11.x_display, x11.x_window, &x11.wm_delete_window, 1);

    return 1;
}

static int init_egl_context() {
    x11.egl_display = eglGetDisplay((EGLNativeDisplayType)x11.x_display);
    if (!x11.egl_display) {
        fprintf(stderr, "Failed to get EGL display\n");
        return 0;
    }

    EGLint major, minor;
    if (!eglInitialize(x11.egl_display, &major, &minor)) {
        fprintf(stderr, "Failed to initialize EGL\n");
        return 0;
    }

    eglBindAPI(EGL_OPENGL_ES_API);

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
    if (!eglChooseConfig(x11.egl_display, config_attribs, &config, 1, &num_config)) {
        fprintf(stderr, "Failed to choose EGL config\n");
        return 0;
    }

    EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    x11.egl_context = eglCreateContext(x11.egl_display, config, EGL_NO_CONTEXT, context_attribs);
    if (!x11.egl_context) {
        fprintf(stderr, "Failed to create EGL context\n");
        return 0;
    }

    x11.egl_surface = eglCreateWindowSurface(x11.egl_display, config, (EGLNativeWindowType)x11.x_window, NULL);
    if (!x11.egl_surface) {
        fprintf(stderr, "Failed to create EGL surface\n");
        return 0;
    }

    eglMakeCurrent(x11.egl_display, x11.egl_surface, x11.egl_surface, x11.egl_context);

    return 1;
}

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
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return program;
}

static GLuint current_program = 0;

static void render_cube() {
    static int i = 0;
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

    XWindowAttributes attributes;
    XGetWindowAttributes(x11.x_display, x11.x_window, &attributes);
    
    glViewport(0, 0, attributes.width, attributes.height);
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);

    float aspect = (float)attributes.width / (float)attributes.height;

    ESMatrix perspective, modelview;
    esMatrixLoadIdentity(&perspective);
    esPerspective(&perspective, 29.0f, aspect, 1.0f, 20.0f);
    
    i++;
    esMatrixLoadIdentity(&modelview);
    esTranslate(&modelview, 0.0f, 0.0f, -8.0f);
    esRotate(&modelview, 45.0f + (0.25f * i), 1.0f, 0.0f, 0.0f);
    esRotate(&modelview, 45.0f - (0.5f * i), 0.0f, 1.0f, 0.0f);
    esRotate(&modelview, 10.0f + (0.15f * i), 0.0f, 0.0f, 1.0f);

    ESMatrix mvpMatrix;
    esMatrixMultiply(&mvpMatrix, &modelview, &perspective);

    GLint mvpLoc = glGetUniformLocation(current_program, "u_mvpMatrix");
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

    eglSwapBuffers(x11.egl_display, x11.egl_surface);
}

int main(int argc, char** argv) {
    struct sigaction sigint;
    
    if (!init_x11_window(500, 500)) {
        return 1;
    }

    if (!init_egl_context()) {
        return 1;
    }

    current_program = create_shader_program(vertexShaderSource, fragmentShaderSource);
    if (!current_program) {
        return 1;
    }
    
    glUseProgram(current_program);

    sigint.sa_handler = handle_signal;
    sigemptyset(&sigint.sa_mask);
    sigint.sa_flags = SA_RESETHAND;
    sigaction(SIGINT, &sigint, NULL);

    x11.running = 1;

    while (x11.running) {
        while (XPending(x11.x_display)) {
            XEvent event;
            XNextEvent(x11.x_display, &event);
            
            switch (event.type) {
                case ClientMessage:
                    if (event.xclient.data.l[0] == x11.wm_delete_window) {
                        x11.running = 0;
                    }
                    break;
                case ConfigureNotify:
                    break;
                case Expose:
                    break;
            }
        }
        
        render_cube();
        usleep(16667); // ~60 FPS
    }

    eglDestroyContext(x11.egl_display, x11.egl_context);
    eglDestroySurface(x11.egl_display, x11.egl_surface);
    eglTerminate(x11.egl_display);
    
    XDestroyWindow(x11.x_display, x11.x_window);
    XCloseDisplay(x11.x_display);

    return 0;
}