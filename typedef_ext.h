#pragma once

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
    const char *name[10];
    uint8_t default_dev;
    INOUT_DEV inout_dev;
    TYPE_DEV type_dev;
} wmdevices_t;

extern wmdevices_t wmdev;
