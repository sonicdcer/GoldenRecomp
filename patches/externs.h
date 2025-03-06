#ifndef EXTERNS_H
#define EXTERNS_H

#include "patches.h"

#define INTERACT_MR_BLIZZARD /* 0x00200000 */ (1 << 21)
#define MODEL_CANNON_BARREL 0x7F // cannon_barrel_geo
#define BHV_PROC_CONTINUE 0
#define MODEL_METALLIC_BALL 0x65        // metallic_ball_geo
#define INT_STATUS_INTERACTED (1 << 15) /* 0x00008000 */
#define TIME_STOP_ACTIVE (1 << 6)
#define INTERACT_BOUNCE_TOP /* 0x00008000 */ (1 << 15)
#define TERRAIN_LOAD_CONTINUE 0x0041 // Stop loading vertices but continues to load other collision commands
#define GET_HIGH_S16_OF_32(var) (((s16*) &(var))[0])
#define MODEL_GOOMBA 0xC0 // goomba_geo

#define CAMERA_MODE_INSIDE_CANNON 0x0A
#define CAM_STATUS_NONE 0
#define CAM_STATUS_MODE_GROUP (CAM_STATUS_MARIO | CAM_STATUS_LAKITU | CAM_STATUS_FIXED)
#define CAM_STATUS_C_MODE_GROUP (CAM_STATUS_C_DOWN | CAM_STATUS_C_UP)
#define CAM_STATUS_NONE 0
#define CAM_STATUS_MARIO 1 << 0
#define CAM_STATUS_LAKITU 1 << 1
#define CAM_STATUS_FIXED 1 << 2
#define CAM_STATUS_C_DOWN 1 << 3
#define CAM_STATUS_C_UP 1 << 4
#define main_hud_lut (u8*) 0x2007700
#define TEXRECT_MIN_X 10
#define TEXRECT_MAX_X 300
#define TEXRECT_MIN_Y 5
#define TEXRECT_MAX_Y 220
#define HUD_COINS_X 168
#define GLYPH_APOSTROPHE 56
#define GLYPH_DOUBLE_QUOTE 57
#define main_hud_camera_lut (Texture**) 0x2007c7c
#define dl_hud_img_begin (Gfx*) 0x2011ac0
#define dl_hud_img_end (Gfx*) 0x2011b28
#define dl_hud_img_load_tex_block (Gfx*) 0x2011af8
#define HUD_TOP_Y 209
#define HUD_STARS_X 78

#define dl_draw_quad_verts_0123 (Gfx*) 0x02017350
#define dl_proj_mtx_fullscreen (Gfx*) 0x020174c0
#define dl_screen_transition_end (Gfx*) 0x020174f8

#define o gCurrentObject

#define dl_draw_text_bg_box (Gfx*) 0x02011c48

#define DIAG_VAL2 240
#define DIAG_VAL4 5

#define MENU_MTX_PUSH 1
#define MENU_MTX_NOPUSH 2

#define VERSION_US
#define DIALOG_BOX_ANGLE_DEFAULT 90.0f
#define DIALOG_BOX_SCALE_DEFAULT 19.0f
#define DIAG_VAL1 16
#define SOUND_BANK_MENU 7
#define SOUND_DISCRETE 0x80 // Every call to play_sound restarts the sound. If not
#define SOUND_MENU_MESSAGE_APPEAR /* 0x70040080 */ SOUND_ARG_LOAD(SOUND_BANK_MENU, 0x04, 0x00, SOUND_DISCRETE)

#define OBJECT_POOL_CAPACITY 240
#define OBJECT_POOL_CAPACITY_EXPANDED 1000
#define MEMORY_POOL_LEFT 0
#define MEMORY_POOL_RIGHT 1

#define MIN_ASPECT_RATIO (4.0f / 3.0f)
#define CLAMP_ASPECT_RATIO(a) ((a) < MIN_ASPECT_RATIO ? MIN_ASPECT_RATIO : (a))

#define GFX_DIMENSIONS_ASPECT_RATIO CLAMP_ASPECT_RATIO(recomp_get_aspect_ratio(0.0f))

#define FLOORF(x) ((x) < 0 ? (int) (x) - 1 : (int) (x))
#define CEILF(x) ((x) > (int) (x) ? (int) (x) + 1 : (int) (x))

#define GFX_DIMENSIONS_FROM_LEFT_EDGE(v) (v)
#define GFX_DIMENSIONS_FROM_RIGHT_EDGE(v) (SCREEN_WIDTH - (v))
#define GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(v) (v)
#define GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(v) (SCREEN_WIDTH - (v))

#define GEO_CONTEXT_CREATE 0      // called when node is created from a geo command
#define GEO_CONTEXT_RENDER 1      // called from rendering_graph_node.c
#define GEO_CONTEXT_AREA_UNLOAD 2 // called when unloading an area
#define GEO_CONTEXT_AREA_LOAD 3   // called when loading an area
#define GEO_CONTEXT_AREA_INIT 4   // called when initializing the 8 areas
#define GEO_CONTEXT_HELD_OBJ 5    // called when processing a GraphNodeHeldObj
#define GFX_NUM_MASTER_LISTS 8

// Passed as first argument to a GraphNodeFunc to give information about in
// which context it was called and what it is expected to do.
#define GEO_CONTEXT_CREATE 0      // called when node is created from a geo command
#define GEO_CONTEXT_RENDER 1      // called from rendering_graph_node.c
#define GEO_CONTEXT_AREA_UNLOAD 2 // called when unloading an area
#define GEO_CONTEXT_AREA_LOAD 3   // called when loading an area
#define GEO_CONTEXT_AREA_INIT 4   // called when initializing the 8 areas
#define GEO_CONTEXT_HELD_OBJ 5    // called when processing a GraphNodeHeldObj

#define GRAPH_NODE_TYPE_FUNCTIONAL 0x100
#define GRAPH_NODE_TYPE_SWITCH_CASE (0x00C | GRAPH_NODE_TYPE_FUNCTIONAL)

#define GRAPH_RENDER_ACTIVE (1 << 0)
#define GRAPH_RENDER_CHILDREN_FIRST (1 << 1)
#define GRAPH_RENDER_BILLBOARD (1 << 2)
#define GRAPH_RENDER_Z_BUFFER (1 << 3)
#define GRAPH_RENDER_INVISIBLE (1 << 4)
#define GRAPH_RENDER_HAS_ANIMATION (1 << 5)

#define GRAPH_NODE_TYPE_ROOT 0x001
#define GRAPH_NODE_TYPE_ORTHO_PROJECTION 0x002
#define GRAPH_NODE_TYPE_PERSPECTIVE (0x003 | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_MASTER_LIST 0x004
#define GRAPH_NODE_TYPE_START 0x00A
#define GRAPH_NODE_TYPE_LEVEL_OF_DETAIL 0x00B
#define GRAPH_NODE_TYPE_SWITCH_CASE (0x00C | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_CAMERA (0x014 | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_TRANSLATION_ROTATION 0x015
#define GRAPH_NODE_TYPE_TRANSLATION 0x016
#define GRAPH_NODE_TYPE_ROTATION 0x017
#define GRAPH_NODE_TYPE_OBJECT 0x018
#define GRAPH_NODE_TYPE_ANIMATED_PART 0x019
#define GRAPH_NODE_TYPE_BILLBOARD 0x01A
#define GRAPH_NODE_TYPE_DISPLAY_LIST 0x01B
#define GRAPH_NODE_TYPE_SCALE 0x01C
#define GRAPH_NODE_TYPE_SHADOW 0x028
#define GRAPH_NODE_TYPE_OBJECT_PARENT 0x029
#define GRAPH_NODE_TYPE_GENERATED_LIST (0x02A | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_BACKGROUND (0x02C | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_HELD_OBJ (0x02E | GRAPH_NODE_TYPE_FUNCTIONAL)
#define GRAPH_NODE_TYPE_CULLING_RADIUS 0x02F
#define GRAPH_RENDER_INVISIBLE (1 << 4)
#define sins(x) gSineTable[(u16) (x) >> 4]
#define coss(x) gCosineTable[(u16) (x) >> 4]
#define GRAPH_NODE_TYPE_CULLING_RADIUS 0x02F
#define SEQUENCE_ARGS(priority, seqId) ((priority << 8) | seqId)
#define SEQ_PLAYER_SFX 2 // Sound effects

typedef uintptr_t LevelScript;
typedef s32 (*BhvCommandProc)(void);


s32 intro_play_its_a_me_mario(void);
s32 intro_regular(void);
s32 intro_game_over(void);
s32 intro_level_select(void);

uintptr_t set_segment_base_addr(s32 segment, void* addr);
s32 obj_move_pitch_approach(s16 target, s16 delta);
s32 obj_face_yaw_approach(s16 targetYaw, s16 deltaYaw);
u16 random_u16(void);
void cur_obj_enable_rendering_if_mario_in_room(void);
void cur_obj_move_xz_using_fvel_and_yaw(void);
void obj_set_face_angle_to_move_angle(struct Object* obj);
void cur_obj_move_y_with_terminal_vel(void);
s16 obj_angle_to_object(struct Object* obj1, struct Object* obj2);
void obj_build_transform_relative_to_parent(struct Object* obj);
void obj_update_gfx_pos_and_angle(struct Object* obj);
void obj_set_throw_matrix_from_transform(struct Object* obj);
void cur_obj_update_floor_and_walls(void);
void chain_chomp_sub_act_turn(void);
void chain_chomp_sub_act_lunge(void);
void chain_chomp_released_trigger_cutscene(void);
void chain_chomp_released_lunge_around(void);
void chain_chomp_released_jump_away(void);
void chain_chomp_released_end_cutscene(void);
void cur_obj_move_standard(s16 steepSlopeAngleDegrees);
void chain_chomp_released_break_gate(void);
void chain_chomp_update_chain_segments(void);
s32 obj_check_attacks(struct ObjectHitbox* hitbox, s32 attackedMarioAction);
void chain_segment_init(struct ChainSegment* segment);
void cur_obj_set_pos_to_home(void);
void cur_obj_unhide(void);
struct Object* spawn_object(struct Object* parent, s32 model, const BehaviorScript* behavior);
void set_object_respawn_info_bits(struct Object* obj, u8 bits);
void spawn_coin_in_formation(s32 coinIndex, s32 coinFormationFlags);
s16 object_step(void);
void cur_obj_play_sound_2(s32 soundMagic);
s16 approach_s16_symmetric(s16 value, s16 target, s16 increment);
void bobomb_buddy_act_turn_to_talk(void);
void bobomb_buddy_act_talk(void);
s8 is_point_within_radius_of_mario(f32 x, f32 y, f32 z, s32 dist);
void cur_obj_scale(f32 scale);
f32 dist_between_objects(struct Object* obj1, struct Object* obj2);
void transform_object_vertices(TerrainData** data, TerrainData* vertexData);
void load_object_surfaces(TerrainData** data, TerrainData* vertexData);
void bobomb_buddy_act_idle(void);
void set_object_visibility(struct Object* obj, s32 dist);
void obj_set_hitbox(struct Object* obj, struct ObjectHitbox* hitbox);
struct Object* spawn_object_relative(s16 behaviorParam, s16 relativePosX, s16 relativePosY, s16 relativePosZ,
                                     struct Object* parent, s32 model, const BehaviorScript* behavior);
struct Object* spawn_object_relative(s16 behaviorParam, s16 relativePosX, s16 relativePosY, s16 relativePosZ,
                                     struct Object* parent, s32 model, const BehaviorScript* behavior);
void cur_obj_enable_rendering_if_mario_in_room(void);
void cur_obj_move_xz_using_fvel_and_yaw(void);
void obj_set_face_angle_to_move_angle(struct Object* obj);
void cur_obj_move_y_with_terminal_vel(void);
s16 obj_angle_to_object(struct Object* obj1, struct Object* obj2);
void obj_build_transform_relative_to_parent(struct Object* obj);
void obj_update_gfx_pos_and_angle(struct Object* obj);
void obj_set_throw_matrix_from_transform(struct Object* obj);
void cur_obj_update_floor_and_walls(void);
void chain_chomp_sub_act_turn(void);
void chain_chomp_sub_act_lunge(void);
void chain_chomp_released_trigger_cutscene(void);
void chain_chomp_released_lunge_around(void);
void chain_chomp_released_jump_away(void);
void chain_chomp_released_end_cutscene(void);
void cur_obj_move_standard(s16 steepSlopeAngleDegrees);
void chain_chomp_released_break_gate(void);
void chain_chomp_update_chain_segments(void);
s32 obj_check_attacks(struct ObjectHitbox* hitbox, s32 attackedMarioAction);
void chain_segment_init(struct ChainSegment* segment);
void cur_obj_set_pos_to_home(void);
void cur_obj_unhide(void);
void* mem_pool_alloc(struct MemoryPool* pool, u32 size);
struct Object* spawn_object(struct Object* parent, s32 model, const BehaviorScript* behavior);
void set_object_respawn_info_bits(struct Object* obj, u8 bits);
void spawn_coin_in_formation(s32 coinIndex, s32 coinFormationFlags);
s16 object_step(void);
void cur_obj_play_sound_2(s32 soundMagic);
s16 approach_s16_symmetric(s16 value, s16 target, s16 increment);
void bobomb_buddy_act_turn_to_talk(void);
void bobomb_buddy_act_talk(void);
s8 is_point_within_radius_of_mario(f32 x, f32 y, f32 z, s32 dist);
void cur_obj_scale(f32 scale);
f32 dist_between_objects(struct Object* obj1, struct Object* obj2);
void transform_object_vertices(TerrainData** data, TerrainData* vertexData);
void load_object_surfaces(TerrainData** data, TerrainData* vertexData);
void bobomb_buddy_act_idle(void);
void set_object_visibility(struct Object* obj, s32 dist);
void obj_set_hitbox(struct Object* obj, struct ObjectHitbox* hitbox);
void* alloc_display_list(u32 size);
void setup_game_memory(void);
void init_controllers(void);
void save_file_load_all(void);
void play_music(u8 player, u16 seqArgs, u16 fadeTimer);
void set_sound_mode(u16 soundMode);
u16 save_file_get_sound_mode(void);
void render_init(void);
void draw_reset_bars(void);
void profiler_log_thread5_time(enum ProfilerGameEvent eventID);
void audio_game_loop_tick(void);
void select_gfx_pool(void);
void read_controller_inputs(void);
struct LevelCommand* level_script_execute(struct LevelCommand* cmd);
void display_and_vsync(void);
void print_text_fmt_int(s32 x, s32 y, const char* str, s32 n);
void* alloc_display_list(u32 size);
void* segmented_to_virtual(const void* addr);
void set_vblank_handler(s32 index, struct VblankHandler* handler, OSMesgQueue* queue, OSMesg* msg);
void geo_process_node_and_siblings(struct GraphNode* firstNode);
void geo_try_process_children(struct GraphNode* node);
void geo_process_ortho_projection(struct GraphNodeOrthoProjection* node);
void geo_process_master_list(struct GraphNodeMasterList* node);
void geo_process_level_of_detail(struct GraphNodeLevelOfDetail* node);
void geo_process_switch(struct GraphNodeSwitchCase* node);
void geo_process_camera(struct GraphNodeCamera* node);
void geo_process_translation_rotation(struct GraphNodeTranslationRotation* node);
void geo_process_translation(struct GraphNodeTranslation* node);
void geo_process_rotation(struct GraphNodeRotation* node);
void geo_process_object(struct Object* node);
void geo_process_animated_part(struct GraphNodeAnimatedPart* node);
void geo_process_billboard(struct GraphNodeBillboard* node);
void geo_process_display_list(struct GraphNodeDisplayList* node);
void geo_process_scale(struct GraphNodeScale* node);
void geo_process_shadow(struct GraphNodeShadow* node);
void geo_process_object_parent(struct GraphNodeObjectParent* node);

void geo_process_held_object(struct GraphNodeHeldObject* node);
void geo_process_generated_list(struct GraphNodeGenerated* node);
void geo_process_perspective(struct GraphNodePerspective* node);
void clear_mario_platform(void);
struct Object* create_object(const BehaviorScript* bhvScript);
struct GraphNode* geo_make_first_child(struct GraphNode* newFirstChild);
void geo_obj_init_spawninfo(struct GraphNodeObject* graphNode, struct SpawnInfo* spawn);
struct Object* allocate_object(struct ObjectNode* objList);
void snap_object_to_floor(struct Object* obj);
void make_vertex(Vtx* vtx, s32 n, s16 x, s16 y, s16 z, s16 tx, s16 ty, u8 r, u8 g, u8 b, u8 a);
void debug_unknown_level_select_check(void);
void init_free_object_list(void);
void clear_object_lists(struct ObjectNode* objLists);
void stub_behavior_script_2(void);
void stub_obj_list_processor_1(void);
void geo_reset_object_node(struct GraphNodeObject* graphNode);
struct MemoryPool* mem_pool_init(u32 size, u32 side);
void clear_dynamic_surfaces(void);
void cur_obj_set_pos_to_home(void);
struct Object* spawn_object(struct Object* parent, s32 model, const BehaviorScript* behavior);
struct Object* spawn_object_relative(s16 behaviorParam, s16 relativePosX, s16 relativePosY, s16 relativePosZ,
                                     struct Object* parent, s32 model, const BehaviorScript* behavior);
void cur_obj_unhide(void);
void chain_segment_init(struct ChainSegment* segment);
void obj_init_animation_with_sound(struct Object* obj, const struct Animation* const* animations, s32 animIndex);
void init_rcp(void);
void clear_framebuffer(s32 color);
void end_master_display_list(void);
void exec_display_list(struct SPTask* spTask);
void parse_width_field(const char* str, s32* srcIndex, u8* width, s8* zeroPad);
void format_integer(s32 n, s32 base, char* dest, s32* totalLength, u8 width, s8 zeroPad);
void create_dl_scale_matrix(s8 pushOp, f32 x, f32 y, f32 z);
void create_dl_translation_matrix(s8 pushOp, f32 x, f32 y, f32 z);
void play_dialog_sound(u8 dialogID);
void play_sound(s32 soundBits, f32* pos);
void handle_special_dialog_text(s16 dialogID);
void level_set_transition(s16 length, void (*updateFunction)(s16*));
void handle_dialog_text_and_pages(s8 colorMode, struct DialogEntry* dialog, s8 lowerBound);
void render_dialog_box_type(struct DialogEntry* dialog, s8 linesPerBox);
void render_dialog_triangle_choice(void);
void render_dialog_triangle_next(s8 linesPerBox);
s32 trigger_cutscene_dialog(s32 trigger);
u32 ensure_nonnegative(s16 value);
s32 set_and_reset_transition_fade_timer(s8 fadeTimer, u8 transTime);
u8 set_transition_color_fade_alpha(s8 fadeType, s8 fadeTimer, u8 transTime);
s32 run_level_id_or_demo(s32 level);
void print_intro_text(void);
void geo_process_root(struct GraphNodeRoot* node, Vp* b, Vp* c, s32 clearColor);
void render_text_labels(void);
void do_cutscene_handler(void);
void print_displaying_credits_entry(void);
s16 render_menus_and_dialogs(void);
void make_viewport_clip_rect(Vp* viewport);
s32 render_screen_transition(s8 fadeTimer, s8 transType, u8 transTime, struct WarpTransitionData* transData);
void set_warp_transition_rgb(u8 red, u8 green, u8 blue);
void clear_viewport(Vp* viewport, s32 color);
void clear_framebuffer(s32 color);
void create_dl_ortho_matrix(void);
void render_hud_cannon_reticle(void);
void render_hud_mario_lives(void);
void render_hud_coins(void);
void render_hud_stars(void);
void render_hud_keys(void);
void render_hud_power_meter(void);
void render_hud_camera_status(void);
void render_hud_timer(void);
s8 char_to_glyph_index(char c);
void add_glyph_texture(s8 glyphIndex);
void render_textrect(s32 x, s32 y, s32 pos);
s32 mem_pool_free(struct MemoryPool* pool, void* addr);
void render_hud_small_tex_lut(s32 x, s32 y, u8* texture);
void render_hud(void);
void render_hud_tex_lut(s32 x, s32 y, u8* texture);
void print_text(s32 x, s32 y, const char* str, int align);
void print_text_fmt_int(s32 x, s32 y, const char* str, s32 n);

extern struct ObjectHitbox sChainChompHitbox;
extern Mtx* gMatStackFixed[32];
extern struct Object gObjectPool[240];
extern const BehaviorScript bhvCannonBarrelBubbles[];
extern s8 gDebugLevelSelect;
extern BhvCommandProc BehaviorCmdTable[];
extern const BehaviorScript* gCurBhvCommand;
extern const BehaviorScript bhvChainChompChainPart[];
extern struct MemoryPool* gObjectMemoryPool;
extern struct Object* gCurrentObject;
extern struct GoombaProperties sGoombaProperties[];
extern struct ObjectHitbox sGoombaHitbox;
extern const BehaviorScript bhvGoomba[];
extern const BehaviorScript bhvGoomba[];
extern struct GoombaProperties sGoombaProperties[];
extern struct ObjectHitbox sGoombaHitbox;
extern struct Controller* gPlayer1Controller;
extern s16 sPlayMarioGreeting;
extern struct Object gObjectPool[240];
extern const BehaviorScript bhvCannonBarrelBubbles[];
extern s8 gDebugLevelSelect;
extern BhvCommandProc BehaviorCmdTable[];
extern const BehaviorScript* gCurBhvCommand;
extern const BehaviorScript bhvChainChompChainPart[];
extern struct MemoryPool* gObjectMemoryPool;
extern struct Object* gCurrentObject;
extern u32 gGlobalTimer;
extern s8 gDebugLevelSelect;
extern f32 gDialogBoxAngle;
extern f32 gDialogBoxScale;
extern s8 gDialogBoxType;
extern s16 gDialogID;
extern f32 gGlobalSoundSource[3];
extern s16 gDialogScrollOffsetY;
extern s16 gNextDialogPageStartStrIndex;
extern s16 gDialogPageStartStrIndex;
extern s8 gMenuState;
extern s32 gDialogResponse;
extern s8 gDialogWithChoice;
extern s8 gMenuLineNum;
extern struct PowerMeterHUD sPowerMeterHUD;
extern struct Controller* gPlayer3Controller;
extern void* seg2_dialog_table[];
extern struct GfxPool* gGfxPool;
extern struct GfxPool gGfxPools[2];
extern Gfx* gDisplayListHead;
extern u8* gGfxPoolEnd;
extern u32 gGlobalTimer;
extern struct SPTask* gGfxSPTask;
extern BiggerGfxPool* gBiggerGfxPool;
extern BiggerGfxPool* gBiggerGfxPools[2];
extern u8 gGfxSPTaskYieldBuffer[];
extern u8 gGfxSPTaskStack[];
extern u64 gGfxSPTaskOutputBuffer[15872];
extern uintptr_t gPhysicalFramebuffers[3];
extern u16 sRenderedFramebuffer;
extern OSMesg gMainReceivedMesg;
extern OSMesgQueue gGfxVblankQueue;
extern u16 sRenderingFramebuffer;
extern struct Object* gCurrentObject;
extern const BehaviorScript bhvChainChompChainPart[];
extern struct ObjectNode gFreeObjectList;
extern struct MemoryPool* gObjectMemoryPool;
extern struct MemoryPool* gObjectMemoryPoolExpanded;
extern RoomData gDoorAdjacentRooms[60][2];
extern s8 gDebugLevelSelect;
extern struct Object gObjectPool[240];
extern struct Object gObjectPoolExpanded[OBJECT_POOL_CAPACITY_EXPANDED];
extern struct Skybox sSkyBoxInfo[2];
extern s16 gCurrCourseNum;
extern s16 gCurrActNum;
extern s16 gCurrAreaIndex;
extern s16 gSavedCourseNum;
extern s16 gMenuOptSelectIndex;
extern s16 gSaveOptSelectIndex;
extern s16 D_8035FEE2;
extern s16 D_8035FEE4;
extern s16 gTHIWaterDrained;
extern s16 gTTCSpeedSetting;
extern s16 gMarioShotFromCannon;
extern s16 gCCMEnteredSlide;
extern s16 gNumRoomedObjectsInMarioRoom;
extern s16 gNumRoomedObjectsNotInMarioRoom;
extern s16 gWDWWaterLevelChanging;
extern s16 gMarioOnMerryGoRound;
extern struct Object* gMarioObject;
extern struct ObjectNode gObjectListArray[16];
extern struct ObjectNode* gObjectLists;
extern u32 gTimeStopState;
extern Gfx* gDisplayListHead;
extern struct RenderModeContainer renderModeTable_1Cycle[2];
extern struct RenderModeContainer renderModeTable_2Cycle[2];
extern u8 gControllerBits;
extern s8 gResetTimer;
extern s8 gShowDebugText;
extern u8* gGfxPoolEnd;
extern Gfx* gDisplayListHead;
extern OSMesgQueue gSIEventMesgQueue;
extern struct VblankHandler gGameVblankHandler;
extern OSMesgQueue gGameVblankQueue;
extern const LevelScript level_castle_courtyard_entry[];
extern const LevelScript level_script_entry[];
extern const LevelScript level_main_menu_entry_1[];
extern struct GraphNodePerspective* gCurGraphNodeCamFrustum;
extern Mat4 gMatStack[32];
extern s16 gMatStackIndex;
extern struct GraphNodeRoot* gCurGraphNodeRoot;
extern f32 gSineTable[];
extern f32 gCosineTable[];
extern struct GraphNodePerspective* gCurGraphNodeCamFrustum;
extern void* sTextureTransitionID[];
extern struct WarpTransition gWarpTransition;
extern Vp* D_8032CE74;
extern Vp* D_8032CE78;
extern Vp D_8032CF00;
extern u32 gFBSetColor;
extern s16 gWarpTransDelay;
extern u32 gWarpTransFBSetColor;
extern struct HudDisplay gHudDisplay;
extern s16 sPowerMeterStoredHealth;
extern s32 sPowerMeterVisibleTimer;
extern struct Area* gCurrentArea;
extern struct MemoryPool* gEffectsMemoryPool;
extern s16 sTextLabelsCount;
extern struct TextLabel* sTextLabels[52];
extern s8 gHudFlash;
extern s32 globalTimer;
extern s16 sCameraHUDStatus;
#define o gCurrentObject

#endif