

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/mman.h>
#include <sys/stat.h>

#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

class WL
{
    static wl_compositor *compositor;
    static wl_display    *display;
    static wl_pointer    *pointer;
    static wl_seat       *seat;
    static wl_shell      *shell;
    static wl_shm        *shm;



    static void registry_global(void *data, wl_registry *registry, uint32_t name,
                                const char *interface, uint32_t version)
    {
        if (strcmp(interface, wl_compositor_interface.name) == 0) {
            compositor = (wl_compositor *)
                wl_registry_bind(registry, name, &wl_compositor_interface, std::fmin(version, 4));
        }
        else if (strcmp(interface, wl_shm_interface.name) == 0) {
            shm = (wl_shm *)
                wl_registry_bind(registry, name, &wl_shm_interface, std::fmin(version, 1));
        }
        else if (strcmp(interface, wl_shell_interface.name) == 0) {
            shell = (wl_shell *)
                wl_registry_bind(registry, name, &wl_shell_interface, std::fmin(version, 1));
        }
        else if (strcmp(interface, wl_seat_interface.name) == 0) {
            seat = (wl_seat *)
                wl_registry_bind(registry, name, &wl_seat_interface, std::fmin(version, 2));

            pointer = wl_seat_get_pointer(seat);
            wl_pointer_add_listener(pointer, &pointer_listener, NULL);
        }
    }

    static void registry_global_remove(void *a, wl_registry *b, uint32_t c) {}

    constexpr static const wl_registry_listener registry_listender{.global = registry_global, .global_remove = registry_global_remove};
    constexpr static const wl_pointer_listener  pointer_listener{};


    struct pool_data {
        int      fd;
        pixel   *memory;
        unsigned capacity;
        unsigned size;
    };


    wl_shm_pool *crate_memory_pool(int file)
    {
        pool_data  *data = new pool_data;
        struct stat stat;

        if (fstat(file, &stat) != 0) {
            return nullptr;
        }


        if (!data) {
            return nullptr;
        }

        data->capacity = stat.st_size;
        data->size     = 0;
        data->fd       = file;

        data->memory = mmap(0, data->capacity, PROT_READ, MAP_SHARED, data->fd, 0);

        if (data->memory == MAP_FAILED) {
            goto cleanup_alloc;
        }

        struct wl_shm_pool *pool;
        pool = wl_shm_create_pool(shm, data->fd, data->capacity);

        if (!pool) {
            goto cleanup_mmap;
        }

        wl_shm_pool_set_user_data(pool, data);

        return pool;

    cleanup_mmap:
        munmap(data->memory, data->capacity);
    cleanup_alloc:
        delete data;
        return nullptr;
    }


    void hello_free_memory_pool(struct wl_shm_pool *pool)
    {
        struct pool_data *data;

        data = (pool_data *)wl_shm_pool_get_user_data(pool);
        wl_shm_pool_destroy(pool);
        munmap(data->memory, data->capacity);
        delete data;
    }


  public:
    WL()
    {
        wl_registry *registry;

        display = wl_display_connect(nullptr);
        if (!display) {
            perror("Open Display");
            exit(EXIT_FAILURE);
        }

        registry = wl_display_get_registry(display);
        wl_registry_add_listener(registry, &registry_listender, nullptr);
        wl_display_roundtrip(display);
        wl_registry_destroy(registry);
    }

    ~WL()
    {
        wl_pointer_destroy(pointer);
        wl_seat_destroy(seat);
        wl_shell_destroy(shell);
        wl_shm_destroy(shm);
        wl_compositor_destroy(compositor);
        wl_display_disconnect(display);
    }
};
