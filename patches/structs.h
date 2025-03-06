#ifndef STRUCTS_H
#define STRUCTS_H

#include "patches.h"

typedef Gfx* (*GraphNodeFunc)(s32 callContext, struct GraphNode* node, void* context);

struct FnGraphNode {
    /*0x00*/ struct GraphNode node;
    /*0x14*/ GraphNodeFunc func;
};

struct GraphNodeRoot {
    /*0x00*/ struct GraphNode node;
    /*0x14*/ u8 areaIndex;
    /*0x15*/ s8 unk15; // ?
    /*0x16*/ s16 x;
    /*0x18*/ s16 y;
    /*0x1A*/ s16 width;    // half width, 160
    /*0x1C*/ s16 height;   // half height
    /*0x1E*/ s16 numViews; // number of entries in mystery array
    /*0x20*/ struct GraphNode** views;
};

struct GraphNodePerspective {
    /*0x00*/ struct FnGraphNode fnNode;
    /*0x18*/ s32 unused;
    /*0x1C*/ f32 fov;  // horizontal field of view in degrees
    /*0x20*/ s16 near; // near clipping plane
    /*0x22*/ s16 far;  // far clipping plane
};

struct list_head {
    struct list_head* prev;
    struct list_head* next;
};

struct DisplayListNode {
    Mtx* transform;
    void* displayList;
    struct DisplayListNode* next;
};

struct GraphNodeMasterList {
    /*0x00*/ struct GraphNode node;
    /*0x14*/ struct DisplayListNode* listHeads[8];
    /*0x34*/ struct DisplayListNode* listTails[8];
};

struct RenderModeContainer {
    u32 modes[8];
};

struct SpawnInfo {
    /*0x00*/ Vec3s startPos;
    /*0x06*/ Vec3s startAngle;
    /*0x0C*/ s8 areaIndex;
    /*0x0D*/ s8 activeAreaIndex;
    /*0x10*/ u32 behaviorArg;
    /*0x14*/ void* behaviorScript;
    /*0x18*/ struct GraphNode* model;
    /*0x1C*/ struct SpawnInfo* next;
};

struct WarpTransitionData {
    /*0x00*/ u8 red;
    /*0x01*/ u8 green;
    /*0x02*/ u8 blue;

    /*0x04*/ s16 startTexRadius;
    /*0x06*/ s16 endTexRadius;
    /*0x08*/ s16 startTexX;
    /*0x0A*/ s16 startTexY;
    /*0x0C*/ s16 endTexX;
    /*0x0E*/ s16 endTexY;

    /*0x10*/ s16 texTimer; // always 0, does seems to affect transition when disabled
};

struct WarpTransition {
    /*0x00*/ u8 isActive;       // Is the transition active. (either TRUE or FALSE)
    /*0x01*/ u8 type;           // Determines the type of transition to use (circle, star, etc.)
    /*0x02*/ u8 time;           // Amount of time to complete the transition (in frames)
    /*0x03*/ u8 pauseRendering; // Should the game stop rendering. (either TRUE or FALSE)
    /*0x04*/ struct WarpTransitionData data;
};

struct PowerMeterHUD {
    s8 animation;
    s16 x;
    s16 y;
    f32 unused;
};

enum PowerMeterAnimation {
    POWER_METER_HIDDEN,
    POWER_METER_EMPHASIZED,
    POWER_METER_DEEMPHASIZING,
    POWER_METER_HIDING,
    POWER_METER_VISIBLE
};

struct HudDisplay {
    /*0x00*/ s16 lives;
    /*0x02*/ s16 coins;
    /*0x04*/ s16 stars;
    /*0x06*/ s16 wedges;
    /*0x08*/ s16 keys;
    /*0x0A*/ s16 flags;
    /*0x0C*/ u16 timer;
};

struct Area {
    /*0x00*/ s8 index;
    /*0x01*/ s8 flags;                    // Only has 1 flag: 0x01 = Is this the active area?
    /*0x02*/ u16 terrainType;             // default terrain of the level (set from level script cmd 0x31)
    /*0x04*/ struct GraphNodeRoot* unk04; // geometry layout data
    /*0x08*/ TerrainData* terrainData;    // collision data (set from level script cmd 0x2E)
    /*0x0C*/ RoomData* surfaceRooms;      // (set from level script cmd 0x2F)
    /*0x10*/ s16* macroObjects;           // Macro Objects Ptr (set from level script cmd 0x39)
    /*0x14*/ struct ObjectWarpNode* warpNodes;
    /*0x18*/ struct WarpNode* paintingWarpNodes;
    /*0x1C*/ struct InstantWarp* instantWarps;
    /*0x20*/ struct SpawnInfo* objectSpawnInfos;
    /*0x24*/ struct Camera* camera;
    /*0x28*/ struct UnusedArea28* unused; // Filled by level script 0x3A, but is unused.
    /*0x2C*/ struct Whirlpool* whirlpools[2];
    /*0x34*/ u8 dialog[2]; // Level start dialog number (set by level script cmd 0x30)
    /*0x36*/ u16 musicParam;
    /*0x38*/ u16 musicParam2;
};

struct Camera {
    /*0x00*/ u8 mode; // What type of mode the camera uses (see defines above)
    /*0x01*/ u8 defMode;
    /**
     * Determines what direction Mario moves in when the analog stick is moved.
     *
     * @warning This is NOT the camera's xz-rotation in world space. This is the angle calculated from the
     *          camera's focus TO the camera's position, instead of the other way around like it should
     *          be. It's effectively the opposite of the camera's actual yaw. Use
     *          vec3f_get_dist_and_angle() if you need the camera's yaw.
     */
    /*0x02*/ s16 yaw;
    /*0x04*/ Vec3f focus;
    /*0x10*/ Vec3f pos;
    /*0x1C*/ Vec3f unusedVec1;
    /// The x coordinate of the "center" of the area. The camera will rotate around this point.
    /// For example, this is what makes the camera rotate around the hill in BoB
    /*0x28*/ f32 areaCenX;
    /// The z coordinate of the "center" of the area. The camera will rotate around this point.
    /// For example, this is what makes the camera rotate around the hill in BoB
    /*0x2C*/ f32 areaCenZ;
    /*0x30*/ u8 cutscene;
    /*0x31*/ u8 filler1[8];
    /*0x3A*/ s16 nextYaw;
    /*0x3C*/ u8 filler2[40];
    /*0x64*/ u8 doorStatus;
    /// The y coordinate of the "center" of the area. Unlike areaCenX and areaCenZ, this is only used
    /// when paused. See zoom_out_if_paused_and_outside
    /*0x68*/ f32 areaCenY;
};

struct TextLabel {
    u32 x;
    u32 y;
    s16 length;
    char buffer[50];
};

enum osd_animation_type { OSD_NONE, OSD_FADE, OSD_NUM_ANIM_TYPES };

enum osd_message_state {
    OSD_APPEAR,    // OSD message is appearing on the screen
    OSD_DISPLAY,   // OSD message is being displayed on the screen
    OSD_DISAPPEAR, // OSD message is disappearing from the screen

    OSD_NUM_STATES
};

enum osd_corner {
    OSD_TOP_LEFT,   // 0 in the picture above
    OSD_TOP_CENTER, // 1 in the picture above
    OSD_TOP_RIGHT,  // 2 in the picture above

    OSD_MIDDLE_LEFT,   // 3 in the picture above
    OSD_MIDDLE_CENTER, // 4 in the picture above
    OSD_MIDDLE_RIGHT,  // 5 in the picture above

    OSD_BOTTOM_LEFT,   // 6 in the picture above
    OSD_BOTTOM_CENTER, // 7 in the picture above
    OSD_BOTTOM_RIGHT,  // 8 in the picture above

    OSD_NUM_CORNERS
};

typedef struct {
    char* text;                                        // Text that this object will have when displayed
    enum osd_corner corner;                            // One of the 9 corners
    float xoffset;                                     // Relative X position
    float yoffset;                                     // Relative Y position
    float color[3];                                    // Red, Green, Blue values
    float sizebox[4];                                  // bounding box (xmin, ymin, xmax, ymax)
    int state;                                         // display state of current message
    enum osd_animation_type animation[OSD_NUM_STATES]; // animations for each display state
    unsigned int timeout[OSD_NUM_STATES];              // timeouts for each display state
#define OSD_INFINITE_TIMEOUT 0xffffffff
    unsigned int frames; // number of frames in this state
    int user_managed;    // structure managed by caller and not to be freed by us
    struct list_head list;
} osd_message_t;

struct GfxPool {
    Gfx buffer[6400];
    struct SPTask spTask;
};

struct DialogEntry {
    /*0x00*/ u32 unused;
    /*0x04*/ s8 linesPerBox;
    /*0x06*/ s16 leftOffset;
    /*0x08*/ s16 width;
    /*0x0C*/ const u8* str;
};

typedef struct BiggerGfxPool {
    Gfx buffer[GFX_POOL_SIZE];
    struct SPTask spTask;
} BiggerGfxPool;

struct ChainSegment {
    f32 posX;
    f32 posY;
    f32 posZ;
    s16 pitch;
    s16 yaw;
    s16 roll;
};

struct Skybox {
    /// The camera's yaw, from 0 to 65536, which maps to 0 to 360 degrees
    u16 yaw;
    /// The camera's pitch, which is bounded by +-16384, which maps to -90 to 90 degrees
    s16 pitch;
    /// The skybox's X position in world space
    s32 scaledX;
    /// The skybox's Y position in world space
    s32 scaledY;

    /// The index of the upper-left tile in the 3x3 grid that gets drawn
    s32 upperLeftTile;
};

struct GraphNodeCullingRadius {
    /*0x00*/ struct GraphNode node;
    /*0x14*/ s16 cullingRadius; // specifies the 'sphere radius' for purposes of frustum culling
    u8 filler[2];
};

struct GraphNodeLevelOfDetail {
    /*0x00*/ struct GraphNode node;
    /*0x14*/ s16 minDistance;
    /*0x16*/ s16 maxDistance;
};

struct GoombaProperties {
    f32 scale;
    u32 deathSound;
    s16 drawDistance;
    s8 damage;
};

enum ProfilerGameEvent { THREAD5_START, LEVEL_SCRIPT_EXECUTE, BEFORE_DISPLAY_LISTS, AFTER_DISPLAY_LISTS, THREAD5_END };

enum TextureTransitionID { TEX_TRANS_STAR, TEX_TRANS_CIRCLE, TEX_TRANS_MARIO, TEX_TRANS_BOWSER };

enum ObjectList {
    OBJ_LIST_PLAYER,      //  (0) Mario
    OBJ_LIST_UNUSED_1,    //  (1) (unused)
    OBJ_LIST_DESTRUCTIVE, //  (2) things that can be used to destroy other objects, like
                          //      bob-ombs and corkboxes
    OBJ_LIST_UNUSED_3,    //  (3) (unused)
    OBJ_LIST_GENACTOR,    //  (4) general actors. most normal 'enemies' or actors are
                          //      on this list. (MIPS, bullet bill, bully, etc)
    OBJ_LIST_PUSHABLE,    //  (5) pushable actors. This is a group of objects which
                          //      can push each other around as well as their parent
                          //      objects. (goombas, koopas, spinies)
    OBJ_LIST_LEVEL,       //  (6) level objects. general level objects such as heart, star
    OBJ_LIST_UNUSED_7,    //  (7) (unused)
    OBJ_LIST_DEFAULT,     //  (8) default objects. objects that didnt start with a 00
                          //      command are put here, so this is treated as a default.
    OBJ_LIST_SURFACE,     //  (9) surface objects. objects that specifically have surface
                          //      collision and not object collision. (thwomp, whomp, etc)
    OBJ_LIST_POLELIKE,    // (10) polelike objects. objects that attract or otherwise
                          //      "cling" Mario similar to a pole action. (hoot,
                          //      whirlpool, trees/poles, etc)
    OBJ_LIST_SPAWNER,     // (11) spawners
    OBJ_LIST_UNIMPORTANT, // (12) unimportant objects. objects that will not load
                          //      if there are not enough object slots: they will also
                          //      be manually unloaded to make room for slots if the list
                          //      gets exhausted.
    NUM_OBJ_LISTS
};

enum DialogID {
    DIALOG_NONE = -1,
    DIALOG_000,
    DIALOG_001,
    DIALOG_002,
    DIALOG_003,
    DIALOG_004,
    DIALOG_005,
    DIALOG_006,
    DIALOG_007,
    DIALOG_008,
    DIALOG_009,
    DIALOG_010,
    DIALOG_011,
    DIALOG_012,
    DIALOG_013,
    DIALOG_014,
    DIALOG_015,
    DIALOG_016,
    DIALOG_017,
    DIALOG_018,
    DIALOG_019,
    DIALOG_020,
    DIALOG_021,
    DIALOG_022,
    DIALOG_023,
    DIALOG_024,
    DIALOG_025,
    DIALOG_026,
    DIALOG_027,
    DIALOG_028,
    DIALOG_029,
    DIALOG_030,
    DIALOG_031,
    DIALOG_032,
    DIALOG_033,
    DIALOG_034,
    DIALOG_035,
    DIALOG_036,
    DIALOG_037,
    DIALOG_038,
    DIALOG_039,
    DIALOG_040,
    DIALOG_041,
    DIALOG_042,
    DIALOG_043,
    DIALOG_044,
    DIALOG_045,
    DIALOG_046,
    DIALOG_047,
    DIALOG_048,
    DIALOG_049,
    DIALOG_050,
    DIALOG_051,
    DIALOG_052,
    DIALOG_053,
    DIALOG_054,
    DIALOG_055,
    DIALOG_056,
    DIALOG_057,
    DIALOG_058,
    DIALOG_059,
    DIALOG_060,
    DIALOG_061,
    DIALOG_062,
    DIALOG_063,
    DIALOG_064,
    DIALOG_065,
    DIALOG_066,
    DIALOG_067,
    DIALOG_068,
    DIALOG_069,
    DIALOG_070,
    DIALOG_071,
    DIALOG_072,
    DIALOG_073,
    DIALOG_074,
    DIALOG_075,
    DIALOG_076,
    DIALOG_077,
    DIALOG_078,
    DIALOG_079,
    DIALOG_080,
    DIALOG_081,
    DIALOG_082,
    DIALOG_083,
    DIALOG_084,
    DIALOG_085,
    DIALOG_086,
    DIALOG_087,
    DIALOG_088,
    DIALOG_089,
    DIALOG_090,
    DIALOG_091,
    DIALOG_092,
    DIALOG_093,
    DIALOG_094,
    DIALOG_095,
    DIALOG_096,
    DIALOG_097,
    DIALOG_098,
    DIALOG_099,
    DIALOG_100,
    DIALOG_101,
    DIALOG_102,
    DIALOG_103,
    DIALOG_104,
    DIALOG_105,
    DIALOG_106,
    DIALOG_107,
    DIALOG_108,
    DIALOG_109,
    DIALOG_110,
    DIALOG_111,
    DIALOG_112,
    DIALOG_113,
    DIALOG_114,
    DIALOG_115,
    DIALOG_116,
    DIALOG_117,
    DIALOG_118,
    DIALOG_119,
    DIALOG_120,
    DIALOG_121,
    DIALOG_122,
    DIALOG_123,
    DIALOG_124,
    DIALOG_125,
    DIALOG_126,
    DIALOG_127,
    DIALOG_128,
    DIALOG_129,
    DIALOG_130,
    DIALOG_131,
    DIALOG_132,
    DIALOG_133,
    DIALOG_134,
    DIALOG_135,
    DIALOG_136,
    DIALOG_137,
    DIALOG_138,
    DIALOG_139,
    DIALOG_140,
    DIALOG_141,
    DIALOG_142,
    DIALOG_143,
    DIALOG_144,
    DIALOG_145,
    DIALOG_146,
    DIALOG_147,
    DIALOG_148,
    DIALOG_149,
    DIALOG_150,
    DIALOG_151,
    DIALOG_152,
    DIALOG_153,
    DIALOG_154,
    DIALOG_155,
    DIALOG_156,
    DIALOG_157,
    DIALOG_158,
    DIALOG_159,
    DIALOG_160,
    DIALOG_161,
    DIALOG_162,
    DIALOG_163,
    DIALOG_164,
    DIALOG_165,
    DIALOG_166,
    DIALOG_167,
    DIALOG_168,
    DIALOG_169,
    DIALOG_COUNT
};

enum MenuState {
    MENU_STATE_0,
    MENU_STATE_1,
    MENU_STATE_2,
    MENU_STATE_3,
    MENU_STATE_DEFAULT = MENU_STATE_0,

    // Dialog
    MENU_STATE_DIALOG_OPENING = MENU_STATE_0,
    MENU_STATE_DIALOG_OPEN = MENU_STATE_1,
    MENU_STATE_DIALOG_SCROLLING = MENU_STATE_2,
    MENU_STATE_DIALOG_CLOSING = MENU_STATE_3,

    // Pause Screen
    MENU_STATE_PAUSE_SCREEN_OPENING = MENU_STATE_0,
    MENU_STATE_PAUSE_SCREEN_COURSE = MENU_STATE_1,
    MENU_STATE_PAUSE_SCREEN_CASTLE = MENU_STATE_2,

    // Course Complete Screen
    MENU_STATE_COURSE_COMPLETE_SCREEN_OPENING = MENU_STATE_0,
    MENU_STATE_COURSE_COMPLETE_SCREEN_OPEN = MENU_STATE_1
};

enum DialogBoxType {
    DIALOG_TYPE_ROTATE, // used in NPCs and level messages
    DIALOG_TYPE_ZOOM    // used in signposts and wall signs and etc
};
// gDialogResponse
enum DialogResponseDefines {
    DIALOG_RESPONSE_NONE,
    DIALOG_RESPONSE_YES,
    DIALOG_RESPONSE_NO,
    DIALOG_RESPONSE_NOT_DEFINED
};

// enum LevelNum { LEVEL_NONE, LEVEL_COUNT, LEVEL_MAX = LEVEL_COUNT - 1, LEVEL_MIN = LEVEL_NONE + 1 };

enum HUDDisplayFlag {
    HUD_DISPLAY_FLAG_LIVES = 0x0001,
    HUD_DISPLAY_FLAG_COIN_COUNT = 0x0002,
    HUD_DISPLAY_FLAG_STAR_COUNT = 0x0004,
    HUD_DISPLAY_FLAG_CAMERA_AND_POWER = 0x0008,
    HUD_DISPLAY_FLAG_KEYS = 0x0010,
    HUD_DISPLAY_FLAG_UNKNOWN_0020 = 0x0020,
    HUD_DISPLAY_FLAG_TIMER = 0x0040,
    HUD_DISPLAY_FLAG_EMPHASIZE_POWER = 0x8000,

    HUD_DISPLAY_NONE = 0x0000,
    HUD_DISPLAY_DEFAULT = HUD_DISPLAY_FLAG_LIVES | HUD_DISPLAY_FLAG_COIN_COUNT | HUD_DISPLAY_FLAG_STAR_COUNT |
                          HUD_DISPLAY_FLAG_CAMERA_AND_POWER | HUD_DISPLAY_FLAG_KEYS | HUD_DISPLAY_FLAG_UNKNOWN_0020
};

enum MenuOption {
    MENU_OPT_NONE,
    MENU_OPT_1,
    MENU_OPT_2,
    MENU_OPT_3,
    MENU_OPT_DEFAULT = MENU_OPT_1,

    // Course Pause Menu
    MENU_OPT_CONTINUE = MENU_OPT_1,
    MENU_OPT_EXIT_COURSE = MENU_OPT_2,
    MENU_OPT_CAMERA_ANGLE_R = MENU_OPT_3,

    // Save Menu
    MENU_OPT_SAVE_AND_CONTINUE = MENU_OPT_1,
    MENU_OPT_SAVE_AND_QUIT = MENU_OPT_2,
    MENU_OPT_CONTINUE_DONT_SAVE = MENU_OPT_3
};

enum CameraHUDLut {
    GLYPH_CAM_CAMERA,
    GLYPH_CAM_MARIO_HEAD,
    GLYPH_CAM_LAKITU_HEAD,
    GLYPH_CAM_FIXED,
    GLYPH_CAM_ARROW_UP,
    GLYPH_CAM_ARROW_DOWN
};

typedef enum ExAlignment {
    /* 0 */ GEX_ALIGN_DEFAULT,
    /* 1 */ GEX_ALIGN_LEFT,
    /* 2 */ GEX_ALIGN_RIGHT,
} ExAlignment;

enum LevelScriptIntroArgs {
    LVL_INTRO_PLAY_ITS_A_ME_MARIO,
    LVL_INTRO_REGULAR,
    LVL_INTRO_GAME_OVER,
    LVL_INTRO_LEVEL_SELECT
};

struct ExTextLabel {
    u32 x;
    u32 y;
    s16 length;
    char buffer[50];
    ExAlignment align;
};

#endif