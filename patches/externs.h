#ifndef EXTERNS_H
#define EXTERNS_H

#include "patches.h"

u32 __osSetFpcCsr(u32);
void* setSPToEnd(u8* stack, u32 size);
void mainproc(void* args);
void osInitialize(void);
s32 boot_osPiRawStartDma(s32, u32, void*, u32);
void osCreateThread(OSThread*, OSId, void (*)(void*), void*, void*, OSPri);

s16 viGetViewLeft(void);
s16 viGetViewTop(void);
s16 viGetViewWidth(void);
s16 viGetViewTop(void);
s16 viGetViewHeight(void);

s16 viGetX(void);
s16 viGetY(void);

s32 get_cur_playernum(void);
s32 getPlayerCount(void);

f32 bondviewGetPauseAnimationPercent(void);
u32 cur_player_get_screen_setting(void);

extern struct player *g_CurrentPlayer;
extern s32 cameraFrameCounter1;
extern s32 cameraFrameCounter2;
extern s32 cameraBufferToggle;
extern s32 g_ModelDistanceDisabled;
extern s32 z_buffer_height;
extern s32 z_buffer_width;
extern s32 z_buffer;
extern u8 sp_main[32768];
extern OSThread mainThread;
extern s32* stack_pointer;
extern s32 g_ModelDistanceDisabled;
extern u8 *g_GfxBuffers[3];
extern u8 g_GfxActiveBufferIndex;
extern s32 g_GfxRequestedDisplayList;

#endif