#ifndef STRUCTS_H
#define STRUCTS_H

#include "patches.h"

#define PROPRECORD_STAN_ROOM_LEN 4
#define WATCH_NUMBER_SCREENS 5

typedef enum ITEM_IDS {
    ITEM_UNARMED,
    ITEM_FIST,
    ITEM_KNIFE,
    ITEM_THROWKNIFE,
    ITEM_WPPK,
    ITEM_WPPKSIL,
    ITEM_TT33,
    ITEM_SKORPION,
    ITEM_AK47,
    ITEM_UZI,
    ITEM_MP5K,
    ITEM_MP5KSIL,
    ITEM_SPECTRE,
    ITEM_M16,
    ITEM_FNP90,
    ITEM_SHOTGUN,
    ITEM_AUTOSHOT,
    ITEM_SNIPERRIFLE,
    ITEM_RUGER,
    ITEM_GOLDENGUN,
    ITEM_SILVERWPPK,
    ITEM_GOLDWPPK,
    ITEM_LASER,
    ITEM_WATCHLASER,
    ITEM_GRENADELAUNCH,
    ITEM_ROCKETLAUNCH,
    ITEM_GRENADE,
    ITEM_TIMEDMINE,
    ITEM_PROXIMITYMINE,
    ITEM_REMOTEMINE,
    ITEM_TRIGGER,
    ITEM_TASER,
    ITEM_TANKSHELLS,
    ITEM_BOMBCASE,
    ITEM_PLASTIQUE,
    ITEM_FLAREPISTOL,
    ITEM_PITONGUN,
    ITEM_BUNGEE,
    ITEM_DOORDECODER,
    ITEM_BOMBDEFUSER,
    ITEM_CAMERA,
    ITEM_LOCKEXPLODER,
    ITEM_DOOREXPLODER,
    ITEM_BRIEFCASE,
    ITEM_WEAPONCASE,
    ITEM_SAFECRACKERCASE,
    ITEM_KEYANALYSERCASE,
    ITEM_BUG,
    ITEM_MICROCAMERA,
    ITEM_BUGDETECTOR,
    ITEM_EXPLOSIVEFLOPPY,
    ITEM_POLARIZEDGLASSES,
    ITEM_DARKGLASSES,
    ITEM_CREDITCARD,
    ITEM_GASKEYRING,
    ITEM_DATATHIEF,
    ITEM_WATCHIDENTIFIER,
    ITEM_WATCHCOMMUNICATOR,
    ITEM_WATCHGEIGERCOUNTER,
    ITEM_WATCHMAGNETREPEL,
    ITEM_WATCHMAGNETATTRACT,
    ITEM_GOLDENEYEKEY,
    ITEM_BLACKBOX,
    ITEM_CIRCUITBOARD,
    ITEM_CLIPBOARD,
    ITEM_STAFFLIST,
    ITEM_DOSSIERRED,
    ITEM_PLANS,
    ITEM_SPYFILE,
    ITEM_BLUEPRINTS,
    ITEM_MAP,
    ITEM_AUDIOTAPE,
    ITEM_VIDEOTAPE,
    ITEM_DATTAPE,
    ITEM_SPOOLTAPE,
    ITEM_MICROFILM,
    ITEM_MICROCODE,
    ITEM_LECTRE,
    ITEM_MONEY,
    ITEM_GOLDBAR,
    ITEM_HEROIN,
    ITEM_KEYCARD,
    ITEM_KEYYALE,
    ITEM_KEYBOLT,
    ITEM_SUIT_LF_HAND,
    ITEM_JOYPAD,
    ITEM_NULL86,
    ITEM_NULL87,
    ITEM_TOKEN,
    ITEM_IDS_MAX
} ITEM_IDS;

typedef struct coord16 {
    union {
        struct {
            s16 x;
            s16 y;
            s16 z;
        };
        s16 AsArray[3];
    };
} coord16;

typedef struct StandTileHeaderMid {
    u16 special : 4; // 0=normal 1=kneeling 3=ladder
    u16 r : 4;
    u16 g : 4;
    u16 b : 4;
} StandTileHeaderMid;

typedef struct StandTileHeaderTail {
    s16 pointCount : 4; // seen lh, not lhu. Also seen with an explicit unnecessary '& 0xF'
    // Indices of the most extreme points (the resulting triangle should encompass _MOST_ of the tile)
    s16 headerC : 4;
    s16 headerD : 4;
    s16 headerE : 4;
} StandTileHeaderTail;

typedef struct StandTilePoint {
    struct coord16; // points for tile
    u16 link;       // link to nother tile
} StandTilePoint;

typedef struct StandTile {
    /* u16 ID;         // Decimal
    u16 GroupID : 4;   // a1,a2,a3...z5,z6,z7
    u16 RoomID : 4;    // compared to 0xFF, not -1 in a function. Seen LBUs.
    */
    u32 id : 24;

    u8 room; // compared to 0xFF, not -1 in a function. Seen LBUs.

    union {
        StandTileHeaderMid headerMid;
        s16 half;
    } mid;

    /* 0x06 */
    // They appear to have performed the bit field work themselves here,
    //   but we provide the StandTileHeaderTail member for clarity - it should be unused I believe.
    union {
        StandTileHeaderTail hdrTail;
        s16 half;
    } tail;

    /* 0x08 */
    StandTilePoint points[];
} StandTile;

typedef struct coord2d {
    union {
        struct {
            f32 x;
            f32 y;
        };
        f32 f[2];
    };
} coord2d;

typedef struct coord3d {
    union {
        struct {
            f32 x;
            f32 y;
            f32 z;
        };
        f32 f[3];
    };
} coord3d;

typedef struct rect4f {
    union {
        struct coord2d points[4];
        f32 f[8];
    };
} rect4f;

struct collision434 {

    /**
     * Offset 0x00.
     */
    struct StandTile* current_tile_ptr;

    /**
     * Offset 0x04.
     */
    coord3d collision_position;

    /**
     * This affects Bond's movement, but not the viewport.
     * This does not affect boost direction.
     *
     * f[0]: forward component (sin theta) in radians
     * f[1]: zero
     * f[2]: sideways component (cos theta) in radians.
     * Offset 0x10.
     */
    struct coord3d theta_transform;

    /**
     * Some kind of alternative to pos3 (in player struct).
     * Offset 0x20.
     */
    coord3d pos3;

    /**
     * Offset 0x2c.
     */
    f32 collision_radius;

    /**
     * Some kind of alternative to pos (in player struct).
     * Offset 0x30.
     */
    coord3d pos;

    /**
     * bondviewMoveAnimationTick calculates 4x4 view matrix transform and then writes rotation
     * to this property and applied_view2. Removing the update to this property will fix
     * Bond's camera orientation such that Bond can still turn, but the camera is always
     * locked facing the starting position.
     * Offset 0x3c.
     */
    struct coord3d applied_view;

    /**
     * Offset 0x48.
     */
    struct coord3d applied_view2;

    /**
     * Offset 0x54.
     */
    StandTile* current_tile_ptr_for_portals;
};

typedef struct ModelNode {
    // u8 UseAdditionalMatrices; //1 = group use MatrixID1 also. not actually used
    u16 Opcode;               /*0x00*/
    union ModelRoData* Data;  /*0x04 Node Data*/
    struct ModelNode* Parent; /*0x08*/
    struct ModelNode* Next;   /*0x0c*/
    struct ModelNode* Prev;   /*0x10*/
    struct ModelNode* Child;  /*0x14*/
} ModelNode;

typedef struct ModelJoint {
    u16 NodeType;
    u16 mtxA;
    u16 mtxB;
} ModelJoint;

typedef struct ModelSkeleton {
    short numjoints; // Always at least 1 (root)
    short pad1;      // pad
    ModelJoint* Joints;
    short SkeletonSize;
    short pad2;
#ifdef DEBUG
    char** JointNames;
#endif
} ModelSkeleton;

typedef struct ModelFileTextures {
    u32 TextureID;
    u8 Width;
    u8 Height;
    u8 MipMapTiles;
    u8 Type;
    u8 RenderDepth; /*use G_IM_SIZ_ Note: CI uses 16bit*/
    u8 sflags;      /*use G_TX_*/
    u8 tflags;      /*use G_TX_*/

} ModelFileTextures;

typedef struct ModelFileHeader {
    ModelNode* RootNode; // possibly this is "root data"
    ModelSkeleton* Skeleton;
    /* This is a pointer to a variable length array of pointers to modelnodes, but the array is followed by an s16 array
     * of part numbers.*/
    ModelNode** Switches;
    s16 numSwitches; // c Number of "05"s, these link to positions/switches Note: Does not reflect number actually used
    s16 numMatrices; // Number of matrices used. (GE Name is LCase)
    f32 BoundingVolumeRadius;    // 10 radius from model center to frustrum edge before culling.
    s16 numRecords;              // 14 used in runtime for number of records
    s16 numtextures;             // 16 Number of textures in table
    ModelFileTextures* Textures; // 18 offset to texture table
#ifdef DEBUG
    char** SwitchNames;
#endif
#if defined(VERSION_EU)
#else
    s32 isLoaded;
#endif
} ModelFileHeader;

typedef struct ModelAnimation {
    s32 address;
    u16 unk04; // nextframe
    u16 unk06;
    u16 unk08;
    u16 unk0A;
    u16 unk0C;
    u16 unk0E;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c; // endframe

    // ...
} ModelAnimation;

typedef union {
    f32 m[4][4];
    s32 unused;
} Mtxf;

typedef union {
    Mtxf pos;
    s32 view[4][4];
} RenderPosView;

typedef struct Model {
    s16 unk00; /*0x00*/ // objInit() indicates that unk00 is a s16...
    s16 Type; /*0x01*/  // but modelInit() indicates that Type is a s16...
                        // not sure which is correct.

    struct ChrRecord* chr; /*0x04*/
    ModelFileHeader* obj;  /*0x08 GE Name confirmed*/

    /**
     * List of length model->obj->numMatrices dynamically allocated.
     */
    RenderPosView* render_pos; /*0x0c*/
    union ModelRwData** datas; // array of pointers to modeldata structs /*0x10*/

    f32 scale;                     /*0x14*/
    struct Model* attachedto;      /*0x18*/
    ModelNode* attachedto_objinst; /*0x1c*/

    // need `struct anim` definition from AI branch.
    ModelAnimation* anim; /*0x20*/

    s8 gunhand; // used by ACT_STAND
    s8 unk25;
    s8 animlooping; /*0x26*/
    s8 unk27;

    f32 unk28; // animation related
    f32 unk2c;

    /**
     * Animation framea (per debug message)
     * Offset 0x30.
     */
    s16 framea;

    /**
     * Animation frameb (per debug message)
     * Offset 0x32.
     */
    s16 frameb;
    s32 unk34;
    s32 unk38;
    f32 endframe;

    f32 speed;       /*0x40*/
    f32 newspeed;    /*0x44*/
    f32 oldspeed;    /*0x48*/
    f32 timespeed;   /*0x4C*/
    f32 elapsespeed; /*0x50*/

    ModelAnimation* anim2;

    f32 unk58;
    f32 unk5c;

    /**
     * Animation frame2a (per debug message)
     * Offset 0x60.
     */
    s16 frame2a;

    /**
     * Animation frame2b (per debug message)
     * Offset 0x62.
     */
    s16 frame2b;

    s32 unk64;
    s32 unk68;
    f32 unk6c;
    // 0x70
    f32 speed2;
    s32 unk74;
    s32 unk78;
    f32 unk7c;
    // 0x80
    s32 unk80;

    /**
     * Related to "anim2", per debug message.
     */
    f32 unk84;

    f32 unk88;
    s32 unk8c;
    // 0x90
    f32 animloopframe; /*0x90*/
    f32 animloopmerge; /*0x94*/
    s32 animflipfunc;  /*0x98*/
    s32 unk9c;
    // 0xa0
    s32 unka0;
    f32 playspeed; // used by ACT_STAND in chrlv
    f32 animrate;
    f32 unkac;
    // 0xb0
    f32 unkb0;
    f32 unkb4;
    f32 unkb8; // used by ACT_ANIM in chrlv
    s32 unkbc;
} Model;

typedef struct rgba_u8 {
    union {
        struct {
            u8 r;
            u8 g;
            u8 b;
            u8 a;
        };
        u8 rgba[4];
        u32 word; // added to match
    };
} rgba_u8;

typedef struct PropRecord {
    u8 type;         /*0x00   PROP_TYPE */
    u8 flags;        /*0x01*/
    s16 timetoregen; // ticks down /*0x02*/
    union {
        struct ChrRecord* chr;
        struct ObjectRecord* obj;
        struct DoorRecord* door;
        struct WeaponObjRecord* weapon;
        struct Explosion* explosion;
        struct Smoke* smoke;
        struct Scorch* scorch;
        void* voidp;
    }; /*0x04*/
    coord3d pos;     /*0x08*/
    StandTile* stan; /*0x14 canonical name */

    /**
     * Maybe float. Something related to draw (render) distance.
     */
    f32 zDepth;                         /*0x18*/
    struct PropRecord* parent;          /*0x1c*/
    struct PropRecord* child;           /*0x20*/
    struct PropRecord* prev;            /*0x24*/
    struct PropRecord* next;            /*0x28*/
    u8 rooms[PROPRECORD_STAN_ROOM_LEN]; /*0x2c*/
    s32 unk30;

    // u16 unk38;         /*0x38* /
    // s16 unk3a;         /*0x3a* /
    // u8 unk3c;          /*0x3c* /
    // u8 propstateindex; /*0x3d* /
    // u8 unk3e;          /*0x3e* /
    // u8 unk3f_00 : 1;   /*0x3f* /
    // u8 unk3f_01 : 1;   /*0x3f* /
    // u8 unk3f_02 : 1;   /*0x3f* /
    // u8 unk3f_03 : 1;   /*0x3f* /
    // u8 unk3f_04 : 1;   /*0x3f* /
    // u8 unk3f_05 : 1;   /*0x3f* /
    // u8 unk3f_06 : 1;   /*0x3f* /
    // u8 unk3f_07 : 1;   /*0x3f* /
    // u32 unk40;         /*0x40* /
    // u32 unk44;         /*0x44*/
} PropRecord;

struct texpool {
    u8* start;
    struct tex* end;
    u8* leftpos;
    struct tex* rightpos;
};

typedef struct ChrRecord_f180 {
    /***/
    s8 unk00;
    s8 item_id; // type ITEM_IDS
    char unk02;
    char unk03;

    coord3d pos;

    /**
     * Offset 0x10
     */
    struct coord3d delta;
    f32 unk1c;

    f32 unk20;
    f32 unk24;
    f32 unk28;
} ChrRecord_f180;

typedef struct ObjectRecord {
    struct PropDefHeaderRecord;
    s16 obj; // canonical name, use with PROP_ enum eg PROP_ALARM1
    /* ID 0x6
        0000+ or 2710+ (10,000+) to use standard presets.
        -1 to -256 to set this object
            inside the previous object.(solo only)
        if control nibble 4x is set at 0xB,
        then this number matches the ID of a guard
    */
    s16 pad;
    /*0x8
        8x    indicates right-handed gun assignment
        4x    08 weapon does not provide ammunition when collected
        2x    indicates object in motion or special function is activated
        1x    indicates left-handed normal pickup or opposite alignment
        0x    indicates no control features set
        x8
        x4    (unknown) Jungle bushes
        x2    (unknown) pete grenade
        x1    indicates embedded crate or other object, creating a chain of boxes, for
        example      x0    indicates normal preset, or beginning/end of chain
        Doors:
        8x    open by default
        4x    area behind door is always visible (no blackouts for gates, lab doors, etc)
        2x    open backwards
        1x    same as 0 as far as I can tell
        x8    always open away from the player regardless what side you're on
        x4
        x2    player can't activate door (spawn block or 16 type activation)
        x1
        0x9:
        8x
        4x    immobile
        2x    (unknown) Silo DAT tape
        1x    uncollectable
        x8
        x4    allows object pickup (chr_name objects only)
        x2    invincibility
        x1
        0xA:
        8x    indicates contained within another object (forward or back # objects = preset value)
        4x    indicates object does not use normal presets but is assigned to guard #preset
        2x    (unknown) part of forced collectable objects
        1x    think this sets object to absolute position.  (similar to 2xxx type)
        x8    something to do with free-standing glass (glass walls)
        x4    (unknown) streets buildings/roadblocks
        x2    seems to align image to preset values for glass
        x1    force collisions (2xxx presets, mostly)
        0xB:
            0xxx presets:
            00    default, on ground
            x1    forced to ground
            x2    room upper limit, rotated y 90 degrees.  top faces direction, front faces up
            x4    room upper limit, upside-down
            x8    room upper limit, right side up
        2xxx presets:
            x1    normal placement
            x2    rotated y 90 degrees.  top faces direction, front faces up. (use on obj 68-6B)
            x4    upside-down
            x8    in-air
            1x    scale object to fit completely within preset bounds
            2x    x set to preset bounds
            4x    y set to preset bounds
            8x    z set to preset bounds
    */
    u32 flags;
    /*0xC:
        8x    force maximum explosion radius/disable detecting
                player
        4x    autoturrets: reset to default, not preset position
        2x    no AI interaction
        1x    1-way lock (back)
        x8    1-way lock (front)
        x4
        x2    Objects (rockets, mines, etc) do not collide with object
        x1    don't load 4 player
        0xD:
        8x    don't load 3 player
        4x    don't load 2 player
        2x    immune to explosions (only gunfire damages object)
        1x    bulletproof
        x8    invisible! can't shoot, but can hit with rockets, bugs, etc.  not counted as a hit
        x4
        x2    (unknown) streets buildings
        x1
        0xE:
        8x    Can fire through object
        4x    immune to gunfire (Only explosives damage object)
        2x    Remove from game when destroyed (anything on top doesn't fall off!  Just Poof! gone)
        1x
        x8    only activate at close range
        x4
        x2    explode on contact with tank?
        x1    (unknown) jungle bush
        0xF:
        8x    don't load on 007
        4x    don't load on 00 agent
        2x    don't load on secret agent
        1x    don't load on agent
        x8    don't load multiplayer (difficulty = -1)
        x4    disable activation text
        x2    lightweight (previous: 2xxx drops to ground when destroyed)
        x1    used on stuff in egyptian, sevx
    */
    u32 flags2;
    PropRecord* prop; /*0x10*/
    Model* model;     /*0x14*/

    Mtxf mtx;            /* Hopefully this is 16 words long*/
    coord3d runtime_pos; /*0x58 - 0x60*/
    union {
        /*0x64*
            10000000
            00060000    owner (0-3); used to attribute kills to players
            00004000    activated
            00000200    only set with disabled or destroyed doors
            00000080    depositted (thrown)
            00000004    removes object when set
        */
        u32 runtime_bitflags;
    };
    struct collision_data* ptr_allocated_collisiondata_block;

    union {
        struct Projectile* projectile; // 0x6c
        struct Embedment* embedment;   // 0x6c
    };

    f32 maxdamage;    // 0x70
    f32 damage;       // 0x74
    rgba_u8 shadecol; // 0x78
    rgba_u8 nextcol;  // 0x7C
} ObjectRecord;

struct ALSoundState_s;
typedef struct ALSoundState_s {
    // offset 0
    ALLink link;

    // offset 8
    ALSound* sound;

    // offset 0xc
    ALVoice voice;

    // current playback pitch ... ?
    // offset 0x28
    f32 pitch_28;

    // bendRatio?
    // offset 0x2c
    f32 pitch_2c;

    // play state for this sound
    // offset 0x30
    struct ALSoundState_s* state;

    // offset 0x34
    s16 vol;

    /// offset 0x36
    u8 priority;

    // unused?
    s8 unk37;

    // counter of some kind
    s32 unk38;

    // offset 0x3c = 60
    // pan - 0 = left, 127 = right
    ALPan pan;

    // offset 0x3d = 61
    // wet/dry mix - 0 = dry, 127 = wet
    u8 fxMix;

    // flags?
    // 0x01 = ? used in sndPlaySfx if there is "nextState"
    // 0x02 = decay time flag
    // 0x04 = maybe: voice allocated/started ?
    // 0x08 = ?
    // 0x10 = active flag
    // 0x20 = related to detune
    // 0x40 = ?
    // 0x80 = ?
    u8 unk3e;
    /*
      AL_STOPPED
      AL_PLAYING
      AL_STOPPING
    */
    u8 playingState;

} ALSoundState;

struct hand {
    ITEM_IDS weaponnum;
    ITEM_IDS weaponnum_watchmenu;
    ITEM_IDS previous_weapon;

    s8 weapon_firing_status;

    s8 field_87D;
    s8 field_87E;
    s8 field_87F;

    /**
     * Time holding weapon
     */
    s32 weapon_hold_time;
    s32 field_884;
    s32 field_888;
    s32 field_88C;
    s32 field_890;
    s32 when_detonating_mines_is_0;
    s32 weapon_current_animation;
    s32 weapon_ammo_in_magazine;
    s32 field_8A0;
    s32 field_8A4;
    s32 field_8A8;
    s32 weapon_next_weapon;
    s32 field_8B0;
    s32 weapon_animation_trigger;
    s32 field_8B8;
    s32 field_8BC;
    s32 field_8C0;
    s32 field_8C4;
    s32 field_8C8;
    s32 field_8CC;
    s32 field_8D0;
    s32 field_8D4;
    s32 field_8D8;
    s32 field_8DC;
    s32 field_8E0;
    s32 field_8E4;
    s32 field_8E8;
    Mtxf field_8EC;
    s32 field_92C;
    s32 field_930;
    s32 field_934;
    s32 field_938;
    s32 field_93C;
    s32 field_940;
    f32 field_944;
    s32 field_948;
    f32 field_94C;
    s32 field_950;
    s32 field_954;
    s32 field_958;
    s32 field_95C;
    s32 field_960;
    s32 field_964;
    f32 field_968;
    s32 field_96C;
    f32 field_970;
    s32 field_974;
    coord3d blendpos[4];
    coord3d blendlook[4];
    coord3d blendup[4];
    s32 curblendpos;
    f32 dampt;
    f32 blendscale;
    f32 blendscale1;
    s32 sideflag;
    f32 weapon_theta_displacement;
    f32 weapon_verta_displacement;
    s32 field_A24;
    s32 field_A28;
    s32 field_A2C;
    f32 field_A30;
    f32 field_A34;
    f32 field_A38;
    f32 field_A3C;
    f32 field_A40;
    ALSoundState* audioHandle;
    s32 field_A48;
    s32 field_A4C;
    s32 field_A50;
    ChrRecord_f180 field_A54;
    f32 noise;
    f32 field_A84;
    f32 field_A88;
    s32 field_A8C;
    ObjectRecord* rocket;
    s32 firedrocket;
    s32 field_A98;
    s32 field_A9C;
    s32 field_AA0;
    s32 field_AA4;
    s32 field_AA8;
    s32 field_AAC;
    s32 field_AB0;
    s32 field_AB4;
    s32 field_AB8;
    s32 field_ABC;
    s32 field_AC0;
    s32 field_AC4;
    s32 field_AC8;
    s32 field_ACC;
    s32 field_AD0;
    s32 field_AD4;

    // offset 0xad8
    Mtxf throw_item_pos_related;

    s32 field_B18;
    s32 field_B1C;
    s32 field_B20;
    s32 field_B24;
    s32 field_B28;
    s32 field_B2C;
    s32 field_B30;
    s32 field_B34;
    s32 field_B38;
    s32 field_B3C;
    s32 field_B40;
    s32 field_B44;
    s32 field_B48;
    s32 field_B4C;
    s32 field_B50;
    s32 field_B54;
    coord3d field_B58;
    f32 field_B64;
    s32 field_B68;
    s32 field_B6C;
    s32 field_B70;
    s32 field_B74;
    s32 field_B78;
    s32 field_B7C;
    s32 field_B80;
    s32 field_B84;
    s32 field_B88;
    s32 field_B8C;
    s32 field_B90;
    s32 field_B94;
    s32 field_B98;
    s32 field_B9C;
    s32 field_BA0;
    s32 field_BA4;
    s32 field_BA8;
    s32 field_BAC;
    s32 field_BB0;
    s32 field_BB4;
    s32 field_BB8;
    s32 field_BBC;
    s32 field_BC0;
    s32 field_BC4;
    s32 field_BC8;
    s32 field_BCC;
    s32 field_BD0;
    s32 field_BD4;
    s32 field_BD8;
    s32 field_BDC;
    s32 field_BE0;
    s32 field_BE4;
    s32 field_BE8;
    s32 field_BEC;
    s32 field_BF0;
    s32 field_BF4;
    s32 field_BF8;
    s32 field_BFC;
    s32 field_C00;
    s32 field_C04;
    s32 volley; // Number of bullets discharged in a row. For pistols, it's always 1 even if the fire button is held.
    coord3d item_related;
};

struct damage_display_val {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    s16 unk08;
    s16 unk0A;
    s8 unk0C;
    s8 unk0D;
    s8 unk0E;
    s8 unk0F;
};

struct damage_display_parent {
    struct damage_display_val items[2];
};

typedef struct invitem_weap {
    s32 weapon;
} invitem_weap;

typedef struct invitem_prop {
    PropRecord* prop;
} invitem_prop;

typedef struct invitem_dual {
    s32 weapon_right;
    s32 weapon_left;
} invitem_dual;

typedef struct InvItem {
    s32 type;

    union {
        struct invitem_weap type_weap;
        struct invitem_prop type_prop;
        struct invitem_dual type_dual;

    } type_inv_item;

    struct InvItem* next;
    struct InvItem* prev;

} InvItem;

struct WatchVertex {
    struct coord16 coord1;
    struct coord16 coord2;
    struct rgba_u8 color;
};

struct WatchRectangle {
    struct WatchVertex vtx[4];
};

typedef struct textoverride {

    /*0x00*/ u32 unk1;
    /*0x04*/ s32 objoffset;
    /*0x08*/ s32 weapon;
    /*0x0c*/ u32 titletext1; // 1st title
    /*0x10*/ u32 titletext2; // 2nd title
    /*0x14*/ u32 shorttext;
    /*0x18*/ u32 longtext; // longname
    /*0x1c*/ u32 pickuptext;
    /*0x20*/ struct textoverride* next;
    /*0x24*/ struct ObjectRecord* obj;
} textoverride;

typedef struct gunheld {
    s32 weapon1;
    s32 weapon2;
    s32 totaltime;
} gunheld;

struct player {
    /* 0x0000 */ s32 unknown;

    /**
     * Offset 0x0004.
     */
    coord3d pos;

    /**
     * Offset 0x0010.
     */
    coord3d pos2;

    /**
     * Offset 0x001c.
     */
    coord3d offset;

    /**
     * Offset 0x0028.
     */
    coord3d pos3;

    /**
     * Offset 0x0034.
     */
    StandTile* room_pointer;

    /**
     * Offset 0x0038.
     */
    coord3d current_model_pos;

    /**
     * Offset 0x0044.
     */
    coord3d previous_model_pos;

    /**
     * Offset 0x0050.
     */
    coord3d current_room_pos;

    /**
     * Used as parameter to gbi macro.
     * Offset 0x005c.
     */
    s32 field_5C;

    /* 0x0060 */ s32 field_60;
    /* 0x0064 */ s32 field_64;
    /* 0x0068 */ s32 field_68;

    /**
     * Collision / clipping related.
     * Related to y offset.
     * Offset 0x006c.
     */
    /* 0x006c */ f32 field_6C;

    /**
     * Collision / clipping related.
     * Seems to hold save value as stanHeight.
     * Offset 0x0070.
     */
    /* 0x0070 */ f32 field_70;

    /* 0x0074 0x800c67d4 */ f32 stanHeight;

    /* 0x0078 */ f32 field_78;

    /**
     * Collision / clipping related.
     * Used when descending down stairs/ramp.
     * Offset 0x007c.
     */
    /* 0x007c */ f32 field_7C;

    /* 0x0080 */ f32 field_80;

    /**
     * Collision / clipping related.
     * Offset 0x0084.
     */
    /* 0x0084 */ f32 field_84;

    /**
     * Collision / clipping related.
     * Offset 0x0088.
     */
    /* 0x0088 */ f32 field_88;

    /**
     * Collision / clipping related.
     * Offset 0x008c.
     */
    /* 0x008c */ s32 field_8C;

    /**
     * Collision / clipping related.
     * When Bond falls off a ladder or similar, will "overshoot" vertical default.
     * Current offset is stored in this field.
     *
     * Offset 0x0090.
     */
    /* 0x0090 */ f32 vertical_bounce_adjust;
    /* 0x0094 */ s32 field_94;
    /* 0x0098 */ f32 field_98;

    /**
     * Flag: 0, 1, 2
     */
    /* 0x009c */ s32 crouchpos;

    /**
     * Varies from 0.0f to -100.0f
     * /
    /* 0x00a0 */
    f32 ducking_height_offset;

    /**
     * Crouch related, only used while moving up or down into or
     * out of crouch position.
     */
    /* 0x00a4 */ f32 field_A4;
    /* 0x00a8 */ PropRecord* prop;
    /* 0x00ac */ s32 field_AC;

    /**
     * Offset 0x00b0.
     */
    struct rect4f collision_bounds;

    /* 0x00d0 */ s32 field_D0;
    /* 0x00d4 */ struct Model* ptr_char_objectinstance; // canonically bondsub
    /* 0x00d8 */ s32 bonddead;
    /* 0x00dc */ f32 bondhealth;
    /* 0x00e0 */ f32 bondarmour;
    /* 0x00e4 */ f32 oldhealth;
    /* 0x00e8 */ f32 oldarmour;
    /* 0x00ec */ f32 apparenthealth;
    /* 0x00f0 */ f32 apparentarmour;

#if defined(VERSION_JP) || defined(VERSION_EU)

    /*
     * When a non-negative integer:
     * - hide ammo and aim sight
     * - hide any active speech text
     * - disable shoot and B press interact
     * Otherwise,
     * - undo the above.
     * 0x00f4
     **/
    f32 damageshowtime;

    /**
     * When a non-negative integer:
     * - Show health and body armor overlay if Bond isn't dead.
     * Otherwise,
     * - undo the above.
     * 0x00f8
     **/
    f32 healthshowtime;
#else
    /* See comments above. 0x00f4 */ s32 damageshowtime;
    /* See comments above. 0x00f8 */ s32 healthshowtime;
#endif

    /* 0x00fc */ s32 healthshowmode;
    /* 0x0100 */ s32 field_100;
    /* 0x0104 */ s32 field_104;
    /* 0x0108 */ s32 field_108;
    /* 0x010c */ s32 field_10C;
    /* 0x0120 */ s32 movecentrerelease;
    /* 0x0124 */ s32 lookaheadcentreenabled;
    /* 0x0128 */ s32 automovecentreenabled;
    /* 0x012c */ s32 fastmovecentreenabled;
    /* 0x0120 */ s32 automovecentre;

    /**
     * 0: crosshair shown on screen
     * 1: crosshair hidden
     * Only applies in level, when in control of bond.
     * Address 0x800c6884
     */
    /* 0x0124 */ s32 insightaimmode;
    /* 0x0128 */ s32 autoyaimenabled;
    /* 0x012c */ f32 autoaimy;
    /* 0x0130 */ struct PropRecord* autoaim_target_y;
    /* 0x0134 */ s32 autoyaimtime60;
    /* 0x0138 */ s32 autoxaimenabled;
    /* 0x013c */ f32 autoaimx;
    /* 0x0140 */ struct PropRecord* autoaim_target_x;
    /* 0x0144 */ s32 autoxaimtime60;
    /* 0x0148 */ f32 vv_theta;
    /* 0x014c */ f32 speedtheta;

    /**
     * Computed value from vv_theta, used to calculate boost direction.
     * 0x0150
     **/
    f32 vv_costheta;

    /**
     * Computed value from vv_theta, used to calculate boost direction.
     * 0x0154
     **/
    f32 vv_sintheta;

    /**
     * Vertical look angle.
     * 0x0158
     **/
    f32 vv_verta;

    /**
     * Vertical look angle. Computed value from vv_verta, should always be between 0 and 360 degrees.
     * 0x015c
     **/
    f32 vv_verta360;

    /* 0x0160 */ f32 speedverta;

    /**
     * Computed value from vv_verta360, but otherwise unused?
     * 0x0164
     **/
    f32 vv_cosverta;

    /**
     * Computed value from vv_verta360, but otherwise unused?
     * 0x0168
     **/
    f32 vv_sinverta;

    /* 0x016c */ f32 speedsideways;
    /* 0x0170 */ f32 speedstrafe;
    /* 0x0174 */ f32 speedforwards;

    /**
     * Normal run speed: 1.0
     * After 3 seconds of running: 1.35.
     * Address 0x80c68d8
     */
    /* 0x0178 */ f32 speedboost;

    /**
     * How long Bond has been running
     */
    /* 0x017c */ s32 speedmaxtime60;

    coord3d bondshotspeed; // 0x180

    // offset 0x18c
    f32 bondfadetime60;

    // offset 0x190
    f32 bondfadetimemax60;

    // offset 0x194
    f32 bondfadefracold;

    // offset 0x198
    f32 bondfadefracnew;

    // offset 0x19c
    f32 bondbreathing;

    s32 field_1A0;
    s32 field_1A4;
    s32 field_1A8;
    s32 field_1AC;
    s32 field_1B0;
    s32 field_1B4;
    s32 field_1B8;
    s32 field_1BC;

    /**
     * Current pause time.
     * Resets after every new pause.
     * Address 0x800c6920
     */
    s32 watch_pause_time;

    /* 0x01c4 */ s32 timer_1C4;
    /* 0x01c8 */ s32 watch_animation_state;

    /**
     * 1 = level is active
     * 0 = game is paused
     * Changes to zero only after fully entering watch.
     * Address 0x800c692c
     */
    s32 outside_watch_menu;
    s32 open_close_solo_watch_menu;
    f32 field_1D4;
    f32 field_1D8;
    f32 pause_watch_position;
    f32 field_1E0;
    f32 field_1E4;
    f32 field_1E8;
    f32 field_1EC;
    f32 field_1F0;
    f32 field_1F4;
    s32 field_1F8;
    s32 field_1FC;
    /**
     * Pausing flag.
     * 0 = level is active
     * 1 = begin pause animation. Set as soon as moving arm begins, cleared when moving arm ends.
     * 0x800C6960
     */
    s32 pausing_flag;
    f32 pause_starting_angle;
    f32 pause_related;
    f32 pause_target_angle;
    f32 field_210;
    f32 field_214;
    s32 field_218;
    s32 field_21C;
    s32 step_in_view_watch_animation;
    f32 pause_animation_counter;

    /**
     * Offset 0x0228.
     */
    f32 pause_watch_related;

    /**
     * Looks to be same as pause_watch_related, but adjusted by scale factor.
     * Offset 0x022c.
     */
    f32 pause_watch_related_scaled;

    s32 something_with_watch_object_instance;

    s32 field_234;
    s32 field_238;
    s32 field_23C;
    s32 field_240;
    s32 watch_scale_destination;
    s32 field_248;
    s32 field_24C;
    s32 field_250;
    s32 field_254;

    /**
     * Some kind of adjustment applied before scaling to set pause_watch_related_scaled.
     * Offset 0x0258.
     */
    f32 pause_watch_related_adjust;
    s32 field_25C;
    s32 field_260;
    s32 field_264;
    s32 field_268;
    s32 field_26C;
    s32 field_270;
    s32 field_274;
    s32 field_278;
    s32 field_27C;
    s32 field_280;
    s32 field_284;
    s32 field_288;
    s32 field_28C;
    s32 field_290;
    s32 field_294;
    s32 field_298;
    s32 field_29C;
    s32 field_2A0;
    s32 field_2A4;
    s32 field_2A8;
    s32 field_2AC;
    s32 field_2B0;
    s32 field_2B4;
    s32 field_2B8;
    s32 field_2BC;
    s32 field_2C0;
    s32 field_2C4;
    s32 field_2C8;
    s32 field_2CC;
    s32 field_2D0;
    s32 field_2D4;
    s32 field_2D8;
    s32 field_2DC;
    s32 field_2E0;
    s32 field_2E4;
    s32 field_2E8;
    s32 field_2EC;
    s32 field_2F0;
    s32 field_2F4;
    s32 field_2F8;
    s32 field_2FC;
    s32 field_300;
    s32 field_304;
    s32 field_308;
    s32 field_30C;
    s32 field_310;
    s32 field_314;
    s32 field_318;
    s32 field_31C;
    s32 field_320;
    s32 field_324;
    s32 field_328;
    s32 field_32C;
    s32 field_330;
    s32 field_334;
    s32 field_338;
    s32 field_33C;
    s32 field_340;
    s32 field_344;
    s32 field_348;
    s32 field_34C;
    s32 field_350;
    s32 field_354;
    s32 field_358;
    s32 field_35C;
    s32 field_360;
    s32 field_364;
    s32 field_368;
    s32 field_36C;
    s32 field_370;
    s32 field_374;
    s32 field_378;
    s32 field_37C;
    s32 field_380;
    s32 field_384;
    s32 field_388;
    s32 field_38C;
    s32 field_390;
    s32 field_394;
    s32 field_398;
    s32 field_39C;
    s32 field_3A0;
    s32 field_3A4;
    s32 field_3A8;
    s32 field_3AC;
    s32 field_3B0;

    /**
     * Copy of buttons pressed.
     * Address 0x800C6B17
     */
    u16 buttons_pressed;
    u16 prev_buttons_pressed;

    struct coord3d field_3B8;

    f32 field_3C4;
    f32 field_3C8;
    f32 field_3CC;
    s32 colourscreenred;
    s32 colourscreengreen;
    s32 colourscreenblue;
    f32 colourscreenfrac;
    f32 colourfadetime60;

    /**
     * Offset 0x3e4.
     */
    f32 colourfadetimemax60;
    s32 colourfaderedold;
    s32 colourfaderednew;
    s32 colourfadegreenold;
    s32 colourfadegreennew;
    s32 colourfadeblueold;
    s32 colourfadebluenew;
    f32 colourfadefracold;
    f32 colourfadefracnew;

    /*
     * Something with position, like previous x position.
     * Offset 0x408.
     */
    coord3d bondprevpos; // 0x408

    f32 thetadie; // 0x414
    f32 vertadie; // 0x418
    s32 bondtype;
    s32 startnewbonddie; // 0x420

    /**
     * Multiplayer related.
     * Offset 0x424.
     */
    s32 redbloodfinished;

    /**
     * Multiplayer related.
     * Offset 0x428.
     */
    s32 deathanimfinished;
    s32 field_42c;
    s32 controldef; // 0x430

    struct collision434 previous_collision_info;
    struct collision434 field_488;

    s32 resetheadpos;  // bool
    s32 resetheadrot;  // bool
    s32 resetheadtick; // bool 0x4e4

    s32 headanim; // index into array of pointers. Pointers are for animations.

    // has NTSC<->PAL difference, but not (5/6)
    f32 headdamp;
    s32 headwalkingtime60; // 0x4f0
    f32 headamplitude;     // 0x4f4
    f32 sideamplitude;     // 0x4f8
    struct coord3d headpos;
    struct coord3d headlook;

    struct coord3d headup;
    struct coord3d headpossum;

    // headlooksum[2] has NTSC->PAL conversion rate (5/6)
    struct coord3d headlooksum;

    // headupsum[1] has NTSC->PAL conversion rate (5/6)
    struct coord3d headupsum;
    struct coord3d headbodyoffset; // 0x544
    f32 standheight;               // old name stationary_ground_offset

    // f32[4] ?? or 2 x f32[2] ??
    coord3d standbodyoffset; // 0x554
    f32 standfrac;           // 0x560

    // offset 0x564
    struct coord3d standlook[2];

    // offset 0x57C
    struct coord3d standup[2];

    // offset 0x594
    s32 standcnt;
    Model* model;
    s32 field_59C;
    s32 field_5A0;
    s32 field_5A4;
    s32 field_5A8;
    s32 field_5AC;
    s32 field_5B0;
    s32 field_5B4;
    s32 field_5B8;
    s8 animFlipFlag;
    s8 field_5BD;
    s8 field_5BE;
    s8 field_5BF;

    f32 field_5C0;

    s32 field_5C4;
    s32 field_5C8;
    s32 field_5CC;
    s32 field_5D0;
    s32 field_5D4;
    s32 field_5D8;
    s32 field_5DC;
    s32 field_5E0;
    s32 field_5E4;
    s32 field_5E8;
    s32 field_5EC;
    s32 field_5F0;
    s32 field_5F4;
    s32 field_5F8;
    s32 field_5FC;
    s32 field_600;
    s32 field_604;
    s32 field_608;
    s32 field_60C;
    s32 field_610;
    s32 field_614;
    s32 field_618;
    s32 field_61C;
    s32 field_620;
    s32 field_624;
    s32 field_628;
    s32 field_62C;
    s32 field_630;
    s32 field_634;
    s32 field_638;
    s32 field_63C;
    s32 field_640;
    s32 field_644;
    s32 field_648;
    s32 field_64C;
    s32 field_650;
    s32 field_654;
    s32 field_658;
    s32 field_65C;
    s32 field_660;
    s32 field_664;
    s32 field_668;
    s32 field_66C;
    s32 field_670;
    s32 field_674;
    s32 field_678;
    s32 field_67C;
    s32 field_680;
    s32 field_684;
    s32 field_688;
    s32 field_68C;
    s32 field_690;
    s32 field_694;
    s32 field_698;
    s32 field_69C;
    s32 field_6A0;
    s32 field_6A4;
    s32 field_6A8;
    s32 field_6AC;
    s32 field_6B0;
    s32 field_6B4;
    s32 field_6B8;
    s32 field_6BC;
    s32 field_6C0;
    s32 field_6C4;
    s32 field_6C8;
    s32 field_6CC;

    Mtxf bondheadmatrices[4];

    Vp viewports[2];

    /**
     * Offset 0x7f0.
     */
    s16 viewx;

    /**
     * Offset 0x7f2.
     */
    s16 viewy;

    /**
     * Offset 0x7f4.
     */
    s16 viewleft;

    /**
     * Offset 0x7f6.
     */
    s16 viewtop;

    s32 hand_invisible[2];                      /* 0x7f8*/
    ITEM_IDS hand_item[2];                      /* 0x800 */
    ModelFileHeader* ptr_hand_weapon_buffer[2]; /* 0x808 */

    /**
     * Offset 0x810.
     */
    ModelFileHeader copy_of_body_obj_header[2];

    struct texpool item_related[2]; /* 0x850 */

    /**
     * Offset 0x870.
     */
    struct hand hands[2];
    f32 gunposamplitude;
    f32 gunxamplitude;
    s32 field_FC8;

    /**
     * Used in lvlRender method, in VERSION_JP build.
     * Set to one when holding a grenade (primed to explode).
     * Offset 0xfcc.
     */
    s32 field_FCC;

    // Seems to be copy of field_FCC
    s32 field_FD0;

    s32 z_trigger_timer;
    s32 field_FD8;
    struct rgba_u8 tileColor;
    s32 resetshadecol;

    // unused. Name comes from XBLA debug.
    s32 aimtype;

    /**
     * Crosshair or bullet angle related.
     * Offset 0xfe8.
     */
    coord2d crosshair_angle;
    f32 crosshair_x_pos;
    f32 crosshair_y_pos;
    f32 guncrossdamp;
    coord2d field_FFC;
    f32 gun_azimuth_angle;
    f32 gun_azimuth_turning;
    f32 gunaimdamp;
    coord3d field_1010;
    Mtxf field_101C;
    s32 last_z_trigger_timer;
    s32 copiedgoldeneye;
    s32 gunammooff;
    s32 field_1068;
    f32 gunsync;
    f32 syncchange;
    f32 synccount;
    s32 syncoffset;
    f32 field_107C;
    f32 field_1080;
    f32 sniper_zoom;
    f32 camera_zoom;
    s32 curRoomIndex;

    /**
     * Offset 0x1090.
     */
    f32 c_screenwidth;

    /**
     * Offset 0x1094.
     */
    f32 c_screenheight;

    /**
     * Offset 0x1098.
     */
    f32 c_screenleft;

    /**
     * Offset 0x109c.
     */
    f32 c_screentop;

    /**
     * Offset 0x10a0.
     */
    f32 c_perspnear;

    /**
     * Offset 0x10a4.
     */
    f32 c_perspfovy;

    /**
     * Offset 0x10a8.
     */
    f32 c_perspaspect;

    /**
     * Offset 0x10ac.
     */
    f32 c_halfwidth;

    /**
     * Offset 0x10b0.
     */
    f32 c_halfheight;

    /**
     * Offset 0x10b4.
     */
    f32 c_scalex;

    /**
     * Offset 0x10b8.
     */
    f32 c_scaley;

    /**
     * Offset 0x10bc.
     */
    f32 c_recipscalex;
    f32 c_recipscaley;
    Mtx* field_10C4;
    Mtx* field_10C8;
    Mtxf* field_10CC;
    s32 field_10D0; // ptr

    /**
     * Offset 0x10d4.
     */
    Mtxf* field_10D4;
    Mtx* projmatrix;
    Mtxf* projmatrixf;
    s32 field_10E0; // ptr
    s32 field_10E4; // ptr
    Mtxf* field_10E8;
    Mtxf* field_10EC;
    f32 c_scalelod60;
    f32 c_scalelod;
    f32 c_lodscalez;
    u32 c_lodscalezu32;
    coord3d c_cameratopnorm;
    coord3d c_cameraleftnorm;

    /**
     * Offset 0x1118.
     */
    f32 screenxminf;

    /**
     * Offset 0x111c.
     */
    f32 screenyminf;

    /**
     * Offset 0x1120.
     */
    f32 screenxmaxf;

    /**
     * Offset 0x1124.
     */
    f32 screenymaxf;

    /**
     * Used during level.
     * 0x2: no crosshair
     * 0x0: cross hair shown on screen.
     *
     * Offset 0x1128.
     */
    s32 gunsightmode;
    s32 field_112C;
    s32 ammoheldarr[30];
    u8* bloodImgCur;
    u8* bloodImgNxt;
    u8* bloodImgBufPtrArray[2];
    s32 bloodImgIdx;
    f32 zoomintime;
    f32 zoomintimemax;
    f32 zoominfovy;
    f32 zoominfovyold;
    f32 zoominfovynew;
    f32 fovy;
    f32 aspect;
    s32 hudmessoff;
    s32 bondmesscnt;
    InvItem* ptr_inventory_first_in_cycle;
    InvItem* p_itemcur;
    s32 equipmaxitems;
    s32 equipallguns;
    s32 equipcuritem;
    textoverride* textoverrides;
    gunheld gunheldarr[10];
    s32 magnetattracttime;
    f32 swaytarget;
    f32 swayoffset0;
    f32 swayoffset2;
    f32 field_1280;
    s32 players_cur_animation;
    f32 field_1288;

    /**
     * This buffers button presses.
     * L_TRIGGER and R_TRIGGER do not add entry by themselves.
     * Entry seems to be added only on other button presses.
     * Offset 0x128c.
     */
    u16 cheat_display_text_related[20];

    /**
     * Offset 0x12b4.
     */
    /* 0x12B4 */ u8 something_with_cheat_text;
    /* 0x12B5 */ u8 can_display_cheat_text;
    /* 0x12B6 */ u8 bondinvincible;
    /* 0x12B7 */ u8 field_12B7;
    /* 0x12B8 */ struct damage_display_parent armor_display_values[23];
    /* 0x1598 */ struct damage_display_parent health_display_values[23];

    /**
     * Offset 0x1878
     */
    struct WatchRectangle buffer_for_watch_greenbackdrop_vertices[WATCH_NUMBER_SCREENS];

    /**
     * Watch static, not "static vertices".
     * Offset 0x19b8.
     */
    struct WatchRectangle buffer_for_watch_static_vertices[1];

    /**
     * Offset 0x19f8
     */
    s32 watch_body_armor_bar_gdl; // used in watch

    s32 field_19FC;
    s32 field_1A00;
    s32 field_1A04;
    s32 field_1A08;
    s32 field_1A0C;
    s32 field_1A10;
    s32 field_1A14;
    s32 field_1A18;
    s32 field_1A1C;
    s32 field_1A20;
    s32 field_1A24;
    s32 field_1A28;
    s32 field_1A2C;
    s32 field_1A30;
    s32 field_1A34;
    s32 field_1A38;
    s32 field_1A3C;
    s32 field_1A40;
    s32 field_1A44;
    s32 field_1A48;
    s32 field_1A4C;
    s32 field_1A50;
    s32 field_1A54;
    s32 field_1A58;
    s32 field_1A5C;
    s32 field_1A60;
    s32 field_1A64;
    s32 field_1A68;
    s32 field_1A6C;
    s32 field_1A70;
    s32 field_1A74;
    s32 field_1A78;
    s32 field_1A7C;
    s32 field_1A80;
    s32 field_1A84;
    s32 field_1A88;
    s32 field_1A8C;
    s32 field_1A90;
    s32 field_1A94;
    s32 field_1A98;
    s32 field_1A9C;
    s32 field_1AA0;
    s32 field_1AA4;
    s32 field_1AA8;
    s32 field_1AAC;
    s32 field_1AB0;
    s32 field_1AB4;
    s32 field_1AB8;
    s32 field_1ABC;
    s32 field_1AC0;
    s32 field_1AC4;
    s32 field_1AC8;
    s32 field_1ACC;
    s32 field_1AD0;
    s32 field_1AD4;
    s32 field_1AD8;
    s32 field_1ADC;
    s32 field_1AE0;
    s32 field_1AE4;
    s32 field_1AE8;
    s32 field_1AEC;
    s32 field_1AF0;
    s32 field_1AF4;
    s32 field_1AF8;
    s32 field_1AFC;
    s32 field_1B00;
    s32 field_1B04;
    s32 field_1B08;
    s32 field_1B0C;
    s32 field_1B10;
    s32 field_1B14;
    s32 field_1B18;
    s32 field_1B1C;
    s32 field_1B20;
    s32 field_1B24;
    s32 field_1B28;
    s32 field_1B2C;
    s32 field_1B30;
    s32 field_1B34;
    s32 field_1B38;
    s32 field_1B3C;
    s32 field_1B40;
    s32 field_1B44;
    s32 field_1B48;
    s32 field_1B4C;
    s32 field_1B50;
    s32 field_1B54;
    s32 field_1B58;
    s32 field_1B5C;
    s32 field_1B60;
    s32 field_1B64;
    s32 field_1B68;
    s32 field_1B6C;
    s32 field_1B70;
    s32 field_1B74;
    s32 field_1B78;
    s32 field_1B7C;
    s32 field_1B80;
    s32 field_1B84;
    s32 field_1B88;
    s32 field_1B8C;
    s32 field_1B90;
    s32 field_1B94;
    s32 field_1B98;
    s32 field_1B9C;
    s32 field_1BA0;
    s32 field_1BA4;
    s32 field_1BA8;
    s32 field_1BAC;
    s32 field_1BB0;
    s32 field_1BB4;
    s32 field_1BB8;
    s32 field_1BBC;
    s32 field_1BC0;
    s32 field_1BC4;
    s32 field_1BC8;
    s32 field_1BCC;
    s32 field_1BD0;
    s32 field_1BD4;
    s32 field_1BD8;
    s32 field_1BDC;
    s32 field_1BE0;
    s32 field_1BE4;
    s32 field_1BE8;
    s32 field_1BEC;
    s32 field_1BF0;
    s32 field_1BF4;
    s32 field_1BF8;
    s32 field_1BFC;
    s32 field_1C00;
    s32 field_1C04;
    s32 field_1C08;
    s32 field_1C0C;
    s32 field_1C10;
    s32 field_1C14;
    s32 field_1C18;
    s32 field_1C1C;
    s32 field_1C20;
    s32 field_1C24;
    s32 field_1C28;
    s32 field_1C2C;
    s32 field_1C30;
    s32 field_1C34;
    s32 field_1C38;
    s32 field_1C3C;
    s32 field_1C40;
    s32 field_1C44;
    s32 field_1C48;
    s32 field_1C4C;
    s32 field_1C50;
    s32 field_1C54;
    s32 field_1C58;
    s32 field_1C5C;
    s32 field_1C60;
    s32 field_1C64;
    s32 field_1C68;
    s32 field_1C6C;
    s32 field_1C70;
    s32 field_1C74;
    s32 field_1C78;
    s32 field_1C7C;
    s32 field_1C80;
    s32 field_1C84;
    s32 field_1C88;
    s32 field_1C8C;
    s32 field_1C90;
    s32 field_1C94;
    s32 field_1C98;
    s32 field_1C9C;
    s32 field_1CA0;
    s32 field_1CA4;
    s32 field_1CA8;
    s32 field_1CAC;
    s32 field_1CB0;
    s32 field_1CB4;
    s32 field_1CB8;
    s32 field_1CBC;
    s32 field_1CC0;
    s32 field_1CC4;
    s32 field_1CC8;
    s32 field_1CCC;
    s32 field_1CD0;
    s32 field_1CD4;
    s32 field_1CD8;
    s32 field_1CDC;
    s32 field_1CE0;
    s32 field_1CE4;
    s32 field_1CE8;
    s32 field_1CEC;
    s32 field_1CF0;
    s32 field_1CF4;
    s32 field_1CF8;
    s32 field_1CFC;
    s32 field_1D00;
    s32 field_1D04;
    s32 field_1D08;
    s32 field_1D0C;
    s32 field_1D10;
    s32 field_1D14;
    s32 field_1D18;
    s32 field_1D1C;
    s32 field_1D20;
    s32 field_1D24;
    s32 field_1D28;
    s32 field_1D2C;
    s32 field_1D30;
    s32 field_1D34;
    s32 field_1D38;
    s32 field_1D3C;
    s32 field_1D40;
    s32 field_1D44;
    s32 field_1D48;
    s32 field_1D4C;
    s32 field_1D50;
    s32 field_1D54;
    s32 field_1D58;
    s32 field_1D5C;
    s32 field_1D60;
    s32 field_1D64;
    s32 field_1D68;
    s32 field_1D6C;
    s32 field_1D70;
    s32 field_1D74;
    s32 field_1D78;
    s32 field_1D7C;
    s32 field_1D80;
    s32 field_1D84;
    s32 field_1D88;
    s32 field_1D8C;
    s32 field_1D90;
    s32 field_1D94;
    s32 field_1D98;
    s32 field_1D9C;
    s32 field_1DA0;
    s32 field_1DA4;
    s32 field_1DA8;
    s32 field_1DAC;
    s32 field_1DB0;
    s32 field_1DB4;
    s32 field_1DB8;
    s32 field_1DBC;
    s32 field_1DC0;
    s32 field_1DC4;
    s32 field_1DC8;
    s32 field_1DCC;
    s32 field_1DD0;
    s32 field_1DD4;
    s32 field_1DD8;
    s32 field_1DDC;
    s32 field_1DE0;
    s32 field_1DE4;
    s32 field_1DE8;
    s32 field_1DEC;
    s32 field_1DF0;
    s32 field_1DF4;
    s32 field_1DF8;
    s32 field_1DFC;
    s32 field_1E00;
    s32 field_1E04;
    s32 field_1E08;
    s32 field_1E0C;
    s32 field_1E10;
    s32 field_1E14;
    s32 field_1E18;
    s32 field_1E1C;
    s32 field_1E20;
    s32 field_1E24;
    s32 field_1E28;
    s32 field_1E2C;
    s32 field_1E30;
    s32 field_1E34;
    s32 field_1E38;
    s32 field_1E3C;
    s32 field_1E40;
    s32 field_1E44;
    s32 field_1E48;
    s32 field_1E4C;
    s32 field_1E50;
    s32 field_1E54;
    s32 field_1E58;
    s32 field_1E5C;
    s32 field_1E60;
    s32 field_1E64;
    s32 field_1E68;
    s32 field_1E6C;
    s32 field_1E70;
    s32 field_1E74;
    s32 field_1E78;
    s32 field_1E7C;
    s32 field_1E80;
    s32 field_1E84;
    s32 field_1E88;
    s32 field_1E8C;
    s32 field_1E90;
    s32 field_1E94;
    s32 field_1E98;
    s32 field_1E9C;
    s32 field_1EA0;
    s32 field_1EA4;
    s32 field_1EA8;
    s32 field_1EAC;
    s32 field_1EB0;
    s32 field_1EB4;
    s32 field_1EB8;
    s32 field_1EBC;
    s32 field_1EC0;
    s32 field_1EC4;
    s32 field_1EC8;
    s32 field_1ECC;
    s32 field_1ED0;
    s32 field_1ED4;
    s32 field_1ED8;
    s32 field_1EDC;
    s32 field_1EE0;
    s32 field_1EE4;
    s32 field_1EE8;
    s32 field_1EEC;
    s32 field_1EF0;
    s32 field_1EF4;
    s32 field_1EF8;
    s32 field_1EFC;
    s32 field_1F00;
    s32 field_1F04;
    s32 field_1F08;
    s32 field_1F0C;
    s32 field_1F10;
    s32 field_1F14;
    s32 field_1F18;
    s32 field_1F1C;
    s32 field_1F20;
    s32 field_1F24;
    s32 field_1F28;
    s32 field_1F2C;
    s32 field_1F30;
    s32 field_1F34;
    s32 field_1F38;
    s32 field_1F3C;
    s32 field_1F40;
    s32 field_1F44;
    s32 field_1F48;
    s32 field_1F4C;
    s32 field_1F50;
    s32 field_1F54;
    s32 field_1F58;
    s32 field_1F5C;
    s32 field_1F60;
    s32 field_1F64;
    s32 field_1F68;
    s32 field_1F6C;
    s32 field_1F70;
    s32 field_1F74;
    s32 field_1F78;
    s32 field_1F7C;
    s32 field_1F80;
    s32 field_1F84;
    s32 field_1F88;
    s32 field_1F8C;
    s32 field_1F90;
    s32 field_1F94;
    s32 field_1F98;
    s32 field_1F9C;
    s32 field_1FA0;
    s32 field_1FA4;
    s32 field_1FA8;
    s32 field_1FAC;
    s32 field_1FB0;
    s32 field_1FB4;
    s32 field_1FB8;
    s32 field_1FBC;
    s32 field_1FC0;
    s32 field_1FC4;
    s32 field_1FC8;
    s32 field_1FCC;
    s32 field_1FD0;
    s32 field_1FD4;
    s32 field_1FD8;
    s32 field_1FDC;
    s32 field_1FE0;
    s32 field_1FE4;
    s32 field_1FE8;
    s32 field_1FEC;
    s32 field_1FF0;
    s32 field_1FF4;
    s32 field_1FF8;
    s32 field_1FFC;
    s32 field_2000;
    s32 field_2004;
    s32 field_2008;
    s32 field_200C;
    s32 field_2010;
    s32 field_2014;
    s32 field_2018;
    s32 field_201C;
    s32 field_2020;
    s32 field_2024;
    s32 field_2028;
    s32 field_202C;
    s32 field_2030;
    s32 field_2034;
    s32 field_2038;
    s32 field_203C;
    s32 field_2040;
    s32 field_2044;
    s32 field_2048;
    s32 field_204C;
    s32 field_2050;
    s32 field_2054;
    s32 field_2058;
    s32 field_205C;
    s32 field_2060;
    s32 field_2064;
    s32 field_2068;
    s32 field_206C;
    s32 field_2070;
    s32 field_2074;
    s32 field_2078;
    s32 field_207C;
    s32 field_2080;
    s32 field_2084;
    s32 field_2088;
    s32 field_208C;
    s32 field_2090;
    s32 field_2094;
    s32 field_2098;
    s32 field_209C;
    s32 field_20A0;
    s32 field_20A4;
    s32 field_20A8;
    s32 field_20AC;
    s32 field_20B0;
    s32 field_20B4;
    s32 field_20B8;
    s32 field_20BC;
    s32 field_20C0;
    s32 field_20C4;
    s32 field_20C8;
    s32 field_20CC;
    s32 field_20D0;
    s32 field_20D4;
    s32 field_20D8;
    s32 field_20DC;
    s32 field_20E0;
    s32 field_20E4;
    s32 field_20E8;
    s32 field_20EC;
    s32 field_20F0;
    s32 field_20F4;
    s32 field_20F8;
    s32 field_20FC;
    s32 field_2100;
    s32 field_2104;
    s32 field_2108;
    s32 field_210C;
    s32 field_2110;
    s32 field_2114;
    s32 field_2118;
    s32 field_211C;
    s32 field_2120;
    s32 field_2124;

    /**
     * Offset 0x2128
     */
    s32 watch_health_bar_gdl; // used in watch

    s32 field_212C;
    s32 field_2130;
    s32 field_2134;
    s32 field_2138;
    s32 field_213C;
    s32 field_2140;
    s32 field_2144;
    s32 field_2148;
    s32 field_214C;
    s32 field_2150;
    s32 field_2154;
    s32 field_2158;
    s32 field_215C;
    s32 field_2160;
    s32 field_2164;
    s32 field_2168;
    s32 field_216C;
    s32 field_2170;
    s32 field_2174;
    s32 field_2178;
    s32 field_217C;
    s32 field_2180;
    s32 field_2184;
    s32 field_2188;
    s32 field_218C;
    s32 field_2190;
    s32 field_2194;
    s32 field_2198;
    s32 field_219C;
    s32 field_21A0;
    s32 field_21A4;
    s32 field_21A8;
    s32 field_21AC;
    s32 field_21B0;
    s32 field_21B4;
    s32 field_21B8;
    s32 field_21BC;
    s32 field_21C0;
    s32 field_21C4;
    s32 field_21C8;
    s32 field_21CC;
    s32 field_21D0;
    s32 field_21D4;
    s32 field_21D8;
    s32 field_21DC;
    s32 field_21E0;
    s32 field_21E4;
    s32 field_21E8;
    s32 field_21EC;
    s32 field_21F0;
    s32 field_21F4;
    s32 field_21F8;
    s32 field_21FC;
    s32 field_2200;
    s32 field_2204;
    s32 field_2208;
    s32 field_220C;
    s32 field_2210;
    s32 field_2214;
    s32 field_2218;
    s32 field_221C;
    s32 field_2220;
    s32 field_2224;
    s32 field_2228;
    s32 field_222C;
    s32 field_2230;
    s32 field_2234;
    s32 field_2238;
    s32 field_223C;
    s32 field_2240;
    s32 field_2244;
    s32 field_2248;
    s32 field_224C;
    s32 field_2250;
    s32 field_2254;
    s32 field_2258;
    s32 field_225C;
    s32 field_2260;
    s32 field_2264;
    s32 field_2268;
    s32 field_226C;
    s32 field_2270;
    s32 field_2274;
    s32 field_2278;
    s32 field_227C;
    s32 field_2280;
    s32 field_2284;
    s32 field_2288;
    s32 field_228C;
    s32 field_2290;
    s32 field_2294;
    s32 field_2298;
    s32 field_229C;
    s32 field_22A0;
    s32 field_22A4;
    s32 field_22A8;
    s32 field_22AC;
    s32 field_22B0;
    s32 field_22B4;
    s32 field_22B8;
    s32 field_22BC;
    s32 field_22C0;
    s32 field_22C4;
    s32 field_22C8;
    s32 field_22CC;
    s32 field_22D0;
    s32 field_22D4;
    s32 field_22D8;
    s32 field_22DC;
    s32 field_22E0;
    s32 field_22E4;
    s32 field_22E8;
    s32 field_22EC;
    s32 field_22F0;
    s32 field_22F4;
    s32 field_22F8;
    s32 field_22FC;
    s32 field_2300;
    s32 field_2304;
    s32 field_2308;
    s32 field_230C;
    s32 field_2310;
    s32 field_2314;
    s32 field_2318;
    s32 field_231C;
    s32 field_2320;
    s32 field_2324;
    s32 field_2328;
    s32 field_232C;
    s32 field_2330;
    s32 field_2334;
    s32 field_2338;
    s32 field_233C;
    s32 field_2340;
    s32 field_2344;
    s32 field_2348;
    s32 field_234C;
    s32 field_2350;
    s32 field_2354;
    s32 field_2358;
    s32 field_235C;
    s32 field_2360;
    s32 field_2364;
    s32 field_2368;
    s32 field_236C;
    s32 field_2370;
    s32 field_2374;
    s32 field_2378;
    s32 field_237C;
    s32 field_2380;
    s32 field_2384;
    s32 field_2388;
    s32 field_238C;
    s32 field_2390;
    s32 field_2394;
    s32 field_2398;
    s32 field_239C;
    s32 field_23A0;
    s32 field_23A4;
    s32 field_23A8;
    s32 field_23AC;
    s32 field_23B0;
    s32 field_23B4;
    s32 field_23B8;
    s32 field_23BC;
    s32 field_23C0;
    s32 field_23C4;
    s32 field_23C8;
    s32 field_23CC;
    s32 field_23D0;
    s32 field_23D4;
    s32 field_23D8;
    s32 field_23DC;
    s32 field_23E0;
    s32 field_23E4;
    s32 field_23E8;
    s32 field_23EC;
    s32 field_23F0;
    s32 field_23F4;
    s32 field_23F8;
    s32 field_23FC;
    s32 field_2400;
    s32 field_2404;
    s32 field_2408;
    s32 field_240C;
    s32 field_2410;
    s32 field_2414;
    s32 field_2418;
    s32 field_241C;
    s32 field_2420;
    s32 field_2424;
    s32 field_2428;
    s32 field_242C;
    s32 field_2430;
    s32 field_2434;
    s32 field_2438;
    s32 field_243C;
    s32 field_2440;
    s32 field_2444;
    s32 field_2448;
    s32 field_244C;
    s32 field_2450;
    s32 field_2454;
    s32 field_2458;
    s32 field_245C;
    s32 field_2460;
    s32 field_2464;
    s32 field_2468;
    s32 field_246C;
    s32 field_2470;
    s32 field_2474;
    s32 field_2478;
    s32 field_247C;
    s32 field_2480;
    s32 field_2484;
    s32 field_2488;
    s32 field_248C;
    s32 field_2490;
    s32 field_2494;
    s32 field_2498;
    s32 field_249C;
    s32 field_24A0;
    s32 field_24A4;
    s32 field_24A8;
    s32 field_24AC;
    s32 field_24B0;
    s32 field_24B4;
    s32 field_24B8;
    s32 field_24BC;
    s32 field_24C0;
    s32 field_24C4;
    s32 field_24C8;
    s32 field_24CC;
    s32 field_24D0;
    s32 field_24D4;
    s32 field_24D8;
    s32 field_24DC;
    s32 field_24E0;
    s32 field_24E4;
    s32 field_24E8;
    s32 field_24EC;
    s32 field_24F0;
    s32 field_24F4;
    s32 field_24F8;
    s32 field_24FC;
    s32 field_2500;
    s32 field_2504;
    s32 field_2508;
    s32 field_250C;
    s32 field_2510;
    s32 field_2514;
    s32 field_2518;
    s32 field_251C;
    s32 field_2520;
    s32 field_2524;
    s32 field_2528;
    s32 field_252C;
    s32 field_2530;
    s32 field_2534;
    s32 field_2538;
    s32 field_253C;
    s32 field_2540;
    s32 field_2544;
    s32 field_2548;
    s32 field_254C;
    s32 field_2550;
    s32 field_2554;
    s32 field_2558;
    s32 field_255C;
    s32 field_2560;
    s32 field_2564;
    s32 field_2568;
    s32 field_256C;
    s32 field_2570;
    s32 field_2574;
    s32 field_2578;
    s32 field_257C;
    s32 field_2580;
    s32 field_2584;
    s32 field_2588;
    s32 field_258C;
    s32 field_2590;
    s32 field_2594;
    s32 field_2598;
    s32 field_259C;
    s32 field_25A0;
    s32 field_25A4;
    s32 field_25A8;
    s32 field_25AC;
    s32 field_25B0;
    s32 field_25B4;
    s32 field_25B8;
    s32 field_25BC;
    s32 field_25C0;
    s32 field_25C4;
    s32 field_25C8;
    s32 field_25CC;
    s32 field_25D0;
    s32 field_25D4;
    s32 field_25D8;
    s32 field_25DC;
    s32 field_25E0;
    s32 field_25E4;
    s32 field_25E8;
    s32 field_25EC;
    s32 field_25F0;
    s32 field_25F4;
    s32 field_25F8;
    s32 field_25FC;
    s32 field_2600;
    s32 field_2604;
    s32 field_2608;
    s32 field_260C;
    s32 field_2610;
    s32 field_2614;
    s32 field_2618;
    s32 field_261C;
    s32 field_2620;
    s32 field_2624;
    s32 field_2628;
    s32 field_262C;
    s32 field_2630;
    s32 field_2634;
    s32 field_2638;
    s32 field_263C;
    s32 field_2640;
    s32 field_2644;
    s32 field_2648;
    s32 field_264C;
    s32 field_2650;
    s32 field_2654;
    s32 field_2658;
    s32 field_265C;
    s32 field_2660;
    s32 field_2664;
    s32 field_2668;
    s32 field_266C;
    s32 field_2670;
    s32 field_2674;
    s32 field_2678;
    s32 field_267C;
    s32 field_2680;
    s32 field_2684;
    s32 field_2688;
    s32 field_268C;
    s32 field_2690;
    s32 field_2694;
    s32 field_2698;
    s32 field_269C;
    s32 field_26A0;
    s32 field_26A4;
    s32 field_26A8;
    s32 field_26AC;
    s32 field_26B0;
    s32 field_26B4;
    s32 field_26B8;
    s32 field_26BC;
    s32 field_26C0;
    s32 field_26C4;
    s32 field_26C8;
    s32 field_26CC;
    s32 field_26D0;
    s32 field_26D4;
    s32 field_26D8;
    s32 field_26DC;
    s32 field_26E0;
    s32 field_26E4;
    s32 field_26E8;
    s32 field_26EC;
    s32 field_26F0;
    s32 field_26F4;
    s32 field_26F8;
    s32 field_26FC;
    s32 field_2700;
    s32 field_2704;
    s32 field_2708;
    s32 field_270C;
    s32 field_2710;
    s32 field_2714;
    s32 field_2718;
    s32 field_271C;
    s32 field_2720;
    s32 field_2724;
    s32 field_2728;
    s32 field_272C;
    s32 field_2730;
    s32 field_2734;
    s32 field_2738;
    s32 field_273C;
    s32 field_2740;
    s32 field_2744;
    s32 field_2748;
    s32 field_274C;
    s32 field_2750;
    s32 field_2754;
    s32 field_2758;
    s32 field_275C;
    s32 field_2760;
    s32 field_2764;
    s32 field_2768;
    s32 field_276C;
    s32 field_2770;
    s32 field_2774;
    s32 field_2778;
    s32 field_277C;
    s32 field_2780;
    s32 field_2784;
    s32 field_2788;
    s32 field_278C;
    s32 field_2790;
    s32 field_2794;
    s32 field_2798;
    s32 field_279C;
    s32 field_27A0;
    s32 field_27A4;
    s32 field_27A8;
    s32 field_27AC;
    s32 field_27B0;
    s32 field_27B4;
    s32 field_27B8;
    s32 field_27BC;
    s32 field_27C0;
    s32 field_27C4;
    s32 field_27C8;
    s32 field_27CC;
    s32 field_27D0;
    s32 field_27D4;
    s32 field_27D8;
    s32 field_27DC;
    s32 field_27E0;
    s32 field_27E4;
    s32 field_27E8;
    s32 field_27EC;
    s32 field_27F0;
    s32 field_27F4;
    s32 field_27F8;
    s32 field_27FC;
    s32 field_2800;
    s32 field_2804;
    s32 field_2808;
    s32 field_280C;
    s32 field_2810;
    s32 field_2814;
    s32 field_2818;
    s32 field_281C;
    s32 field_2820;
    s32 field_2824;
    s32 field_2828;
    s32 field_282C;
    s32 field_2830;
    s32 field_2834;
    s32 field_2838;
    s32 field_283C;
    s32 field_2840;
    s32 field_2844;
    s32 field_2848;
    s32 field_284C;
    s32 field_2850;
    s32 field_2854;

    /**
     * Offset 0x2858
     */
    Gfx buffer_for_watch_greenbackdrop_DL[WATCH_NUMBER_SCREENS];

    s32 field_2880;
    s32 field_2884;
    s32 field_2888;
    s32 field_288C;
    s32 field_2890;
    s32 field_2894;
    s32 field_2898;
    s32 field_289C;
    s32 field_28A0;
    s32 field_28A4;
    s32 field_28A8;
    s32 field_28AC;
    s32 field_28B0;
    s32 field_28B4;
    s32 field_28B8;
    s32 field_28BC;
    s32 field_28C0;
    s32 field_28C4;
    s32 field_28C8;
    s32 field_28CC;
    s32 field_28D0;
    s32 field_28D4;
    s32 field_28D8;
    s32 field_28DC;
    s32 field_28E0;
    s32 field_28E4;
    s32 field_28E8;
    s32 field_28EC;
    s32 field_28F0;
    s32 field_28F4;
    s32 field_28F8;
    s32 field_28FC;
    s32 field_2900;
    s32 field_2904;
    s32 field_2908;
    s32 field_290C;
    s32 field_2910;
    s32 field_2914;
    s32 field_2918;
    s32 field_291C;
    s32 field_2920;
    s32 field_2924;
    s32 field_2928;
    s32 field_292C;
    s32 field_2930;
    s32 field_2934;
    s32 field_2938;
    s32 field_293C;
    s32 field_2940;
    s32 field_2944;
    s32 field_2948;
    s32 field_294C;
    s32 field_2950;
    s32 field_2954;
    s32 field_2958;
    s32 field_295C;
    s32 field_2960;
    s32 field_2964;
    s32 field_2968;
    s32 field_296C;
    s32 field_2970;
    s32 field_2974;
    s32 field_2978;
    s32 field_297C;
    s32 field_2980;
    s32 field_2984;
    s32 field_2988;
    s32 field_298C;
    s32 field_2990;
    s32 field_2994;

    /**
     * Watch static, not "static DL".
     * Offset 0x2998
     */
    Gfx buffer_for_watch_static_DL[1];

    s32 field_29A0;
    s32 field_29A4;
    s32 field_29A8;
    s32 field_29AC;
    s32 field_29B0;
    s32 field_29B4;

    // Alt field_29C0 ?? Used in EU.
    s32 healthDamageType;

    /**
     * Related to player perspective.
     * Offset 0x29bc.
     */
    f32 field_29BC;
    f32 field_29C0;

    s32 mpmenuon;
    s32 mpmenumode;
    s32 mpquitconfirm;
    s32 mpjoywascentre;
    s32 damagetype;
    s32 deathcount;
    s32 num_suicides;
    s32 field_29E0;
    s32 last_kill_time[4];

    /**
     * Holds mission offset timer value.
     * Offset 0x29f4.
     */
    s32 field_29F4;

    s32 field_29F8;
    s32 autocrouchpos;
    s32 healthdisplaytime;

    /**
     * Current tile pointer -> room.
     *
     * Offset 0x2a04.
     */
    s16 field_2A04;
    f32 field_2A08;
    f32 field_2A0C;
    s32 ptr_text_first_mp_award;
    s32 ptr_text_second_mp_award;
    coord3d field_2A18[2];
    s32 field_2A30;
    s32 field_2A34;
    s32 cur_item_weapon_getname;
    /* 0x2a3c */ f32 actual_health;
    /* 0x2a40 */ f32 actual_armor;
    ITEM_IDS field_2A44[2];
    f32 speedgo;
    s32 lock_hand_model[2];
    s32 cur_player_control_type_0;
    s32 cur_player_control_type_1;
    f32 cur_player_control_type_2;
    s32 neg_vspacing_for_control_type_entry;
    u32 has_set_control_type_data;
    /**
     * Collision / clipping related.
     * Related to 0ffset 0x2a70.
     * Offset 0x2a6c.
     */
    s32 field_2A6C;

    /**
     * Collision / clipping related.
     * Offset 0x2a70
     */
    struct StandTile* field_2A70;

    s32 field_2A74;
    s32 field_2A78;
    s32 field_2A7C;
};

#endif