#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "wayland-util.h"

extern const struct wl_interface wl_output_interface;
extern const struct wl_interface wl_seat_interface;
extern const struct wl_interface wl_surface_interface;

static const struct wl_interface *xdg_wm_base_types[] = {
	NULL,
	NULL,
	NULL,
	NULL,
};

static const struct wl_interface *xdg_surface_types[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static const struct wl_interface *xdg_toplevel_types[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static const struct wl_message xdg_wm_base_methods[] = {
	{ "destroy", "xdg_wm_base", xdg_wm_base_types + 0 },
	{ "create_positioner", "n", xdg_wm_base_types + 1 },
	{ "get_xdg_surface", "no", xdg_wm_base_types + 2 },
	{ "pong", "u", xdg_wm_base_types + 0 },
};

static const struct wl_message xdg_wm_base_events[] = {
	{ "ping", "u", xdg_wm_base_types + 0 },
};

const struct wl_interface xdg_wm_base_interface = {
	.name = "xdg_wm_base",
	.version = 6,
	.method_count = 4,
	.methods = xdg_wm_base_methods,
	.event_count = 1,
	.events = xdg_wm_base_events,
};

static const struct wl_message xdg_surface_methods[] = {
	{ "destroy", "xdg_surface", xdg_surface_types + 0 },
	{ "get_toplevel", "n", xdg_surface_types + 1 },
	{ "get_popup", "noo", xdg_surface_types + 2 },
	{ "set_window_geometry", "iiii", xdg_surface_types + 0 },
	{ "ack_configure", "u", xdg_surface_types + 0 },
};

static const struct wl_message xdg_surface_events[] = {
	{ "configure", "u", xdg_surface_types + 0 },
};

const struct wl_interface xdg_surface_interface = {
	.name = "xdg_surface",
	.version = 6,
	.method_count = 5,
	.methods = xdg_surface_methods,
	.event_count = 1,
	.events = xdg_surface_events,
};

static const struct wl_message xdg_toplevel_methods[] = {
	{ "destroy", "xdg_toplevel", xdg_toplevel_types + 0 },
	{ "set_parent", "?o", xdg_toplevel_types + 1 },
	{ "set_title", "s", xdg_toplevel_types + 0 },
	{ "set_app_id", "s", xdg_toplevel_types + 0 },
	{ "show_window_menu", "oui", xdg_toplevel_types + 2 },
	{ "move", "ou", xdg_toplevel_types + 3 },
	{ "resize", "ouu", xdg_toplevel_types + 4 },
	{ "set_max_size", "ii", xdg_toplevel_types + 0 },
	{ "set_min_size", "ii", xdg_toplevel_types + 0 },
	{ "set_maximized", "xdg_toplevel", xdg_toplevel_types + 0 },
	{ "unset_maximized", "xdg_toplevel", xdg_toplevel_types + 0 },
	{ "set_fullscreen", "?o", xdg_toplevel_types + 5 },
	{ "unset_fullscreen", "xdg_toplevel", xdg_toplevel_types + 0 },
	{ "set_minimized", "xdg_toplevel", xdg_toplevel_types + 0 },
};

static const struct wl_message xdg_toplevel_events[] = {
	{ "configure", "iia", xdg_toplevel_types + 6 },
	{ "close", "", xdg_toplevel_types + 0 },
};

const struct wl_interface xdg_toplevel_interface = {
	.name = "xdg_toplevel",
	.version = 6,
	.method_count = 14,
	.methods = xdg_toplevel_methods,
	.event_count = 2,
	.events = xdg_toplevel_events,
};