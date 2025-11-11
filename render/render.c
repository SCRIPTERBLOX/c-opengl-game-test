#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/common.h"
#include "../common/window.h"
#include "renderer.h"

int render(int argc, char** argv) {
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
    
    // Initialize renderer
    renderer_init(&window);

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

int main(int argc, char** argv) {
    return render(argc, argv);
}