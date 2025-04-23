#include "patches.h"
#include "PR/libaudio.h"

#define ROM_MUSIC_START_OFFSET 0x10000U
#define NUM_MUSIC_TRACKS 63
#define ALIGN16_a(val) (((val) + 0xf | 0xf) ^ 0xf)

typedef enum MUSIC_TRACKS {
    M_NONE,
    M_SHORT_SOLO_DEATH,
    M_INTRO,
    M_TRAIN,
    M_DEPOT,
    M_MPTHEME,
    M_CITADEL,
    M_FACILITY,
    M_CONTROL,
    M_DAM,
    M_FRIGATE,
    M_ARCHIVES,
    M_SILO,
    M_MPTHEME2,
    M_STREETS,
    M_BUNKER1,
    M_BUNKER2,
    M_STATUE,
    M_ELEVATOR_CONTROL,
    M_CRADLE,
    M_UNK,
    M_ELEVATOR_WC,
    M_EGYPTIAN,
    M_FOLDERS,
    M_WATCH,
    M_AZTEC,
    M_WATERCAVERNS,
    M_DEATHSOLO,
    M_SURFACE2,
    M_TRAINX,
    M_UNK2,
    M_FACILITYX,
    M_DEPOTX,
    M_CONTROLX,
    M_WATERCAVERNSX,
    M_DAMX,
    M_FRIGATEX,
    M_ARCHIVESX,
    M_SILOX,
    M_EGYPTIANX,
    M_STREETSX,
    M_BUNKER1X,
    M_BUNKER2X,
    M_JUNGLEX,
    M_INTROSWOOSH,
    M_STATUEX,
    M_AZTECX,
    M_EGYPTX,
    M_CRADLEX,
    M_CUBA,
    M_RUNWAY,
    M_RUNWAYPLANE,
    M_MPTHEME3,
    M_WIND,
    M_GUITARGLISS,
    M_JUNGLE,
    M_RUNWAYX,
    M_SURFACE1,
    M_MPDEATH,
    M_SURFACE2X,
    M_SURFACE2END,
    M_STATUEPART,
    M_END_SOMETHING
} MUSIC_TRACKS;

typedef struct {
    // address is offset from the start of .sbk file
    u8* address;

    // seq length after uncompressed.
    u16 uncompressed_len;

    // len is data segment length in the rom. This is the 1172 compressed length.
    u16 len;
} RareALSeqData;

/**
 * Structure for storing collection of sequence metadatas.
 * These are stored 1172 compressed.
 * Based on original ALSeqFile in n64devkit\ultra\usr\include\PR\libaudio.h.
 */
typedef struct {
    /**
     * number of sequences.
     */
    u16 seqCount;

    /**
     * Unknown, maybe unused padding.
     */
    u16 unk;

    /**
     * ARRAY of sequence info. This is a "dynamic" array, more space
     * will be allocated from ALHeap at runtime.
     */
    RareALSeqData seqArray[1];
} RareALSeqBankFile;

struct music_struct_b {
    u8 data[8438];
    u8* seqData;
};

struct huft {
    u8 e; /* number of extra bits or operation */
    u8 b; /* number of bits in this code or subcode */
    union {
        u16 n;          /* literal, length base, or distance base */
        struct huft* t; /* pointer to next level of table */
    } v;
};

extern s8 g_sndBootswitchSound;
extern s32 g_musicXTrack1CurrentTrackNum;
extern ALCSPlayer* g_musicXTrack1SeqPlayer;
extern u16 g_musicTrackLength[64];
extern u16 g_musicTrackCompressedLength[63];
extern u8* g_musicXTrack1SeqData;
extern RareALSeqBankFile* g_musicDataTable;
extern ALCSeq g_musicXTrack1Seq;

s32 alCSPGetState(ALCSPlayer* seqp);
void musicTrack1Stop(void);
void musicTrack1ApplySeqpVol(u16 volume);
void romCopy(void* target, void* source, u32 size);
u32 decompressdata(u8* src, u8* dst, struct huft* hlist);
u16 musicTrack1GetVolume(void);

#if 1
RECOMP_PATCH void musicTrack1Play(s32 track) __attribute__((optnone)) {
    u32 trackSizeBytes;
    struct music_struct_b thing;
    u8* temp_a0;
    void* romAddress;
    u32 t3;
    struct huft hlist;

    if (g_sndBootswitchSound) {
        return;
    }

    if (g_musicXTrack1CurrentTrackNum) {
        musicTrack1Stop();
    }

    g_musicXTrack1CurrentTrackNum = track;

    while (alCSPGetState(g_musicXTrack1SeqPlayer)) {
        // @recomp: Yield the thread so it doesn't stall
        yield_self_1ms();
    }

    romAddress = g_musicDataTable->seqArray[g_musicXTrack1CurrentTrackNum].address;

    if (romAddress < (void*) ROM_MUSIC_START_OFFSET) {
        // Note: recursive call
        musicTrack1Play(M_SHORT_SOLO_DEATH);

        return;
    }

    t3 = ALIGN16_a(g_musicTrackLength[g_musicXTrack1CurrentTrackNum]) + ALIGN16_a(NUM_MUSIC_TRACKS);
    trackSizeBytes = ALIGN16_a(g_musicTrackCompressedLength[g_musicXTrack1CurrentTrackNum]);
    thing.seqData = g_musicXTrack1SeqData;
    temp_a0 = (u8*) ((t3 + (s32) thing.seqData) - trackSizeBytes);

    romCopy(temp_a0, romAddress, trackSizeBytes);
    decompressdata(temp_a0, thing.seqData, &hlist);

    alCSeqNew(&g_musicXTrack1Seq, g_musicXTrack1SeqData);
    alCSPSetSeq(g_musicXTrack1SeqPlayer, &g_musicXTrack1Seq);
    musicTrack1ApplySeqpVol(musicTrack1GetVolume());
    alCSPPlay(g_musicXTrack1SeqPlayer);
}
#endif