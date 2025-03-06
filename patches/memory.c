#include "patches.h"

#if 0

struct MemoryPool* gObjectMemoryPoolExpanded;
struct Object gObjectPoolExpanded[OBJECT_POOL_CAPACITY_EXPANDED];

extern s16 gTHIWaterDrained;
extern u32 gTimeStopState;
extern struct Object* gMarioObject;
extern s16 gMarioCurrentRoom;

RECOMP_PATCH void clear_objects(void) {
    s32 i;

    gTHIWaterDrained = 0;
    gTimeStopState = 0;
    gMarioObject = NULL;
    gMarioCurrentRoom = 0;

    for (i = 0; i < 60; i++) {
        gDoorAdjacentRooms[i][0] = 0;
        gDoorAdjacentRooms[i][1] = 0;
    }

    debug_unknown_level_select_check();

    init_free_object_list();
    clear_object_lists(gObjectListArray);

    stub_behavior_script_2();
    stub_obj_list_processor_1();

    for (i = 0; i < OBJECT_POOL_CAPACITY; i++) {
        gObjectPoolExpanded[i].activeFlags = ACTIVE_FLAG_DEACTIVATED;
        geo_reset_object_node(&gObjectPoolExpanded[i].header.gfx);
    }

    gObjectMemoryPoolExpanded = mem_pool_init(0x800*4, MEMORY_POOL_LEFT);
    gObjectLists = gObjectListArray;

    clear_dynamic_surfaces();
}

RECOMP_PATCH void init_free_object_list(void) {
    s32 i;
    s32 poolLength = OBJECT_POOL_CAPACITY;

    // Add the first object in the pool to the free list
    struct Object* obj = &gObjectPoolExpanded[0];
    gFreeObjectList.next = (struct ObjectNode*) obj;

    // Link each object in the pool to the following object
    for (i = 0; i < poolLength - 1; i++) {
        obj->header.next = &(obj + 1)->header;
        obj++;
    }

    // End the list
    obj->header.next = NULL;
}

#define MODEL_METALLIC_BALL 0x65 // metallic_ball_geo

RECOMP_PATCH void chain_chomp_act_uninitialized(void) {
    struct ChainSegment* segments;
    s32 i;

    if (o->oDistanceToMario < 3000.0f || 1) {
        segments = mem_pool_alloc(gObjectMemoryPoolExpanded, 5 * sizeof(struct ChainSegment));
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

#define MODEL_WIGGLER_BODY 0x58 // wiggler_body_geo
extern const BehaviorScript bhvWigglerBody[];
RECOMP_PATCH void wiggler_init_segments(void) {
    s32 i;
    struct ChainSegment* segments = mem_pool_alloc(gObjectMemoryPoolExpanded, 4 * sizeof(struct ChainSegment));

    if (segments != NULL) {
        // Each segment represents the global position and orientation of each
        // object. Segment 0 represents the wiggler's head, and segment i>0
        // represents body part i.
        o->oWigglerSegments = segments;
        for (i = 0; i <= 3; i++) {
            chain_segment_init(segments + i);

            (segments + i)->posX = o->oPosX;
            (segments + i)->posY = o->oPosY;
            (segments + i)->posZ = o->oPosZ;

            (segments + i)->pitch = o->oFaceAnglePitch;
            (segments + i)->yaw = o->oFaceAngleYaw;
        }

        o->header.gfx.animInfo.animFrame = -1;

        // Spawn each body part
        for (i = 1; i <= 3; i++) {
            struct Object* bodyPart = spawn_object_relative(i, 0, 0, 0, o, MODEL_WIGGLER_BODY, bhvWigglerBody);
            if (bodyPart != NULL) {
                obj_init_animation_with_sound(bodyPart, (struct Animation*) 0x0500C874, 0);
                bodyPart->header.gfx.animInfo.animFrame = (23 * i) % 26 - 1;
            }
        }

        o->oAction = WIGGLER_ACT_WALK;
        cur_obj_unhide();
    }

    // #if defined(VERSION_EU) || defined(AVOID_UB)
    o->oHealth = 4; // This fixes Wiggler reading UB on his first frame of his acceleration, as his
                    // health is not set.
    // #endif
}
    #endif 