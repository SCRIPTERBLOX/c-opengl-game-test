#ifndef RENDERER_H
#define RENDERER_H

#include "../common/window.h"

// Initialize the renderer
void renderer_init(struct window* window);

// Main redraw function
void redraw(void* data, struct wl_callback* callback, uint32_t time);

#endif // RENDERER_H