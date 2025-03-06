#include "patches.h"


// Draw Distance (need memory increased)
#if 1

// load_object_collision_model (moving platforms, posts, signs) 
#if 1
RECOMP_PATCH void load_object_collision_model(void) {
    UNUSED u8 filler[4];
    TerrainData vertexData[600];

    TerrainData* collisionData = gCurrentObject->collisionData;
    f32 marioDist = gCurrentObject->oDistanceToMario;
    f32 tangibleDist = gCurrentObject->oCollisionDistance;

    // On an object's first frame, the distance is set to 19000.0f.
    // If the distance hasn't been updated, update it now.
    if (gCurrentObject->oDistanceToMario == 19000.0f) {
        marioDist = dist_between_objects(gCurrentObject, gMarioObject);
    }

    // If the object collision is supposed to be loaded more than the
    // drawing distance of 4000, extend the drawing range.
    if (gCurrentObject->oCollisionDistance > 4000.0f) {
        gCurrentObject->oDrawingDistance = gCurrentObject->oCollisionDistance;
    }

    // Update if no Time Stop, in range, and in the current room.
    if (!(gTimeStopState & TIME_STOP_ACTIVE) && marioDist < tangibleDist &&
        !(gCurrentObject->activeFlags & ACTIVE_FLAG_IN_DIFFERENT_ROOM)) {
        collisionData++;
        transform_object_vertices(&collisionData, vertexData);

        // TERRAIN_LOAD_CONTINUE acts as an "end" to the terrain data.
        while (*collisionData != TERRAIN_LOAD_CONTINUE) {
            load_object_surfaces(&collisionData, vertexData);
        }
    }

    if (marioDist < gCurrentObject->oDrawingDistance * 3) { // increase draw distance
        gCurrentObject->header.gfx.node.flags |= GRAPH_RENDER_ACTIVE;
    } else {
        gCurrentObject->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
    }
}

#endif

// bhv_goomba_triplet_spawner_update
#if 1
RECOMP_PATCH void bhv_goomba_triplet_spawner_update(void) {
    UNUSED u8 filler1[4];
    s16 goombaFlag;
    UNUSED u8 filler2[2];
    s32 angle;

    // If mario is close enough and the goombas aren't currently loaded, then
    // spawn them
    if (o->oAction == GOOMBA_TRIPLET_SPAWNER_ACT_UNLOADED) {
        if (o->oDistanceToMario < 3000.0f * 3) { // increase draw distance
            // The spawner is capable of spawning more than 3 goombas, but this
            // is not used in the game
            s32 dAngle = 0x10000 / (((o->oBhvParams2ndByte & GOOMBA_TRIPLET_SPAWNER_BP_EXTRA_GOOMBAS_MASK) >> 2) + 3);

            for (angle = 0, goombaFlag = 1 << 8; angle < 0xFFFF; angle += dAngle, goombaFlag <<= 1) {
                // Only spawn goombas which haven't been killed yet
                if (!(o->oBhvParams & goombaFlag)) {
                    s16 dx = 500.0f * coss(angle);
                    s16 dz = 500.0f * sins(angle);

                    spawn_object_relative((o->oBhvParams2ndByte & GOOMBA_BP_SIZE_MASK) | (goombaFlag >> 6), dx, 0, dz,
                                          o, MODEL_GOOMBA, bhvGoomba);
                }
            }

            o->oAction++;
        }
    } else if (o->oDistanceToMario > 4000.0f && 0) { // remove unspawn
        // If mario is too far away, enter the unloaded action. The goombas
        // will detect this and unload themselves
        o->oAction = GOOMBA_TRIPLET_SPAWNER_ACT_UNLOADED;
    }
}
#endif

// set_object_visibility (pink bobombs, large rolling balls)
#if 1
RECOMP_PATCH void set_object_visibility(struct Object* obj, s32 dist) {
    f32 objX = obj->oPosX;
    f32 objY = obj->oPosY;
    f32 objZ = obj->oPosZ;

    if (is_point_within_radius_of_mario(objX, objY, objZ, dist) == TRUE) {
        obj->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
    } else {
        //obj->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
    }
}
#endif

// chain_chomp_act_move // chain_chomp_act_uninitialized
#if 1
RECOMP_PATCH void chain_chomp_act_move(void) {
    f32 maxDistToPivot;

    // Unload chain if mario is far enough
    if ((o->oChainChompReleaseStatus == CHAIN_CHOMP_NOT_RELEASED && o->oDistanceToMario > 4000.0f * 3)) {
        o->oAction = CHAIN_CHOMP_ACT_UNLOAD_CHAIN;
        o->oForwardVel = o->oVelY = 0.0f;
    } else {
        cur_obj_update_floor_and_walls();

        switch (o->oChainChompReleaseStatus) {
            case CHAIN_CHOMP_NOT_RELEASED:
                switch (o->oSubAction) {
                    case CHAIN_CHOMP_SUB_ACT_TURN:
                        chain_chomp_sub_act_turn();
                        break;
                    case CHAIN_CHOMP_SUB_ACT_LUNGE:
                        chain_chomp_sub_act_lunge();
                        break;
                }
                break;
            case CHAIN_CHOMP_RELEASED_TRIGGER_CUTSCENE:
                chain_chomp_released_trigger_cutscene();
                break;
            case CHAIN_CHOMP_RELEASED_LUNGE_AROUND:
                chain_chomp_released_lunge_around();
                break;
            case CHAIN_CHOMP_RELEASED_BREAK_GATE:
                chain_chomp_released_break_gate();
                break;
            case CHAIN_CHOMP_RELEASED_JUMP_AWAY:
                chain_chomp_released_jump_away();
                break;
            case CHAIN_CHOMP_RELEASED_END_CUTSCENE:
                chain_chomp_released_end_cutscene();
                break;
        }

        cur_obj_move_standard(78);

        // Segment 0 connects the pivot to the chain chomp itself
        o->oChainChompSegments[0].posX = o->oPosX - o->parentObj->oPosX;
        o->oChainChompSegments[0].posY = o->oPosY - o->parentObj->oPosY;
        o->oChainChompSegments[0].posZ = o->oPosZ - o->parentObj->oPosZ;

        o->oChainChompDistToPivot = sqrtf(o->oChainChompSegments[0].posX * o->oChainChompSegments[0].posX +
                                          o->oChainChompSegments[0].posY * o->oChainChompSegments[0].posY +
                                          o->oChainChompSegments[0].posZ * o->oChainChompSegments[0].posZ);

        // If the chain is fully stretched
        maxDistToPivot = o->oChainChompMaxDistFromPivotPerChainPart * 5;
        if (o->oChainChompDistToPivot > maxDistToPivot) {
            f32 ratio = maxDistToPivot / o->oChainChompDistToPivot;
            o->oChainChompDistToPivot = maxDistToPivot;

            o->oChainChompSegments[0].posX *= ratio;
            o->oChainChompSegments[0].posY *= ratio;
            o->oChainChompSegments[0].posZ *= ratio;

            if (o->oChainChompReleaseStatus == CHAIN_CHOMP_NOT_RELEASED) {
                // Restrict chain chomp position
                o->oPosX = o->parentObj->oPosX + o->oChainChompSegments[0].posX;
                o->oPosY = o->parentObj->oPosY + o->oChainChompSegments[0].posY;
                o->oPosZ = o->parentObj->oPosZ + o->oChainChompSegments[0].posZ;

                o->oChainChompRestrictedByChain = TRUE;
            } else {
                // Move pivot like the chain chomp is pulling it along
                f32 oldPivotY = o->parentObj->oPosY;

                o->parentObj->oPosX = o->oPosX - o->oChainChompSegments[0].posX;
                o->parentObj->oPosY = o->oPosY - o->oChainChompSegments[0].posY;
                o->parentObj->oVelY = o->parentObj->oPosY - oldPivotY;
                o->parentObj->oPosZ = o->oPosZ - o->oChainChompSegments[0].posZ;
            }
        } else {
            o->oChainChompRestrictedByChain = FALSE;
        }

        chain_chomp_update_chain_segments();

        // Begin a lunge if mario tries to attack
        if (obj_check_attacks(&sChainChompHitbox, o->oAction) != 0) {
            o->oSubAction = CHAIN_CHOMP_SUB_ACT_LUNGE;
            o->oChainChompMaxDistFromPivotPerChainPart = 900.0f / 5;
            o->oForwardVel = 0.0f;
            o->oVelY = 300.0f;
            o->oGravity = -4.0f;
            o->oChainChompTargetPitch = -0x3000;
        }
    }
}

RECOMP_PATCH void chain_chomp_act_uninitialized(void) {
    struct ChainSegment* segments;
    s32 i;

    if (o->oDistanceToMario < 3000.0f || 1) { // increase draw distance
        segments = mem_pool_alloc(gObjectMemoryPool, 5 * sizeof(struct ChainSegment));
        if (segments != NULL) {
            // Each segment represents the offset of a chain part to the pivot.
            // Segment 0 connects the pivot to the chain chomp itself. Segment
            // 1 connects the pivot to the chain part next to the chain chomp
            // (chain part 1), etc.
            o->oChainChompSegments = segments;
            for (i = 0; i <= 4; i++) {
                chain_segment_init(&segments[i]);
            }

            cur_obj_set_pos_to_home();

            // Spawn the pivot and set to parent
            if ((o->parentObj = spawn_object(o, CHAIN_CHOMP_CHAIN_PART_BP_PIVOT, bhvChainChompChainPart)) != NULL) {
                // Spawn the non-pivot chain parts, starting from the chain
                // chomp and moving toward the pivot
                for (i = 1; i <= 4; i++) {
                    spawn_object_relative(i, 0, 0, 0, o, MODEL_METALLIC_BALL, bhvChainChompChainPart);
                }

                o->oAction = CHAIN_CHOMP_ACT_MOVE;
                cur_obj_unhide();
            }
        }
    }
}
#endif

// bhv_coin_formation_loop
#if 1
RECOMP_PATCH void bhv_coin_formation_loop(void) {
    s32 coinIndex;

    switch (o->oAction) {
        case COIN_FORMATION_ACT_SPAWN_COINS:
            if (o->oDistanceToMario < 2000.0f * 6) {
                for (coinIndex = 0; coinIndex <= 7; coinIndex++) {
                    if (!(o->oCoinCollectedFlags & (1 << coinIndex))) {
                        spawn_coin_in_formation(coinIndex, o->oBhvParams2ndByte);
                    }
                }
                o->oAction++;
            }
            break;

        case COIN_FORMATION_ACT_IDLE:
            if (o->oDistanceToMario > 2100.0f * 6) {
                o->oAction++;
            }
            break;

        case COIN_FORMATION_ACT_RESPAWN_COINS:
            o->oAction = COIN_FORMATION_ACT_SPAWN_COINS;
            break;
    }

    set_object_respawn_info_bits(o, o->oCoinCollectedFlags & 0xFF);
}
#endif

// cur_obj_update (single goombas, bobombs)
#if 1
RECOMP_PATCH void cur_obj_update(void) {
    UNUSED u8 filler[4];

    s16 objFlags = gCurrentObject->oFlags;
    f32 distanceFromMario;
    BhvCommandProc bhvCmdProc;
    s32 bhvProcResult;

    // Calculate the distance from the object to Mario.
    if (objFlags & OBJ_FLAG_COMPUTE_DIST_TO_MARIO) {
        gCurrentObject->oDistanceToMario = dist_between_objects(gCurrentObject, gMarioObject);
        distanceFromMario = gCurrentObject->oDistanceToMario;
    } else {
        distanceFromMario = 0.0f;
    }

    // Calculate the angle from the object to Mario.
    if (objFlags & OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO) {
        gCurrentObject->oAngleToMario = obj_angle_to_object(gCurrentObject, gMarioObject);
    }

    // If the object's action has changed, reset the action timer.
    if (gCurrentObject->oAction != gCurrentObject->oPrevAction) {
        (void) (gCurrentObject->oTimer = 0, gCurrentObject->oSubAction = 0,
                gCurrentObject->oPrevAction = gCurrentObject->oAction);
    }

    // Execute the behavior script.
    gCurBhvCommand = gCurrentObject->curBhvCommand;

    do {
        bhvCmdProc = BehaviorCmdTable[*gCurBhvCommand >> 24];
        bhvProcResult = bhvCmdProc();
    } while (bhvProcResult == BHV_PROC_CONTINUE);

    gCurrentObject->curBhvCommand = gCurBhvCommand;

    // Increment the object's timer.
    if (gCurrentObject->oTimer < 0x3FFFFFFF) {
        gCurrentObject->oTimer++;
    }

    // If the object's action has changed, reset the action timer.
    if (gCurrentObject->oAction != gCurrentObject->oPrevAction) {
        (void) (gCurrentObject->oTimer = 0, gCurrentObject->oSubAction = 0,
                gCurrentObject->oPrevAction = gCurrentObject->oAction);
    }

    // Execute various code based on object flags.
    objFlags = (s16) gCurrentObject->oFlags;

    if (objFlags & OBJ_FLAG_SET_FACE_ANGLE_TO_MOVE_ANGLE) {
        obj_set_face_angle_to_move_angle(gCurrentObject);
    }

    if (objFlags & OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW) {
        gCurrentObject->oFaceAngleYaw = gCurrentObject->oMoveAngleYaw;
    }

    if (objFlags & OBJ_FLAG_MOVE_XZ_USING_FVEL) {
        cur_obj_move_xz_using_fvel_and_yaw();
    }

    if (objFlags & OBJ_FLAG_MOVE_Y_WITH_TERMINAL_VEL) {
        cur_obj_move_y_with_terminal_vel();
    }

    if (objFlags & OBJ_FLAG_TRANSFORM_RELATIVE_TO_PARENT) {
        obj_build_transform_relative_to_parent(gCurrentObject);
    }

    if (objFlags & OBJ_FLAG_SET_THROW_MATRIX_FROM_TRANSFORM) {
        obj_set_throw_matrix_from_transform(gCurrentObject);
    }

    if (objFlags & OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE) {
        obj_update_gfx_pos_and_angle(gCurrentObject);
    }

    // Handle visibility of object
    if (gCurrentObject->oRoom != -1) {
        // If the object is in a room, only show it when Mario is in the room.
        cur_obj_enable_rendering_if_mario_in_room();
    } else if ((objFlags & OBJ_FLAG_COMPUTE_DIST_TO_MARIO) && gCurrentObject->collisionData == NULL) {
        if (!(objFlags & OBJ_FLAG_ACTIVE_FROM_AFAR)) {
            // If the object has a render distance, check if it should be shown.
            if (distanceFromMario > gCurrentObject->oDrawingDistance * 3) { // Increased draw distance
                // Out of render distance, hide the object.
                gCurrentObject->header.gfx.node.flags &= ~GRAPH_RENDER_ACTIVE;
                gCurrentObject->activeFlags |= ACTIVE_FLAG_FAR_AWAY;
            } else if (gCurrentObject->oHeldState == HELD_FREE) {
                // In render distance (and not being held), show the object.
                gCurrentObject->header.gfx.node.flags |= GRAPH_RENDER_ACTIVE;
                gCurrentObject->activeFlags &= ~ACTIVE_FLAG_FAR_AWAY;
            }
        }
    }
}
#endif

// Water Cannon DD
#if 1
RECOMP_PATCH void water_bomb_cannon_act_0(void) {
    if (o->oDistanceToMario < 2000.0f * 8) { // increased draw distance
        spawn_object(o, MODEL_CANNON_BARREL, bhvCannonBarrelBubbles);
        cur_obj_unhide();

        o->oAction = 1;
        o->oMoveAnglePitch = o->oWaterCannonUnkFC = 0x1C00;
    }
}

RECOMP_PATCH void water_bomb_cannon_act_1(void) {
    if (o->oDistanceToMario > 2500.0f *8) { // increase draw distance
        o->oAction = 2;
    } else if (o->oBhvParams2ndByte == 0) {
        if (o->oWaterCannonUnkF4 != 0) {
            o->oWaterCannonUnkF4--;
        } else {
            obj_move_pitch_approach(o->oWaterCannonUnkFC, 0x80);
            obj_face_yaw_approach(o->oWaterCannonUnk100, 0x100);

            if ((s16) o->oFaceAngleYaw == (s16) o->oWaterCannonUnk100) {
                if (o->oWaterCannonUnkF8 != 0) {
                    o->oWaterCannonUnkF8--;
                } else {
                    cur_obj_play_sound_2(SOUND_OBJ_CANNON4);
                    o->oWaterCannonUnkF4 = 70;
                    o->oWaterCannonUnkFC = 0x1000 + 0x400 * (random_u16() & 0x03);
                    o->oWaterCannonUnk100 = -0x2000 + o->oMoveAngleYaw + 0x1000 * (random_u16() % 5);
                    o->oWaterCannonUnkF8 = 60;
                }
            }
        }
    }
}

#endif

// Mario LOD
#if 1
RECOMP_PATCH Gfx* geo_switch_mario_stand_run(s32 callContext, struct GraphNode* node, UNUSED Mat4* mtx) {
    // struct GraphNodeSwitchCase* switchCase = (struct GraphNodeSwitchCase*) node;
    // struct MarioBodyState* bodyState = &gBodyStates[switchCase->numCases];
    // return; // Disable LOD
    // if (callContext == GEO_CONTEXT_RENDER) {
    //     // assign result. 0 if moving, 1 if stationary.
    //     switchCase->selectedCase = ((bodyState->action & ACT_FLAG_STATIONARY) == 0);
    // }
    return NULL;
}
#endif

// Level Geometry LOD
#if 1
RECOMP_PATCH void geo_process_level_of_detail(struct GraphNodeLevelOfDetail* node) {
#ifdef GBI_FLOATS
    Mtx* mtx = gMatStackFixed[gMatStackIndex];
    s16 distanceFromCam = (s32) -mtx->m[3][2]; // z-component of the translation column
#else
    // The fixed point Mtx type is defined as 16 longs, but it's actually 16
    // shorts for the integer parts followed by 16 shorts for the fraction parts
    Mtx* mtx = gMatStackFixed[gMatStackIndex];
    // s16 distanceFromCam = -GET_HIGH_S16_OF_32(mtx->m[1][3]); // z-component of the translation column
    s16 distanceFromCam = 0.0f; // Increase Level of Detail
#endif

    if (node->minDistance <= distanceFromCam && distanceFromCam < node->maxDistance) {
        if (node->node.children != 0) {
            geo_process_node_and_siblings(node->node.children);
        }
    }
}
#endif

// Draw Distance Testing (not needed???)
#if 0 
// bhv_goomba_init
#if 0
RECOMP_PATCH void bhv_goomba_init(void) {
    o->oGoombaSize = o->oBhvParams2ndByte & GOOMBA_BP_SIZE_MASK;

    o->oGoombaScale = sGoombaProperties[o->oGoombaSize].scale;
    o->oDeathSound = sGoombaProperties[o->oGoombaSize].deathSound;

    obj_set_hitbox(o, &sGoombaHitbox);

    o->oDrawingDistance = sGoombaProperties[o->oGoombaSize].drawDistance * 3; // improved Draw Distance.
    o->oDamageOrCoinValue = sGoombaProperties[o->oGoombaSize].damage;

    o->oGravity = -8.0f / 3.0f * o->oGoombaScale;
}
#endif

// bobomb_buddy_act_idle
#if 0
RECOMP_PATCH void bobomb_buddy_act_idle(void) {
    UNUSED u8 filler[4];
    s16 animFrame = o->header.gfx.animInfo.animFrame;
    UNUSED s16 collisionFlags = 0;

    o->oBobombBuddyPosXCopy = o->oPosX;
    o->oBobombBuddyPosYCopy = o->oPosY;
    o->oBobombBuddyPosZCopy = o->oPosZ;

    collisionFlags = object_step();

    if (animFrame == 5 || animFrame == 16) {
        cur_obj_play_sound_2(SOUND_OBJ_BOBOMB_WALK);
    }

    if (o->oDistanceToMario < 1000.0f * 3) {
        o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oAngleToMario, 0x140);
    }

    if (o->oInteractStatus == INT_STATUS_INTERACTED) {
        o->oAction = BOBOMB_BUDDY_ACT_TURN_TO_TALK;
    }
}
#endif

// bobomb_buddy_actions  (no change yet)
#if 0
RECOMP_PATCH void bobomb_buddy_actions(void) {
    switch (o->oAction) {
        case BOBOMB_BUDDY_ACT_IDLE:
            bobomb_buddy_act_idle();
            break;

        case BOBOMB_BUDDY_ACT_TURN_TO_TALK:
            bobomb_buddy_act_turn_to_talk();
            break;

        case BOBOMB_BUDDY_ACT_TALK:
            bobomb_buddy_act_talk();
            break;
    }

    set_object_visibility(o, 3000);
}
#endif

// bhv_koopa_init
#if 0
RECOMP_PATCH void bhv_koopa_init(void) {
    if ((o->oKoopaMovementType = o->oBhvParams2ndByte) == KOOPA_BP_TINY) {
        // Tiny koopa in THI
        o->oKoopaMovementType = KOOPA_BP_NORMAL;
        o->oKoopaAgility = 1.6f / 3.0f;
        o->oDrawingDistance = 1500.0f;
        cur_obj_scale(0.8f);
        o->oGravity = -6.4f / 3.0f;
    } else if (o->oKoopaMovementType >= KOOPA_BP_KOOPA_THE_QUICK_BASE) {
        // Koopa the Quick. Race index is 0 for BoB and 1 for THI
        o->oKoopaTheQuickRaceIndex = o->oKoopaMovementType - KOOPA_BP_KOOPA_THE_QUICK_BASE;
        o->oKoopaAgility = 4.0f;
        cur_obj_scale(3.0f);
    } else {
        o->oKoopaAgility = 1.0f;
    }
}
#endif

#endif

#endif // END