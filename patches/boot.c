#include "patches.h"

#define ALIGN16(val) (((val) + 0xF) & ~0xF)
int dummy;
int dummy3 = 0;
extern OSIoMesg gDmaIoMesg;
extern OSMesgQueue gDmaMesgQueue;
extern OSMesg gMainReceivedMesg;

#if 1
RECOMP_PATCH void dma_read(u8 *dest, u8 *srcStart, u8 *srcEnd) {
    u32 size = ALIGN16(srcEnd - srcStart);

    recomp_load_overlays((u32) srcStart, (u32*) dest, size);
    // recomp_printf("ROM: %p \n RAM: %p \n SIZE: %d \n", srcStart, dest, size);

    while (size != 0) {
        u32 copySize = (size >= 0x1000) ? 0x1000 : size;

        osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ, (uintptr_t) srcStart, dest, copySize,
                     &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);

        dest += copySize;
        srcStart += copySize;
        size -= copySize;
    }
}
#endif

typedef struct MainPoolBlock {
    struct MainPoolBlock *prev;
    struct MainPoolBlock *next;
} MainPoolBlock;

void dma_read(u8 *dest, u8 *srcStart, u8 *srcEnd);
void *main_pool_alloc(u32 size, u32 side);
extern MainPoolBlock *sPoolListHeadR;
extern volatile s32 gAudioFrameCount;

#if 1
RECOMP_PATCH void wait_for_audio_frames(s32 frames) {
    // recomp_printf("gAudioFrameCount: %d\n frames: %d\n", gAudioFrameCount, frames);
    gAudioFrameCount = 0;
    // Sound thread will update gAudioFrameCount
    
    // @recomp: yield the thread so the audio thread has a chance to update.
    while (gAudioFrameCount < frames) {
        // spin
        yield_self_1ms();
    }
    
}
#endif