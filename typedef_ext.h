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

extern wmdevices_t wmdev;
