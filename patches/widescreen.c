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