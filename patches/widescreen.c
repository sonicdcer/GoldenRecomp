#include "patches.h"

#include "PR/gbi.h"

s16 viGetX(void);
s16 viGetY(void);
s32 get_cur_playernum(void);
s32 getPlayerCount(void);
extern s32 z_buffer_height;
extern s32 z_buffer_width;
extern s32 z_buffer;

#if 1
RECOMP_PATCH Gfx* zbufClearCurrentPlayer(Gfx* gdl) __attribute__((optnone)) {
    s32 start_x;
    s32 end_x;

    gDPPipeSync(gdl++);
    gDPSetRenderMode(gdl++, G_RM_NOOP, G_RM_NOOP2);
    gDPSetColorImage(gdl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, z_buffer_width, OS_K0_TO_PHYSICAL(z_buffer));
    gDPSetCycleType(gdl++, G_CYC_FILL);
    gDPSetFillColor(gdl++, (GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0)));
    // gDPSetScissor(gdl++, G_SC_NON_INTERLACE, 0, 0, viGetX()/2, viGetY()/2);
    gEXEnable(gdl++);
    gEXSetScissor(gdl++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0, viGetY());
    if (getPlayerCount() < 3) {
        start_x = 0;
        end_x = viGetX() - 1;
    } else if ((get_cur_playernum() == 0) || (get_cur_playernum() == 2)) {
        start_x = 0;
        end_x = (viGetX() / 2) - 1;
    } else {
        start_x = viGetX() / 2;
        end_x = viGetX() - 1;
    }
    gDPFillRectangle(gdl++, start_x, 0, end_x + 240, (z_buffer_height - 1));
    gDPPipeSync(gdl++);
    return gdl;
}
#endif

/**
 * Provisory way to nuke the gDPSetScissor call inside this function,
 * this breaks multiplayer so it needs to be handled properly once we
 * fix the headers from the decomp.
 */
#if 1
RECOMP_PATCH Gfx* bgScissorCurrentPlayerView(Gfx* arg0, s32 left, s32 top, s32 width, s32 height) {
    return arg0;
}
#endif

extern s32 g_ModelDistanceDisabled;

#if 1
RECOMP_PATCH void modelSetDistanceDisabled(s32 param_1) {
    // @recomp: ModelDistance always disabled
    g_ModelDistanceDisabled = 1;
}
#endif

#define VIEWPORT_HEIGHT_DEFAULT 240
#define CINEMA_SCALE_FACTOR 84.0f
#define VIEWPORT_OFFSET_HEIGHT_CINEMA 136
#define WIDESCREEN_SCALE_FACTOR 40.0f
#define VIEWPORT_OFFSET_HEIGHT_WIDESCREEN 240 /*180 */

#define VIEWPORT_HEIGHT_FULLSCREEN 320 /* 304 ?? */
#define VIEWPORT_HEIGHT_WIDESCREEN 248
#define VIEWPORT_HEIGHT_CINEMA 190

/* 109 = (440 / 4) - 1 */
#define VIEWPORT_HEIGHT_4P_109 109
#define VIEWPORT_HEIGHT_4P VIEWPORT_HEIGHT_4P_109

#if 1
RECOMP_PATCH s16 bondviewGetCurrentPlayerViewportHeight(void) {
    f32 t;

    if (getPlayerCount() >= 2) {
        return VIEWPORT_HEIGHT_4P;
    }

    if (cameraBufferToggle != 0) {
        // if (cur_player_get_screen_setting() == SCREEN_SIZE_WIDESCREEN) {
        //     return VIEWPORT_HEIGHT_WIDESCREEN;
        // } else if (cur_player_get_screen_setting() == SCREEN_SIZE_CINEMA) {
        //     return VIEWPORT_HEIGHT_CINEMA;
        // } else {
        return VIEWPORT_HEIGHT_FULLSCREEN;
        // }
    }

    // if (cur_player_get_screen_setting() == SCREEN_SIZE_WIDESCREEN) {
    //     t = bondviewGetPauseAnimationPercent();
    //     return (s16) ((s32) (WIDESCREEN_SCALE_FACTOR * t) + VIEWPORT_OFFSET_HEIGHT_WIDESCREEN);
    // } else if (cur_player_get_screen_setting() == SCREEN_SIZE_CINEMA) {
    //     t = bondviewGetPauseAnimationPercent();
    //     return (s16) ((s32) (CINEMA_SCALE_FACTOR * t) + VIEWPORT_OFFSET_HEIGHT_CINEMA);
    // } else {
    return VIEWPORT_HEIGHT_DEFAULT;
    // }
}
#endif

#if 1
RECOMP_PATCH Gfx* currentPlayerDrawFade(Gfx* gdl) {
    f32 frac = g_CurrentPlayer->colourscreenfrac;
    s32 r = g_CurrentPlayer->colourscreenred;
    s32 g = g_CurrentPlayer->colourscreengreen;
    s32 b = g_CurrentPlayer->colourscreenblue;

    if ((cameraFrameCounter1 != 0) || (cameraFrameCounter2 != 0)) {
        frac = 1.0f;
        b = 0;
        g = 0;
        r = 0;
    }
    
    if (frac > 0) {
        gDPPipeSync(gdl++);
        gDPSetCycleType(gdl++, G_CYC_1CYCLE);
        gDPSetColorDither(gdl++, G_CD_DISABLE);
        gDPSetTexturePersp(gdl++, G_TP_NONE);
        gDPSetAlphaCompare(gdl++, G_AC_NONE);
        gDPSetTextureLOD(gdl++, G_TL_TILE);
        gDPSetTextureFilter(gdl++, G_TF_BILERP);
        gDPSetTextureConvert(gdl++, G_TC_FILT);
        gDPSetTextureLUT(gdl++, G_TT_NONE);
        gDPSetRenderMode(gdl++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
        gDPSetCombineMode(gdl++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
        gDPSetPrimColor(gdl++, 0, 0, r, g, b, (s32) (frac * 255.0f));

        // gDPFillRectangle(gdl++, viGetViewLeft(), viGetViewTop(), (viGetViewLeft() + viGetViewWidth()),
        // (viGetViewTop() + viGetViewHeight()));

        // @recomp: Remove margins
        gDPFillRectangle(gdl++, 0, 0, 320, 240);

        gDPPipeSync(gdl++);
        gDPSetColorDither(gdl++, G_CD_BAYER);
        gDPSetTexturePersp(gdl++, G_TP_PERSP);
        gDPSetTextureLOD(gdl++, G_TL_LOD);
    }

    return gdl;
}
#endif