#ifndef EXTERNS_H
#define EXTERNS_H

#include "patches.h"

s16 viGetViewLeft(void);
s16 viGetViewTop(void);
s16 viGetViewWidth(void);
s16 viGetViewTop(void);
s16 viGetViewHeight(void);

f32 bondviewGetPauseAnimationPercent(void);
u32 cur_player_get_screen_setting(void);

extern struct player *g_CurrentPlayer;
extern s32 cameraFrameCounter1;
extern s32 cameraFrameCounter2;
extern s32 cameraBufferToggle;

#endif