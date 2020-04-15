/*  This file is part of SAIL (https://github.com/sailor-keg/sail)

    Copyright (c) 2020 Dmitry Baryshev <dmitrymq@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this library. If not, see <https://www.gnu.org/licenses/>.
*/

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef SAIL_WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

/* libsail-common */
#include "common.h"
#include "error.h"
#include "log.h"

#include "plugin_info.h"
#include "plugin.h"

int sail_alloc_plugin(const struct sail_plugin_info *plugin_info, struct sail_plugin **plugin) {

    if (plugin_info == NULL || plugin_info->path == NULL) {
        return SAIL_INVALID_ARGUMENT;
    }

    *plugin = (struct sail_plugin *)malloc(sizeof(struct sail_plugin));

    if (*plugin == NULL) {
        return SAIL_MEMORY_ALLOCATION_FAILED;
    }

    (*plugin)->layout = plugin_info->layout;
    (*plugin)->handle = NULL;
    (*plugin)->v2     = NULL;

    SAIL_LOG_DEBUG("Loading plugin '%s'", plugin_info->path);

#ifdef SAIL_WIN32
    HMODULE handle = LoadLibrary(plugin_info->path);

    if (handle == NULL) {
        SAIL_LOG_ERROR("Failed to load '%s'. Error: %d", plugin_info->path, GetLastError());
        sail_destroy_plugin(*plugin);
        return SAIL_PLUGIN_LOAD_ERROR;
    }

    (*plugin)->handle = handle;
#else
    void *handle = dlopen(plugin_info->path, RTLD_LAZY | RTLD_LOCAL);

    if (handle == NULL) {
        SAIL_LOG_ERROR("Failed to load '%s': %s", plugin_info->path, dlerror());
        sail_destroy_plugin(*plugin);
        return SAIL_PLUGIN_LOAD_ERROR;
    }

    (*plugin)->handle = handle;
#endif

#ifdef SAIL_WIN32
    #define SAIL_RESOLVE GetProcAddress
#else
    #define SAIL_RESOLVE dlsym
#endif

    if ((*plugin)->layout == SAIL_PLUGIN_LAYOUT_V2) {
        (*plugin)->v2 = (struct sail_plugin_layout_v2 *)malloc(sizeof(struct sail_plugin_layout_v2));

        (*plugin)->v2->read_features_v2        = (sail_plugin_read_features_v2_t)       SAIL_RESOLVE(handle, "sail_plugin_read_features_v2");
        (*plugin)->v2->read_init_v2            = (sail_plugin_read_init_v2_t)           SAIL_RESOLVE(handle, "sail_plugin_read_init_v2");
        (*plugin)->v2->read_seek_next_frame_v2 = (sail_plugin_read_seek_next_frame_v2_t)SAIL_RESOLVE(handle, "sail_plugin_read_seek_next_frame_v2");
        (*plugin)->v2->read_seek_next_pass_v2  = (sail_plugin_read_seek_next_pass_v2_t) SAIL_RESOLVE(handle, "sail_plugin_read_seek_next_pass_v2");
        (*plugin)->v2->read_scan_line_v2       = (sail_plugin_read_scan_line_v2_t)      SAIL_RESOLVE(handle, "sail_plugin_read_scan_line_v2");
        (*plugin)->v2->read_alloc_scan_line_v2 = (sail_plugin_read_alloc_scan_line_v2_t)SAIL_RESOLVE(handle, "sail_plugin_read_alloc_scan_line_v2");
        (*plugin)->v2->read_finish_v2          = (sail_plugin_read_finish_v2_t)         SAIL_RESOLVE(handle, "sail_plugin_read_finish_v2");

        (*plugin)->v2->write_features_v2        = (sail_plugin_write_features_v2_t)       SAIL_RESOLVE(handle, "sail_plugin_write_features_v2");
        (*plugin)->v2->write_init_v2            = (sail_plugin_write_init_v2_t)           SAIL_RESOLVE(handle, "sail_plugin_write_init_v2");
        (*plugin)->v2->write_seek_next_frame_v2 = (sail_plugin_write_seek_next_frame_v2_t)SAIL_RESOLVE(handle, "sail_plugin_write_seek_next_frame_v2");
        (*plugin)->v2->write_seek_next_pass_v2  = (sail_plugin_write_seek_next_pass_v2_t) SAIL_RESOLVE(handle, "sail_plugin_write_seek_next_pass_v2");
        (*plugin)->v2->write_scan_line_v2       = (sail_plugin_write_scan_line_v2_t)      SAIL_RESOLVE(handle, "sail_plugin_write_scan_line_v2");
        (*plugin)->v2->write_finish_v2          = (sail_plugin_write_finish_v2_t)         SAIL_RESOLVE(handle, "sail_plugin_write_finish_v2");
    } else {
        return SAIL_UNSUPPORTED_PLUGIN_LAYOUT;
    }

    return 0;
}

void sail_destroy_plugin(struct sail_plugin *plugin) {

    if (plugin == NULL) {
        return;
    }

    if (plugin->handle != NULL) {
#ifdef SAIL_WIN32
        FreeLibrary((HMODULE)plugin->handle);
#else
        dlclose(plugin->handle);
#endif
    }

    if (plugin->layout == SAIL_PLUGIN_LAYOUT_V2) {
        free(plugin->v2);
    } else {
        SAIL_LOG_WARNING("Don't know how to destroy plugin interface version %d", plugin->layout);
    }

    free(plugin);
}

sail_error_t sail_plugin_read_features(const struct sail_plugin *plugin, struct sail_read_features **read_features) {

    SAIL_CHECK_PLUGIN_PTR(plugin);

    if (plugin->layout == SAIL_PLUGIN_LAYOUT_V2) {
        SAIL_TRY(plugin->v2->read_features_v2(read_features));
    } else {
        return SAIL_UNSUPPORTED_PLUGIN_LAYOUT;
    }

    return 0;
}

sail_error_t sail_plugin_write_features(const struct sail_plugin *plugin, struct sail_write_features **write_features) {

    SAIL_CHECK_PLUGIN_PTR(plugin);

    if (plugin->layout == SAIL_PLUGIN_LAYOUT_V2) {
        SAIL_TRY(plugin->v2->write_features_v2(write_features));
    } else {
        return SAIL_UNSUPPORTED_PLUGIN_LAYOUT;
    }

    return 0;
}
