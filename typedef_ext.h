#pragma once

#include <inttypes.h>

enum INOUT_DEV {
    INPUT_DEV,
    OUTPUT_DEV
};

enum TYPE_DEV {
    PHISICAL,
    VIRTUAL
};

typedef struct __wmdevices_t {
    uint8_t dev_count;
    const char *name[50];
    uint8_t default_dev;
    INOUT_DEV inout_dev[50];
    TYPE_DEV type_dev[50];
    uint8_t index_tmp_out[50];
} wmdevices_t;

typedef struct __wm_system_status_t {
    bool conected;
    bool close_app;
    bool server_running;
    bool server_shuttingdown;
    bool silent_mode;
    bool autostart_mode;
    bool help_opt;
} wm_system_status_t;

extern wmdevices_t wmdev;
extern wm_system_status_t wmsystem_status;
