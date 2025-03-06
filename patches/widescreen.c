#include "patches.h"

s16 sExTextLabelsCount = 0;
struct ExTextLabel* sExTextLabels[52];

#if 1 // ALL
// obj_is_in_view
#if 1
RECOMP_PATCH s32 obj_is_in_view(struct GraphNodeObject* node, Mat4 matrix) {
    s16 cullingRadius;
    s16 halfFov; // half of the fov in in-game angle units instead of degrees
    struct GraphNode* geo;
    f32 hScreenEdge;

    if (node->node.flags & GRAPH_RENDER_INVISIBLE) {
        return FALSE;
    }

    geo = node->sharedChild;

    // ! @bug The aspect ratio is not accounted for. When the fov value is 45,
    // the horizontal effective fov is actually 60 degrees, so you can see objects
    // visibly pop in or out at the edge of the screen.
    halfFov = (gCurGraphNodeCamFrustum->fov / 2.0f + 1.0f) * 32768.0f / 180.0f + 0.5f;

    hScreenEdge = -matrix[3][2] * sins(halfFov) / coss(halfFov);

    // -matrix[3][2] is the depth, which gets multiplied by tan(halfFov) to get
    // the amount of units between the center of the screen and the horizontal edge
    // given the distance from the object to the camera.

#ifdef WIDESCREEN
    // This multiplication should really be performed on 4:3 as well,
    // but the issue will be more apparent on widescreen.
    //hScreenEdge *= 16.0f / 9.0f; // GFX_DIMENSIONS_ASPECT_RATIO; <-- workaround till memory pool is increased
    hScreenEdge *= (GFX_DIMENSIONS_ASPECT_RATIO < (4.0f / 3.0f) ? (4.0f / 3.0f) : (GFX_DIMENSIONS_ASPECT_RATIO > (16.0f / 9.0f) ? (16.0f / 9.0f) : GFX_DIMENSIONS_ASPECT_RATIO));

#endif

    if (geo != NULL && geo->type == GRAPH_NODE_TYPE_CULLING_RADIUS) {
        cullingRadius = (f32) ((struct GraphNodeCullingRadius*) geo)->cullingRadius; //! Why is there a f32 cast?
    } else {
        cullingRadius = 300;
    }

    // Don't render if the object is close to or behind the camera
    if (matrix[3][2] > -100.0f + cullingRadius) {
        return FALSE;
    }

    //! This makes the HOLP not update when the camera is far away, and it
    //  makes PU travel safe when the camera is locked on the main map.
    //  If Mario were rendered with a depth over 65536 it would cause overflow
    //  when converting the transformation matrix to a fixed point matrix.

    if (gMenuState == MENU_STATE_PAUSE_SCREEN_COURSE) {
        if (matrix[3][2] < -10000.0f - cullingRadius) {
            return FALSE;
        }
    } else {
        if (matrix[3][2] < -20000.0f - cullingRadius) {
            return FALSE;
        }
    }

    // Check whether the object is horizontally in view
    if (matrix[3][0] > hScreenEdge + cullingRadius) {
        return FALSE;
    }
    if (matrix[3][0] < -hScreenEdge - cullingRadius) {
        return FALSE;
    }
    return TRUE;
}

#endif

// create_skybox_ortho_matrix
#if 1
RECOMP_PATCH void* create_skybox_ortho_matrix(s8 player) {
    f32 left = sSkyBoxInfo[player].scaledX;
    f32 right = sSkyBoxInfo[player].scaledX + SCREEN_WIDTH;
    f32 bottom = sSkyBoxInfo[player].scaledY - SCREEN_HEIGHT;
    f32 top = sSkyBoxInfo[player].scaledY;
    Mtx* mtx = alloc_display_list(sizeof(*mtx));

#ifdef WIDESCREEN
    f32 half_width = (4.0f / 3.0f) / GFX_DIMENSIONS_ASPECT_RATIO * SCREEN_WIDTH / 2;
    f32 center = (sSkyBoxInfo[player].scaledX + SCREEN_WIDTH / 2);
    if (half_width < SCREEN_WIDTH / 2) {
        // A wider screen than 4:3
        left = center - half_width;
        right = center + half_width;
    }
#endif

    if (mtx != NULL) {
        guOrtho(mtx, left, right, bottom, top, 0.0f, 3.0f, 1.0f);
    } else {
    }

    return mtx;
}

#endif

// render_cannon_circle_base
#if 1
RECOMP_PATCH Gfx* render_cannon_circle_base(void) {
    float aspect = recomp_get_aspect_ratio(1.0f); // adjust parameter as needed
    int borderLength = (int) (SCREEN_WIDTH * aspect);
    Vtx* verts = alloc_display_list(12 * sizeof(*verts));
    Gfx* dlist = alloc_display_list(48 * sizeof(*dlist));
    Gfx* g = dlist;

    if (verts != NULL && dlist != NULL) {
        // Circle quad (vertices 0-3)
        make_vertex(verts, 0, 0, 0, -1, -1152, 1824, 0, 0, 0, 255);
        make_vertex(verts, 1, SCREEN_WIDTH, 0, -1, 1152, 1824, 0, 0, 0, 255);
        make_vertex(verts, 2, SCREEN_WIDTH, SCREEN_HEIGHT, -1, 1152, 192, 0, 0, 0, 255);
        make_vertex(verts, 3, 0, SCREEN_HEIGHT, -1, -1152, 192, 0, 0, 0, 255);

        // Left black rectangle (vertices 4-7): from x = -borderLength to x = 0
        make_vertex(verts, 4, -borderLength, 0, -1, 0, 0, 0, 0, 0, 255);
        make_vertex(verts, 5, 0, 0, -1, 0, 0, 0, 0, 0, 255);
        make_vertex(verts, 6, 0, SCREEN_HEIGHT, -1, 0, 0, 0, 0, 0, 255);
        make_vertex(verts, 7, -borderLength, SCREEN_HEIGHT, -1, 0, 0, 0, 0, 0, 255);

        // Right black rectangle (vertices 8-11): from x = SCREEN_WIDTH to x = SCREEN_WIDTH+borderLength
        make_vertex(verts, 8, SCREEN_WIDTH, 0, -1, 0, 0, 0, 0, 0, 255);
        make_vertex(verts, 9, SCREEN_WIDTH + borderLength, 0, -1, 0, 0, 0, 0, 0, 255);
        make_vertex(verts, 10, SCREEN_WIDTH + borderLength, SCREEN_HEIGHT, -1, 0, 0, 0, 0, 0, 255);
        make_vertex(verts, 11, SCREEN_WIDTH, SCREEN_HEIGHT, -1, 0, 0, 0, 0, 0, 255);

        // Draw the cannon circle quad
        gSPDisplayList(g++, (Gfx*) 0x20174c0);
        gDPSetCombineMode(g++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
        gDPSetTextureFilter(g++, G_TF_BILERP);
        gDPLoadTextureBlock(g++, sTextureTransitionID[TEX_TRANS_CIRCLE], G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                            G_TX_WRAP | G_TX_MIRROR, G_TX_WRAP | G_TX_MIRROR, 5, 6, G_TX_NOLOD, G_TX_NOLOD);
        gSPTexture(g++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gSPVertex(g++, VIRTUAL_TO_PHYSICAL(verts), 4, 0);
        gSPDisplayList(g++, (Gfx*) 0x2017350);
        gSPTexture(g++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
        gSPDisplayList(g++, (Gfx*) 0x020174f8);

        // Draw left black rectangle
        gDPSetRenderMode(g++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gDPSetCombineMode(g++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
        gDPSetPrimColor(g++, 0, 0, 0, 0, 0, 255);
        gSPVertex(g++, VIRTUAL_TO_PHYSICAL(verts + 4), 4, 0);
        gSP2Triangles(g++, 0, 1, 2, 0, 0, 2, 3, 0);

        // Draw right black rectangle
        gSPVertex(g++, VIRTUAL_TO_PHYSICAL(verts + 8), 4, 0);
        gSP2Triangles(g++, 0, 1, 2, 0, 0, 2, 3, 0);
        gSPEndDisplayList(g);
    } else {
        return NULL;
    }
    return dlist;
}
#endif

// clear_viewport
#if 1
RECOMP_PATCH void clear_viewport(Vp* viewport, s32 color) {
    s16 vpUlx = (viewport->vp.vtrans[0] - viewport->vp.vscale[0]) / 4 + 1;
    s16 vpUly = (viewport->vp.vtrans[1] - viewport->vp.vscale[1]) / 4 + 1;
    s16 vpLrx = (viewport->vp.vtrans[0] + viewport->vp.vscale[0]) / 4 - 2;
    s16 vpLry = (viewport->vp.vtrans[1] + viewport->vp.vscale[1]) / 4 - 2;

#ifdef WIDESCREEN
    vpUlx = GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(vpUlx);
    vpLrx = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(SCREEN_WIDTH - vpLrx);
#endif

    gDPPipeSync(gDisplayListHead++);

    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);

    gDPSetFillColor(gDisplayListHead++, color);
    gDPFillRectangle(gDisplayListHead++, vpUlx, vpUly, vpLrx, vpLry);

    gDPPipeSync(gDisplayListHead++);

    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}
#endif

// render_dialog_entries (crashes game ?? What is this for?)
#if 0
RECOMP_PATCH void render_dialog_entries(void) {
#ifdef VERSION_EU
        s8 lowerBound;
#endif
        void **dialogTable;
        struct DialogEntry *dialog;
#if defined(VERSION_US) || defined(VERSION_SH) || defined(VERSION_CN)
        s8 lowerBound;
#endif

#ifdef VERSION_EU
        gInGameLanguage = eu_get_language();
        switch (gInGameLanguage) {
            case LANGUAGE_ENGLISH:
                dialogTable = segmented_to_virtual(dialog_table_eu_en);
                break;
            case LANGUAGE_FRENCH:
                dialogTable = segmented_to_virtual(dialog_table_eu_fr);
                break;
            case LANGUAGE_GERMAN:
                dialogTable = segmented_to_virtual(dialog_table_eu_de);
                break;
        }
#else
        dialogTable = segmented_to_virtual((void*)0x02010a68);
#endif
        dialog = segmented_to_virtual(dialogTable[gDialogID]);
    
        // if the dialog entry is invalid, set the ID to DIALOG_NONE.
        if (dialog == segmented_to_virtual(NULL)) {
            gDialogID = DIALOG_NONE;
            return;
        }

#ifdef VERSION_EU
        gDialogX = 0;
        gDialogY = 0;
#endif
    
        switch (gMenuState) {
            case MENU_STATE_DIALOG_OPENING:
                if (gDialogBoxAngle == DIALOG_BOX_ANGLE_DEFAULT) {
                    play_dialog_sound(gDialogID);
                    play_sound(SOUND_MENU_MESSAGE_APPEAR, gGlobalSoundSource);
                }
    
                if (gDialogBoxType == DIALOG_TYPE_ROTATE) {
                    gDialogBoxAngle -= 7.5;
                    gDialogBoxScale -= 1.5;
                } else {
                    gDialogBoxAngle -= 10.0;
                    gDialogBoxScale -= 2.0;
                }
    
                if (gDialogBoxAngle == 0.0f) {
                    gMenuState = MENU_STATE_DIALOG_OPEN;
                    gMenuLineNum = 1;
                }
#ifndef VERSION_JP
                lowerBound = 1;
#endif
                break;
    
            case MENU_STATE_DIALOG_OPEN:
                gDialogBoxAngle = 0.0f;
    
                if ((gPlayer3Controller->buttonPressed & A_BUTTON)
                 || (gPlayer3Controller->buttonPressed & B_BUTTON)) {
                    if (gNextDialogPageStartStrIndex == -1) {
                        handle_special_dialog_text(gDialogID);
                        gMenuState = MENU_STATE_DIALOG_CLOSING;
                    } else {
                        gMenuState = MENU_STATE_DIALOG_SCROLLING;
                        play_sound(SOUND_MENU_MESSAGE_NEXT_PAGE, gGlobalSoundSource);
                    }
                }
#ifndef VERSION_JP
                lowerBound = 1;
#endif
                break;
    
            case MENU_STATE_DIALOG_SCROLLING:
                gDialogScrollOffsetY += dialog->linesPerBox * 2;
    
                if (gDialogScrollOffsetY >= dialog->linesPerBox * DIAG_VAL1) {
                    gDialogPageStartStrIndex = gNextDialogPageStartStrIndex;
                    gMenuState = MENU_STATE_DIALOG_OPEN;
                    gDialogScrollOffsetY = 0;
                }
#ifndef VERSION_JP
                lowerBound = (gDialogScrollOffsetY / DIAG_VAL1) + 1;
#endif
                break;
    
            case MENU_STATE_DIALOG_CLOSING:
                if (gDialogBoxAngle == 20.0f) {
                    level_set_transition(0, NULL);
                    play_sound(SOUND_MENU_MESSAGE_DISAPPEAR, gGlobalSoundSource);
    
                    if (gDialogBoxType == DIALOG_TYPE_ZOOM) {
                        trigger_cutscene_dialog(2);
                    }
    
                    gDialogResponse = gMenuLineNum;
                }
    
                gDialogBoxAngle += 10.0f;
                gDialogBoxScale += 2.0f;
    
                if (gDialogBoxAngle == DIALOG_BOX_ANGLE_DEFAULT) {
                    gMenuState = MENU_STATE_DEFAULT;
                    gDialogID = DIALOG_NONE;
                    gDialogPageStartStrIndex = 0;
                    gDialogWithChoice = FALSE;
                    gNextDialogPageStartStrIndex = 0;
                    gDialogResponse = DIALOG_RESPONSE_NONE;
                }
#ifndef VERSION_JP
                lowerBound = 1;
#endif
                break;
        }
    
        render_dialog_box_type(dialog, dialog->linesPerBox);

#ifdef VERSION_CN
    // This isn't really a diff. The iQue compiler doesn't allow the use of ifdefs inside a macro for some reason, so those were eliminated.
#ifdef WIDESCREEN
#define ulx 0
#define lrx SCREEN_WIDTH
#else
#define ulx ensure_nonnegative(dialog->leftOffset)
#define lrx ensure_nonnegative(dialog->leftOffset + DIAG_VAL3)
#endif
        gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, ulx, ensure_nonnegative(DIAG_VAL2 - dialog->width),
            lrx, ensure_nonnegative((256 - dialog->width) + (dialog->linesPerBox * 80 / DIAG_VAL4)));
#undef ulx
#undef lrx
#else
        gDPSetScissor(
            gDisplayListHead++, G_SC_NON_INTERLACE,
            // Horizontal scissoring isn't really required and can potentially mess up widescreen enhancements.
#ifdef WIDESCREEN
            0,
#else
            ensure_nonnegative(dialog->leftOffset),
#endif
            ensure_nonnegative(DIAG_VAL2 - dialog->width),
#ifdef VERSION_EU
#ifdef WIDESCREEN
            SCREEN_WIDTH,
#else
            ensure_nonnegative(dialog->leftOffset + (DIAG_VAL3 / gDialogBoxScale)),
#endif
            ensure_nonnegative((240 - dialog->width) + (dialog->linesPerBox * 80 / DIAG_VAL4 / gDialogBoxScale))
#else
#ifdef WIDESCREEN
            SCREEN_WIDTH,
#else
            ensure_nonnegative(dialog->leftOffset + DIAG_VAL3),
#endif
            ensure_nonnegative((240 - dialog->width) + (dialog->linesPerBox * 80 / DIAG_VAL4))
#endif
        );
#endif

#ifdef VERSION_JP
        handle_dialog_text_and_pages(0, dialog);
#else
        handle_dialog_text_and_pages(0, dialog, lowerBound);
#endif
    
        if (gNextDialogPageStartStrIndex == -1 && gDialogWithChoice == TRUE) {
            render_dialog_triangle_choice();
        }

#ifdef VERSION_EU
#undef BORDER_HEIGHT
#define BORDER_HEIGHT 8
#endif
        gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 2, 2, SCREEN_WIDTH - BORDER_HEIGHT / 2, SCREEN_HEIGHT - BORDER_HEIGHT / 2);
#ifdef VERSION_EU
#undef BORDER_HEIGHT
#define BORDER_HEIGHT 1
#endif
    
        if (gNextDialogPageStartStrIndex != -1 && gMenuState == MENU_STATE_DIALOG_OPEN) {
            render_dialog_triangle_next(dialog->linesPerBox);
        }
    }
#endif

// shade_screen
#if 1
RECOMP_PATCH void shade_screen(void) {

    create_dl_translation_matrix(MENU_MTX_PUSH, GFX_DIMENSIONS_FROM_LEFT_EDGE(-SCREEN_WIDTH * 8), SCREEN_HEIGHT, 0);

// This is a bit weird. It reuses the dialog text box (width 130, height -80),
// so scale to at least fit the screen.
#ifdef WIDESCREEN

    create_dl_scale_matrix(MENU_MTX_NOPUSH, GFX_DIMENSIONS_ASPECT_RATIO * SCREEN_HEIGHT / 1.0f, 3.0f, 1.0f);

#else
    create_dl_scale_matrix(MENU_MTX_NOPUSH, 2.6f, 3.4f, 1.0f);
#endif

    gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 110);
    gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}
#endif
// Unknow Fade?
#if 0
    RECOMP_PATCH void dl_rgba16_begin_cutscene_msg_fade(void) {
        // gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
        // gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gCutsceneMsgFade);
    }

    RECOMP_PATCH void dl_rgba16_stop_cutscene_msg_fade(void) {
        //gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
        //
        //if (gCutsceneMsgFade < 250) {
        //    gCutsceneMsgFade += 25;
        //} else {
        //    gCutsceneMsgFade = 255;
        //}
    }
#endif

// White Fade
#if 1
static Vtx* vertex_transition_color(struct WarpTransitionData* transData, u8 alpha) {
    Vtx* verts = alloc_display_list(4 * sizeof(*verts));
    u8 r = transData->red;
    u8 g = transData->green;
    u8 b = transData->blue;

    if (verts != NULL) {
        // Set far left to -SCREEN_WIDTH * 8
        s32 leftCoord = -SCREEN_WIDTH * 8;
        // Extend the far right by the same amount (adjust as needed)
        s32 rightCoord = GFX_DIMENSIONS_FROM_RIGHT_EDGE(0) + (SCREEN_WIDTH * 8);
        make_vertex(verts, 0, leftCoord, 0, -1, 0, 0, r, g, b, alpha);
        make_vertex(verts, 1, rightCoord, 0, -1, 0, 0, r, g, b, alpha);
        make_vertex(verts, 2, rightCoord, SCREEN_HEIGHT, -1, 0, 0, r, g, b, alpha);
        make_vertex(verts, 3, leftCoord, SCREEN_HEIGHT, -1, 0, 0, r, g, b, alpha);
    }
    return verts;
}

RECOMP_PATCH s32 dl_transition_color(s8 fadeTimer, u8 transTime, struct WarpTransitionData* transData, u8 alpha) {
    Vtx* verts = vertex_transition_color(transData, alpha);

    if (verts != NULL) {
        gSPDisplayList(gDisplayListHead++, dl_proj_mtx_fullscreen);
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
        gSPVertex(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(verts), 4, 0);
        gSPDisplayList(gDisplayListHead++, dl_draw_quad_verts_0123);
        gSPDisplayList(gDisplayListHead++, dl_screen_transition_end);
    }
    return set_and_reset_transition_fade_timer(fadeTimer, transTime);
}

RECOMP_PATCH s32 render_fade_transition_from_color(s8 fadeTimer, u8 transTime, struct WarpTransitionData* transData) {
    u8 alpha = set_transition_color_fade_alpha(1, fadeTimer, transTime);

    return dl_transition_color(fadeTimer, transTime, transData, alpha);
}

RECOMP_PATCH s32 render_fade_transition_into_color(s8 fadeTimer, u8 transTime, struct WarpTransitionData* transData) {
    u8 alpha = set_transition_color_fade_alpha(0, fadeTimer, transTime);

    return dl_transition_color(fadeTimer, transTime, transData, alpha);
}
#endif

// Title Screen
#if 0
RECOMP_PATCH s32 intro_regular(void) {
    s32 level = LEVEL_NONE;

    // @recomp: draws black box over the entire buffer
    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
    gDPSetFillColor(gDisplayListHead++, (GPACK_RGBA5551(0, 0, 0, 1) << 16) | GPACK_RGBA5551(0, 0, 0, 1));
    gDPFillRectangle(gDisplayListHead++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);

#ifndef VERSION_JP
    // When the game stars, gGlobalTimer is less than 129 frames,
    // so Mario greets the player. After that, he will always say
    // "press start to play" when it goes back to the title screen
    // (using SAVE AND QUIT)
    if (sPlayMarioGreeting == TRUE) {
        if (gGlobalTimer < 129) {
            play_sound(SOUND_MARIO_HELLO, gGlobalSoundSource);
        } else {
            play_sound(SOUND_MARIO_PRESS_START_TO_PLAY, gGlobalSoundSource);
        }
        sPlayMarioGreeting = FALSE;
    }
#endif
    print_intro_text();

    if (gPlayer1Controller->buttonPressed & START_BUTTON) {
        play_sound(SOUND_MENU_STAR_SOUND, gGlobalSoundSource);
#if ENABLE_RUMBLE
        queue_rumble_data(60, 70);
        func_sh_8024C89C(1);
#endif
        // calls level ID 100 (or 101 adding level select bool value)
        // defined in level_intro_mario_head_regular JUMP_IF commands
        // 100 is File Select - 101 is Level Select
        level = 100 + gDebugLevelSelect;
#ifndef VERSION_JP
        sPlayMarioGreeting = TRUE;
#endif
    }
    return run_level_id_or_demo(level);
}
#endif

// lvl_intro_update
#if 1
RECOMP_PATCH s32 lvl_intro_update(s16 arg, UNUSED s32 unusedArg) {
    s32 retVar;

<<<<<<< HEAD
=======
                // @recomp: draws black box clearing the entire buffer
                gDPPipeSync(gDisplayListHead++);
                gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
                gDPSetFillColor(gDisplayListHead++, (GPACK_RGBA5551(0, 0, 0, 1) << 16) | GPACK_RGBA5551(0, 0, 0, 1));
                gDPFillRectangle(gDisplayListHead++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
                gDPPipeSync(gDisplayListHead++);
                gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);

>>>>>>> 23ae22ca5a80a7c326b5c9947074dcb6a3499035
    switch (arg) {
        case LVL_INTRO_PLAY_ITS_A_ME_MARIO:
            retVar = intro_play_its_a_me_mario();
            break;
        case LVL_INTRO_REGULAR:
            retVar = intro_regular();
            break;
        case LVL_INTRO_GAME_OVER:
            retVar = intro_game_over();
            break;
        case LVL_INTRO_LEVEL_SELECT:
            retVar = intro_level_select();
            break;
    }

            // @recomp: draws black box clearing the entire buffer
            gDPPipeSync(gDisplayListHead++);
            gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
            gDPSetFillColor(gDisplayListHead++, (GPACK_RGBA5551(0, 0, 0, 1) << 16) | GPACK_RGBA5551(0, 0, 0, 1));
            gDPFillRectangle(gDisplayListHead++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
            gDPPipeSync(gDisplayListHead++);
            gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);

    return retVar;
}
#endif

// render_game
#if 1
RECOMP_PATCH void render_game(void) {

    if (gCurrentArea != NULL && !gWarpTransition.pauseRendering) {
        geo_process_root(gCurrentArea->unk04, D_8032CE74, D_8032CE78, gFBSetColor);

        gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_8032CF00));

        // gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, BORDER_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT -
        // BORDER_HEIGHT);
        gEXSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0,
                      SCREEN_HEIGHT);

        render_hud();

        // gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        gEXSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0,
                      SCREEN_HEIGHT);

        render_text_labels();
        do_cutscene_handler();
        print_displaying_credits_entry();

        // gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, BORDER_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT -
        // BORDER_HEIGHT);
        gEXSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0,
                      SCREEN_HEIGHT); // @recomp

        gMenuOptSelectIndex = render_menus_and_dialogs();
        if (gMenuOptSelectIndex != MENU_OPT_NONE) {
            gSaveOptSelectIndex = gMenuOptSelectIndex;
        }

        if (D_8032CE78 != NULL) {
            make_viewport_clip_rect(D_8032CE78);
        } else {
            // gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, BORDER_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT -
            // BORDER_HEIGHT);
            gEXSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0,
                          SCREEN_HEIGHT); // @recomp
        }

        if (gWarpTransition.isActive) {
            if (gWarpTransDelay == 0) {
                gWarpTransition.isActive =
                    !render_screen_transition(0, gWarpTransition.type, gWarpTransition.time, &gWarpTransition.data);
                if (!gWarpTransition.isActive) {
                    if (gWarpTransition.type & 1) {
                        gWarpTransition.pauseRendering = TRUE;
                    } else {
                        set_warp_transition_rgb(0, 0, 0);
                    }
                }
            } else {
                gWarpTransDelay--;
            }
        }
    } else {
        render_text_labels();

        if (D_8032CE78 != NULL) {
            clear_viewport(D_8032CE78, gWarpTransFBSetColor);
        } else {
            clear_framebuffer(gWarpTransFBSetColor);
        }
    }

    D_8032CE74 = NULL;
    D_8032CE78 = NULL;
}
#endif

// render_textrect
#if 1
RECOMP_PATCH void render_textrect(s32 x, s32 y, s32 pos) {
    s32 rectBaseX = x + pos * 12;
    s32 rectBaseY = 224 - y;
    s32 rectX;
    s32 rectY;

#ifndef WIDESCREEN
    // For widescreen we must allow drawing outside the usual area
    clip_to_bounds(&rectBaseX, &rectBaseY);
#endif
    rectX = rectBaseX;
    rectY = rectBaseY;
    gSPTextureRectangle(gDisplayListHead++, rectX << 2, rectY << 2, (rectX + 15) << 2, (rectY + 15) << 2,
                        G_TX_RENDERTILE, 0, 0, 4 << 10, 1 << 10);
}
#endif

// render_hud_mario_lives
#if 1
RECOMP_PATCH void render_hud_mario_lives(void) {
    print_text((22), HUD_TOP_Y, ",", GEX_ALIGN_LEFT); // 'Mario Head' glyph
    print_text((38), HUD_TOP_Y, "*", GEX_ALIGN_LEFT); // 'X' glyph
    print_text_fmt_int((54 + -0), HUD_TOP_Y, "%d", gHudDisplay.lives);
}
#endif

// render_hud_camera_status
#if 1
RECOMP_PATCH void render_hud_camera_status(void) {
    u8*(*cameraLUT)[6] = segmented_to_virtual(main_hud_camera_lut);
    s32 x = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(54);
    s32 y = 205;

    // @recomp: Right Aligned
    gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0,
                    -(SCREEN_WIDTH) * 4, 0);

    if (sCameraHUDStatus == CAM_STATUS_NONE) {
        return;
    }
    gSPDisplayList(gDisplayListHead++, dl_hud_img_begin);
    render_hud_tex_lut(x, y, (*cameraLUT)[GLYPH_CAM_CAMERA]);

    switch (sCameraHUDStatus & CAM_STATUS_MODE_GROUP) {
        case CAM_STATUS_MARIO:
            render_hud_tex_lut(x + 16, y, (*cameraLUT)[GLYPH_CAM_MARIO_HEAD]);
            break;
        case CAM_STATUS_LAKITU:
            render_hud_tex_lut(x + 16, y, (*cameraLUT)[GLYPH_CAM_LAKITU_HEAD]);
            break;
        case CAM_STATUS_FIXED:
            render_hud_tex_lut(x + 16, y, (*cameraLUT)[GLYPH_CAM_FIXED]);
            break;
    }

    switch (sCameraHUDStatus & CAM_STATUS_C_MODE_GROUP) {
        case CAM_STATUS_C_DOWN:
            render_hud_small_tex_lut(x + 4, y + 16, (*cameraLUT)[GLYPH_CAM_ARROW_DOWN]);
            break;
        case CAM_STATUS_C_UP:
            render_hud_small_tex_lut(x + 4, y - 8, (*cameraLUT)[GLYPH_CAM_ARROW_UP]);
            break;
    }

    gSPDisplayList(gDisplayListHead++, dl_hud_img_end);

    // @recomp: Align reset
    gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
}
#endif

// render_hud_stars
#if 1
RECOMP_PATCH void render_hud_stars(void) {
    s8 showX = 0;

    if (gHudFlash == 1 && gGlobalTimer & 0x08) {
        return;
    }

    if (gHudDisplay.stars < 100) {
        showX = 1;
    }

    // @recomp: Right Aligned
    print_text((320 - (78)), HUD_TOP_Y, "-", GEX_ALIGN_RIGHT); // 'Star' glyph
    if (showX == 1) {
        print_text((320 - (78)) + 16, HUD_TOP_Y, "*", GEX_ALIGN_RIGHT); // 'X' glyph
    }
    print_text_fmt_int((showX * 14) + (320 - (78 - 16)), HUD_TOP_Y, "%d", gHudDisplay.stars);
}
#endif

// render_hud_coins
#if 0
RECOMP_PATCH void render_hud_coins(void) {
    // @recomp: Right Aligned
    print_text((HUD_COINS_X), HUD_TOP_Y, "+", GEX_ALIGN_RIGHT);      // 'Coin' glyph
    print_text((HUD_COINS_X) + 16, HUD_TOP_Y, "*", GEX_ALIGN_RIGHT); // 'X' glyph
    print_text_fmt_int((HUD_COINS_X + 30), HUD_TOP_Y, "%d", gHudDisplay.coins);
}
#endif

// render_hud_timer
#if 1
RECOMP_PATCH void render_hud_timer(void) {
    u8*(*hudLUT)[58];
    u16 timerValFrames;
    u16 timerMins;
    u16 timerSecs;
    u16 timerFracSecs;

    hudLUT = segmented_to_virtual(main_hud_lut);
    timerValFrames = gHudDisplay.timer;

    timerMins = timerValFrames / (30 * 60);
    timerSecs = (timerValFrames - (timerMins * 1800)) / 30;

    timerFracSecs = ((timerValFrames - (timerMins * 1800) - (timerSecs * 30)) & 0xFFFF) / 3;

    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(91), 185, "%0d", timerMins);
    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(71), 185, "%02d", timerSecs);
    print_text_fmt_int(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(37), 185, "%d", timerFracSecs);

    // @recomp: Right aligned
    gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0,
                    -(SCREEN_WIDTH) * 4, 0);

    gSPDisplayList(gDisplayListHead++, dl_hud_img_begin);
    render_hud_tex_lut(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(81), 32, (*hudLUT)[GLYPH_APOSTROPHE]);
    render_hud_tex_lut(GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(46), 32, (*hudLUT)[GLYPH_DOUBLE_QUOTE]);
    gSPDisplayList(gDisplayListHead++, dl_hud_img_end);

    // @recomp: align reset
    gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
}
#endif

// render_text_labels
#if 1
RECOMP_PATCH void render_text_labels(void) {
    s32 i;
    s32 j;
    s8 glyphIndex;
    Mtx* mtx;

    if (sExTextLabelsCount == 0) {
        return;
    }

    mtx = alloc_display_list(sizeof(*mtx));

    if (mtx == NULL) {
        sExTextLabelsCount = 0;
        return;
    }

    guOrtho(mtx, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -10.0f, 10.0f, 1.0f);
    gSPPerspNormalize((Gfx*) (gDisplayListHead++), 0xFFFF);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(mtx), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gDisplayListHead++, dl_hud_img_begin);

    for (i = 0; i < sExTextLabelsCount; i++) {
        for (j = 0; j < sExTextLabels[i]->length; j++) {
            glyphIndex = char_to_glyph_index(sExTextLabels[i]->buffer[j]);
            if (glyphIndex != -1) {
                // @recomp
                if (sExTextLabels[i]->align >= GEX_ALIGN_LEFT && sExTextLabels[i]->align <= GEX_ALIGN_RIGHT) {
                    switch (sExTextLabels[i]->align) {
                        case GEX_ALIGN_LEFT:
                            gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT, 0, 0, 0, 0);
                            break;

                        case GEX_ALIGN_RIGHT:
                            gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT,
                                            -(SCREEN_WIDTH) * 4, 0, -(SCREEN_WIDTH) * 4, 0);
                            break;

                        default:
                            gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
                            break;
                    }
                }
                add_glyph_texture(glyphIndex);
                render_textrect(sExTextLabels[i]->x, sExTextLabels[i]->y, j);

                // @recomp: Reset
                gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
            }
        }

        mem_pool_free(gEffectsMemoryPool, sExTextLabels[i]);
    }

    gSPDisplayList(gDisplayListHead++, dl_hud_img_end);

    sExTextLabelsCount = 0;
}
#endif

// print_text_centered
#if 1
// @recomp: use sExTextLabels & sExTextLabelsCount
RECOMP_PATCH void print_text_centered(s32 x, s32 y, const char* str) {
    char c = 0;
    UNUSED s8 unused1 = 0;
    UNUSED s32 unused2 = 0;
    s32 length = 0;
    s32 srcIndex = 0;
#ifdef VERSION_CN
    s32 width = 0;
#endif
    // sExTextLabels[sExTextLabelsCount]->type = GEX_ALIGN_DEFAULT;

    // Don't continue if there is no memory to do so.
    if ((sExTextLabels[sExTextLabelsCount] = mem_pool_alloc(gEffectsMemoryPool, sizeof(struct TextLabel))) == NULL) {
        return;
    }

    c = str[srcIndex];

    // Set the array with the text to print while finding length.
    while (c != '\0') {
#ifdef VERSION_CN
        if ((u8) c == 0xB0 || (u8) c == 0xC0) {
            width = 16;
        } else {
            width = 12;
        }
#endif
        sExTextLabels[sExTextLabelsCount]->buffer[length] = c;
        length++;
        srcIndex++;
        c = str[srcIndex];
    }

    sExTextLabels[sExTextLabelsCount]->length = length;
#ifdef VERSION_CN
    sExTextLabels[sExTextLabelsCount]->x = x - width * length / 2;
#else
    sExTextLabels[sExTextLabelsCount]->x = x - 12 * length / 2;
#endif
    sExTextLabels[sExTextLabelsCount]->y = y;
    sExTextLabelsCount++;
}
#endif

#if 1
// @recomp: use sExTextLabels & sExTextLabelsCount
RECOMP_PATCH void print_text(s32 x, s32 y, const char* str, int align) {
    char c = 0;
    s32 length = 0;
    s32 srcIndex = 0;

    // Don't continue if there is no memory to do so.
    if ((sExTextLabels[sExTextLabelsCount] = mem_pool_alloc(gEffectsMemoryPool, sizeof(struct TextLabel))) == NULL) {
        return;
    }

    sExTextLabels[sExTextLabelsCount]->x = x;
    sExTextLabels[sExTextLabelsCount]->y = y;

    if ((align >= GEX_ALIGN_LEFT) && (align <= GEX_ALIGN_RIGHT)) {
        sExTextLabels[sExTextLabelsCount]->align = align;
    } else {
        sExTextLabels[sExTextLabelsCount]->align = GEX_ALIGN_DEFAULT;
    }

    c = str[srcIndex];

    // Set the array with the text to print while finding length.
    while (c != '\0') {
        sExTextLabels[sExTextLabelsCount]->buffer[length] = c;
        length++;
        srcIndex++;
        c = str[srcIndex];
    }

    sExTextLabels[sExTextLabelsCount]->length = length;
    sExTextLabelsCount++;
}

#endif

#if 1
// @recomp: use sExTextLabels & sExTextLabelsCount
RECOMP_PATCH void print_text_fmt_int(s32 x, s32 y, const char* str, s32 n) {
    char c = 0;
    s8 zeroPad = FALSE;
    u8 width = 0;
    s32 base = 0;
    s32 len = 0;
    s32 srcIndex = 0;

    // Don't continue if there is no memory to do so.
    if ((sExTextLabels[sExTextLabelsCount] = mem_pool_alloc(gEffectsMemoryPool, sizeof(struct TextLabel))) == NULL) {
        return;
    }

    sExTextLabels[sExTextLabelsCount]->x = x;
    sExTextLabels[sExTextLabelsCount]->y = y;

    c = str[srcIndex];

    while (c != '\0') {
        if (c == '%') {
            srcIndex++;

            parse_width_field(str, &srcIndex, &width, &zeroPad);

            if (str[srcIndex] != 'd' && str[srcIndex] != 'x') {
                break;
            }
            if (str[srcIndex] == 'd') {
                base = 10;
            }
            if (str[srcIndex] == 'x') {
                base = 16;
            }

            srcIndex++;

            format_integer(n, base, sExTextLabels[sExTextLabelsCount]->buffer + len, &len, width, zeroPad);
        } else { // straight copy
            sExTextLabels[sExTextLabelsCount]->buffer[len] = c;
            len++;
            srcIndex++;
        }
        c = str[srcIndex];
    }

    sExTextLabels[sExTextLabelsCount]->length = len;
    sExTextLabelsCount++;
}
#endif

// remove black boarders
#if 1


// draw_screen_borders
#if 1
RECOMP_PATCH void draw_screen_borders(void) {
    gDPPipeSync(gDisplayListHead++);

    //gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);

    gDPSetFillColor(gDisplayListHead++, GPACK_RGBA5551(0, 0, 0, 0) << 16 | GPACK_RGBA5551(0, 0, 0, 0));

// #if BORDER_HEIGHT != 0
//     gDPFillRectangle(gDisplayListHead++, GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), 0,
//                      GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0) - 1, BORDER_HEIGHT - 1);
//     gDPFillRectangle(gDisplayListHead++,
//                      GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), SCREEN_HEIGHT - BORDER_HEIGHT,
//                      GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0) - 1, SCREEN_HEIGHT - 1);
// #endif
}
#endif

extern uintptr_t gPhysicalZBuffer;

// init_z_buffer
#if 1
RECOMP_PATCH void init_z_buffer(void) {
    gDPPipeSync(gDisplayListHead++);

    gDPSetDepthSource(gDisplayListHead++, G_ZS_PIXEL);
    gDPSetDepthImage(gDisplayListHead++, gPhysicalZBuffer);

    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, gPhysicalZBuffer);
    gDPSetFillColor(gDisplayListHead++, GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0));

    gDPFillRectangle(gDisplayListHead++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 - 0);
}
#endif

#if 1
RECOMP_PATCH void select_framebuffer(void) {
    gDPPipeSync(gDisplayListHead++);

    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                     gPhysicalFramebuffers[sRenderingFramebuffer]);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH,
                  SCREEN_HEIGHT - 0);
}
#endif

//clear_framebuffer
#if 1
RECOMP_PATCH void clear_framebuffer(s32 color) {
    gDPPipeSync(gDisplayListHead++);

    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);

    gDPSetFillColor(gDisplayListHead++, color);
    gDPFillRectangle(gDisplayListHead++,
                     GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), 0,
                     GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0) - 1, SCREEN_HEIGHT - 0 - 1);

    gDPPipeSync(gDisplayListHead++);

    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}
#endif

struct GraphNodeBackground {
    /*0x00*/ struct FnGraphNode fnNode;
    /*0x18*/ s32 unused;
    /*0x1C*/ s32 background; // background ID, or rgba5551 color if fnNode.func is null
};

static void geo_append_display_list(void *displayList, s16 layer);

extern struct GraphNodeMasterList *gCurGraphNodeMasterList;


#if 1
RECOMP_PATCH void geo_process_background(struct GraphNodeBackground *node) {
    Gfx *list = NULL;

    if (node->fnNode.func != NULL) {
        list = node->fnNode.func(GEO_CONTEXT_RENDER, &node->fnNode.node, (struct AllocOnlyPool *) gMatStack[gMatStackIndex]);
    }
    if (list != NULL) {
        geo_append_display_list((void *) VIRTUAL_TO_PHYSICAL(list), node->fnNode.node.flags >> 8);
    } else if (gCurGraphNodeMasterList != NULL) {
#ifndef F3DEX_GBI_2E
        Gfx *gfxStart = alloc_display_list(sizeof(Gfx) * 7);
#else
        Gfx *gfxStart = alloc_display_list(sizeof(Gfx) * 8);
#endif
        Gfx *gfx = gfxStart;

        gDPPipeSync(gfx++);
        gDPSetCycleType(gfx++, G_CYC_FILL);
        gDPSetFillColor(gfx++, node->background);
        gDPFillRectangle(gfx++, GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), 0, GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0) - 1, SCREEN_HEIGHT - 0 - 1);
        gDPPipeSync(gfx++);
        gDPSetCycleType(gfx++, G_CYC_1CYCLE);
        gSPEndDisplayList(gfx++);

        geo_append_display_list((void *) VIRTUAL_TO_PHYSICAL(gfxStart), 0);
    }
    if (node->fnNode.node.children != NULL) {
        geo_process_node_and_siblings(node->fnNode.node.children);
    }
}
#endif

#endif // remove balck boarders

#endif // END