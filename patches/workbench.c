#include "patches.h"
#include "sounds.h"

int pTagObjectId = 5;

s32 createObjId(void) {
    pTagObjectId += 1;
    recomp_printf("Created Object Tag: %d\n", (u32) pTagObjectId);
    return pTagObjectId;
}

// Leaving this here in case we want to implement our own level scripts, the entry point is located here.
#if 1
/**
 * Main game loop thread. Runs forever as long as the game continues.
 */
RECOMP_PATCH void thread5_game_loop(void* arg) __attribute__((optnone)) {
    struct LevelCommand* addr;

    // gGfxPool = gBiggerGfxPool;
    // gGfxPools = gBiggerGfxPools;
    gDebugLevelSelect = 0;

    CN_DEBUG_PRINTF(("start gfx thread\n"));

    setup_game_memory();
#if ENABLE_RUMBLE
    init_rumble_pak_scheduler_queue();
#endif

    CN_DEBUG_PRINTF(("init ctrl\n"));
    init_controllers();
    CN_DEBUG_PRINTF(("done ctrl\n"));

#if ENABLE_RUMBLE
    create_thread_6();
#endif
    save_file_load_all();

    set_vblank_handler(2, &gGameVblankHandler, &gGameVblankQueue, (OSMesg) 1);

    // Point address to the entry point into the level script data.
    addr = segmented_to_virtual(level_script_entry);

    play_music(SEQ_PLAYER_SFX, SEQUENCE_ARGS(0, 0x00), 0);
    set_sound_mode(save_file_get_sound_mode());
    render_init();

    while (TRUE) {
        // If the reset timer is active, run the process to reset the game.
        if (gResetTimer != 0) {
            draw_reset_bars();
            continue;
        }
        profiler_log_thread5_time(THREAD5_START);

        // If any controllers are plugged in, start read the data for when
        // read_controller_inputs is called later.
        if (gControllerBits) {
#if ENABLE_RUMBLE
            block_until_rumble_pak_free();
#endif
            osContStartReadData(&gSIEventMesgQueue);
        }

        audio_game_loop_tick();
        select_gfx_pool();
        read_controller_inputs();
        addr = level_script_execute(addr);

        display_and_vsync();

        // when debug info is enabled, print the "BUF %d" information.
        if (gShowDebugText) {
            // subtract the end of the gfx pool with the display list to obtain the
            // amount of free space remaining.
            print_text_fmt_int(180, 20, "BUF %d", gGfxPoolEnd - (u8*) gDisplayListHead);
        }
        static int object_count = 0;

        /* Make sure this code is placed inside a function so that the static variable persists */
        static int previous_object_count = -1; /* Ensures the count prints on the first run */
        object_count = 0;
        {
            int i;
            for (i = 0; i < 240; i++) {
                if (gObjectPool[i].activeFlags != 0) {
                    object_count++;
                }
            }
        }

        if (object_count != previous_object_count) {
            recomp_printf("Object count: %d\n", object_count);
            previous_object_count = object_count;
        }
    }
}
#endif

#if 0
RECOMP_PATCH void geo_process_node_and_siblings(struct GraphNode* firstNode) __attribute__((optnone)) {
    s16 iterateChildren = TRUE;
    struct GraphNode* curGraphNode = firstNode;
    struct GraphNode* parent = curGraphNode->parent;

    // In the case of a switch node, exactly one of the children of the node is
    // processed instead of all children like usual
    if (parent != NULL) {
        iterateChildren = (parent->type != GRAPH_NODE_TYPE_SWITCH_CASE);
    }
    recomp_printf("geo_process_node_and_siblings: \n");
    do {
        if (curGraphNode->flags & GRAPH_RENDER_ACTIVE) {

            if (curGraphNode->flags & GRAPH_RENDER_CHILDREN_FIRST) {
                geo_try_process_children(curGraphNode);
            } else {
                struct Object* test = (struct Object*) curGraphNode;
                u32 tag;
                switch (curGraphNode->type) {
                    case GRAPH_NODE_TYPE_ORTHO_PROJECTION:

                        geo_process_ortho_projection((struct GraphNodeOrthoProjection*) curGraphNode);
                        // @recomp Pop the transform id.
                        // gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
                        break;
                    case GRAPH_NODE_TYPE_PERSPECTIVE:
                        // recomp_printf("Previus Data: %x\n", (u32) test->oUnusedBhvParams);

                        // recomp_printf("Object Tag: %x\n", (u32) tag);
                        geo_process_perspective((struct GraphNodePerspective*) curGraphNode);
                        // @recomp Pop the transform id.
                        // gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
                        break;
                    case GRAPH_NODE_TYPE_MASTER_LIST:
                        recomp_printf("GRAPH_NODE_TYPE_MASTER_LIST: \n");
                        geo_process_master_list((struct GraphNodeMasterList*) curGraphNode);
                        // @recomp Pop the transform id.
                        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
                        break;
                    case GRAPH_NODE_TYPE_LEVEL_OF_DETAIL:
                        geo_process_level_of_detail((struct GraphNodeLevelOfDetail*) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_SWITCH_CASE:
                        geo_process_switch((struct GraphNodeSwitchCase*) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_CAMERA:
                        geo_process_camera((struct GraphNodeCamera*) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_TRANSLATION_ROTATION:
                        geo_process_translation_rotation((struct GraphNodeTranslationRotation*) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_TRANSLATION:
                        geo_process_translation((struct GraphNodeTranslation*) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_ROTATION:
                        geo_process_rotation((struct GraphNodeRotation*) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_OBJECT:
                        // @recomp Tag the transform.
                        //gEXMatrixGroupDecomposedNormal(gDisplayListHead++, (u32) test->oUnusedBhvParams, G_EX_PUSH,
                        //                               G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
                        //recomp_printf("GRAPH_NODE_TYPE_OBJECT: %d\n", test->oUnusedBhvParams);
                        geo_process_object((struct Object*) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_ANIMATED_PART:
                        geo_process_animated_part((struct GraphNodeAnimatedPart*) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_BILLBOARD:
                        geo_process_billboard((struct GraphNodeBillboard*) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_DISPLAY_LIST:
                        geo_process_display_list((struct GraphNodeDisplayList*) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_SCALE:
                        geo_process_scale((struct GraphNodeScale*) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_SHADOW:
                        geo_process_shadow((struct GraphNodeShadow*) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_OBJECT_PARENT:
                        geo_process_object_parent((struct GraphNodeObjectParent*) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_GENERATED_LIST:
                        geo_process_generated_list((struct GraphNodeGenerated*) curGraphNode);

                        break;
                    case GRAPH_NODE_TYPE_BACKGROUND:
                        geo_process_background((struct GraphNodeBackground*) curGraphNode);
                        break;
                    case GRAPH_NODE_TYPE_HELD_OBJ:
                        geo_process_held_object((struct GraphNodeHeldObject*) curGraphNode);
                        break;
                    default:
                        geo_try_process_children((struct GraphNode*) curGraphNode);
                        break;
                }
            }

        } else {
            if (curGraphNode->type == GRAPH_NODE_TYPE_OBJECT) {
                ((struct GraphNodeObject*) curGraphNode)->throwMatrix = NULL;
            }
        }
    } while (iterateChildren && (curGraphNode = curGraphNode->next) != firstNode);
}
#endif

#if 0
RECOMP_PATCH void spawn_objects_from_info(s32 unused, struct SpawnInfo* spawnInfo) __attribute__((optnone)) {
    gObjectLists = gObjectListArray;
    gTimeStopState = 0;

    gWDWWaterLevelChanging = FALSE;
    gMarioOnMerryGoRound = FALSE;

    //! (Spawning Displacement) On the Japanese version, Mario's platform object
    //  isn't cleared when transitioning between areas. This can cause Mario to
    //  receive displacement after spawning.
    clear_mario_platform();

    if (gCurrAreaIndex == 2) {
        gCCMEnteredSlide |= 1;
    }

    while (spawnInfo != NULL) {
        struct Object* object;
        UNUSED u8 filler[4];
        const BehaviorScript* script;
        UNUSED s16 arg16 = (s16) (spawnInfo->behaviorArg & 0xFFFF);

        script = segmented_to_virtual(spawnInfo->behaviorScript);

        // If the object was previously killed/collected, don't respawn it
        if ((spawnInfo->behaviorArg & (RESPAWN_INFO_DONT_RESPAWN << 8)) != (RESPAWN_INFO_DONT_RESPAWN << 8)) {
            object = create_object(script);

            // Behavior parameters are often treated as four separate bytes, but
            // are stored as an s32.
            object->oBhvParams = spawnInfo->behaviorArg;
            // The second byte of the behavior parameters is copied over to a special field
            // as it is the most frequently used by objects.
            object->oBhvParams2ndByte = ((spawnInfo->behaviorArg) >> 16) & 0xFF;

            object->behavior = script;
            // object->oUnusedBhvParams = createObjId();
            object->unused1 = 0;
            // Record death/collection in the SpawnInfo
            object->respawnInfoType = RESPAWN_INFO_TYPE_32;
            object->respawnInfo = &spawnInfo->behaviorArg;

            if (spawnInfo->behaviorArg & 0x01) {
                gMarioObject = object;
                geo_make_first_child(&object->header.gfx.node);
            }

            geo_obj_init_spawninfo(&object->header.gfx, spawnInfo);

            object->oPosX = spawnInfo->startPos[0];
            object->oPosY = spawnInfo->startPos[1];
            object->oPosZ = spawnInfo->startPos[2];

            object->oFaceAnglePitch = spawnInfo->startAngle[0];
            object->oFaceAngleYaw = spawnInfo->startAngle[1];
            object->oFaceAngleRoll = spawnInfo->startAngle[2];

            object->oMoveAnglePitch = spawnInfo->startAngle[0];
            object->oMoveAngleYaw = spawnInfo->startAngle[1];
            object->oMoveAngleRoll = spawnInfo->startAngle[2];
        }

        spawnInfo = spawnInfo->next;
    }
}
#endif

#if 0
RECOMP_PATCH struct Object* create_object(const BehaviorScript* bhvScript) {
    s32 objListIndex;
    struct Object* obj;
    struct ObjectNode* objList;
    const BehaviorScript* behavior = bhvScript;

    // If the first behavior script command is "begin <object list>", then
    // extract the object list from it
    if ((bhvScript[0] >> 24) == 0) {
        objListIndex = (bhvScript[0] >> 16) & 0xFFFF;
    } else {
        objListIndex = OBJ_LIST_DEFAULT;
    }

    objList = &gObjectLists[objListIndex];
    obj = allocate_object(objList);

    obj->curBhvCommand = bhvScript;
    obj->behavior = behavior;
    obj->oUnusedBhvParams = createObjId();

    if (objListIndex == OBJ_LIST_UNIMPORTANT) {
        obj->activeFlags |= ACTIVE_FLAG_UNIMPORTANT;
    }

    //! They intended to snap certain objects to the floor when they spawn.
    //  However, at this point the object's position is the origin. So this will
    //  place the object at the floor beneath the origin. Typically this
    //  doesn't matter since the caller of this function sets oPosX/Y/Z
    //  themselves.
    switch (objListIndex) {
        case OBJ_LIST_GENACTOR:
        case OBJ_LIST_PUSHABLE:
        case OBJ_LIST_POLELIKE:
            snap_object_to_floor(obj);
            break;
        default:
            break;
    }

    return obj;
}
#endif

#if 0
RECOMP_PATCH void geo_process_master_list_sub(struct GraphNodeMasterList* node) {
    struct DisplayListNode* currList;
    s32 i;
    s32 enableZBuffer = (node->node.flags & GRAPH_RENDER_Z_BUFFER) != 0;
    struct RenderModeContainer* modeList = &renderModeTable_1Cycle[enableZBuffer];
    struct RenderModeContainer* mode2List = &renderModeTable_2Cycle[enableZBuffer];

    // @bug This is where the LookAt values should be calculated but aren't.
    // As a result, environment mapping is broken on Fast3DEX2 without the
    // changes below.
#ifdef F3DEX_GBI_2
    Mtx lMtx;
    guLookAtReflect(&lMtx, &lookAt, 0, 0, 0, /* eye */ 0, 0, 1, /* at */ 1, 0, 0 /* up */);
#endif

    if (enableZBuffer != 0) {
        gDPPipeSync(gDisplayListHead++);
        gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER);
    }

    for (i = 0; i < GFX_NUM_MASTER_LISTS; i++) {
        if ((currList = node->listHeads[i]) != NULL) {
            gDPSetRenderMode(gDisplayListHead++, modeList->modes[i], mode2List->modes[i]);
            while (currList != NULL) {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gDisplayListHead++, (u32) node->listHeads[i], G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);

                gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(currList->transform),
                          G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
                gSPDisplayList(gDisplayListHead++, currList->displayList);
                currList = currList->next;

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
            }
        }
    }
    if (enableZBuffer != 0) {
        gDPPipeSync(gDisplayListHead++);
        gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER);
    }
}

#endif

#if 0
RECOMP_PATCH void make_viewport_clip_rect(Vp* viewport) {
    s16 vpUlx = (viewport->vp.vtrans[0] - viewport->vp.vscale[0]) / 4 + 1;
    s16 vpPly = (viewport->vp.vtrans[1] - viewport->vp.vscale[1]) / 4 + 1;
    s16 vpLrx = (viewport->vp.vtrans[0] + viewport->vp.vscale[0]) / 4 - 1;
    s16 vpLry = (viewport->vp.vtrans[1] + viewport->vp.vscale[1]) / 4 - 1;

   gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, vpUlx, vpPly, vpLrx, vpLry);
}
#endif