#ifndef Z_FBDEMO_TRIFORCE_H
#define Z_FBDEMO_TRIFORCE_H

#include "global.h"

typedef enum {
    /* 0 */ STATE_NONE,
    /* 1 */ STATE_SPIRAL_IN_SLOW,
    /* 2 */ STATE_SPIRAL_IN_FAST,
    /* 3 */ STATE_SPIRAL_OUT_SLOW,
    /* 4 */ STATE_SPIRAL_OUT_FAST
} FbTriforceState;

typedef enum {
    /* 0 */ TYPE_NONE,
    /* 1 */ TYPE_TRANSPARENT_TRIFORCE,
    /* 2 */ TYPE_FILLED_TRIFORCE
} TriforceFadeType;

typedef struct {
    /* 0x000 */ Color_RGBA8_u32 color;
    /* 0x004 */ f32 transPos;
    /* 0x008 */ f32 step;
    /* 0x00C */ s32 state;
    /* 0x010 */ s32 fadeType;
    /* 0x018 */ Mtx projection;
    /* 0x058 */ s32 frame;
    /* 0x060 */ Mtx modelView[2][3];
} TransitionTriforce; // size = 0x1E0

#endif
