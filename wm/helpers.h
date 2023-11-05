#pragma once

#ifndef _HELPERS_H_
    #define _HELPERS_H_

    #include <wayland-client.h>

extern struct wl_compositor *compositor;
extern struct wl_display    *display;
extern struct wl_pointer    *pointer;
extern struct wl_seat       *seat;
extern struct wl_shell      *shell;
extern struct wl_shm        *shm;

typedef uint32_t pixel;

void hello_setup_wayland(void);
void hello_cleanup_wayland(void);

struct wl_buffer *hello_create_buffer(struct wl_shm_pool *pool,
                                      unsigned width, unsigned height);
void              hello_bind_buffer(struct wl_buffer        *buffer,
                                    struct wl_shell_surface *shell_surface);
void              hello_free_buffer(struct wl_buffer *buffer);

struct wl_shm_pool      *hello_create_memory_pool(int file);
void                     hello_free_memory_pool(struct wl_shm_pool *pool);
struct wl_shell_surface *hello_create_surface(void);
void                     hello_free_surface(struct wl_shell_surface *surface);

void hello_set_cursor_from_pool(struct wl_shm_pool *pool,
                                unsigned width, unsigned height, int32_t hot_spot_x, int32_t hot_spot_y);
void hello_free_cursor(void);
void hello_set_button_callback(struct wl_shell_surface *surface,
                               void (*callback)(uint32_t));



static void pointer_enter(void              *data,
                          struct wl_pointer *wl_pointer,
                          uint32_t serial, struct wl_surface *surface,
                          wl_fixed_t surface_x, wl_fixed_t surface_y);

static void pointer_leave(void              *data,
                          struct wl_pointer *wl_pointer, uint32_t serial,
                          struct wl_surface *wl_surface);

static void pointer_motion(void              *data,
                           struct wl_pointer *wl_pointer, uint32_t time,
                           wl_fixed_t surface_x, wl_fixed_t surface_y);

static void pointer_button(void              *data,
                           struct wl_pointer *wl_pointer, uint32_t serial,
                           uint32_t time, uint32_t button, uint32_t state);

static void pointer_axis(void              *data,
                         struct wl_pointer *wl_pointer, uint32_t time,
                         uint32_t axis, wl_fixed_t value);

static void registry_global_remove(void               *a,
                                   struct wl_registry *b, uint32_t c);


static void registry_global(void               *data,
                            struct wl_registry *registry, uint32_t name,
                            const char *interface, uint32_t version);

#endif /* _HELPERS_H_ */
