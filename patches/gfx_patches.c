#include "patches.h"
#include "types.h"

#if 0
BiggerGfxPool *gBiggerGfxPool;
BiggerGfxPool *gBiggerGfxPools[2];
#else
#define gBiggerGfxPool gGfxPool
#define gBiggerGfxPools gGfxPools
#endif

#if 1

RECOMP_PATCH void create_gfx_task_structure(void) {
    s32 entries = gDisplayListHead - gBiggerGfxPool->buffer;

    gGfxSPTask->msgqueue = &gGfxVblankQueue;
    gGfxSPTask->msg = (OSMesg) 2;
    gGfxSPTask->task.t.type = M_GFXTASK;
    gGfxSPTask->task.t.ucode_boot = rspF3DBootStart;
    gGfxSPTask->task.t.ucode_boot_size = ((u8*) rspF3DBootEnd - (u8*) rspF3DBootStart);
    gGfxSPTask->task.t.flags = 0;
    gGfxSPTask->task.t.ucode = rspF3DStart;
    gGfxSPTask->task.t.ucode_data = rspF3DDataStart;
    gGfxSPTask->task.t.ucode_size = SP_UCODE_SIZE; // (this size is ignored)
    gGfxSPTask->task.t.ucode_data_size = SP_UCODE_DATA_SIZE;
    gGfxSPTask->task.t.dram_stack = (u64*) gGfxSPTaskStack;
    gGfxSPTask->task.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
    gGfxSPTask->task.t.output_buff = gGfxSPTaskOutputBuffer;
    gGfxSPTask->task.t.output_buff_size = (u64*) ((u8*) gGfxSPTaskOutputBuffer + sizeof(gGfxSPTaskOutputBuffer));
    gGfxSPTask->task.t.data_ptr = (u64*) &gBiggerGfxPool->buffer;
    gGfxSPTask->task.t.data_size = entries * sizeof(Gfx);
    gGfxSPTask->task.t.yield_data_ptr = (u64*) gGfxSPTaskYieldBuffer;
    gGfxSPTask->task.t.yield_data_size = OS_YIELD_DATA_SIZE;
}

RECOMP_PATCH void render_init(void) {
    gBiggerGfxPool = &gBiggerGfxPools[0];
    set_segment_base_addr(1, gBiggerGfxPool->buffer);
    gGfxSPTask = &gBiggerGfxPool->spTask;
    gDisplayListHead = gBiggerGfxPool->buffer;
    gGfxPoolEnd = (u8*) (gBiggerGfxPool->buffer + GFX_POOL_SIZE);
    init_rcp();
    clear_framebuffer(0);
    end_master_display_list();
    exec_display_list(&gBiggerGfxPool->spTask);
    
    sRenderingFramebuffer++;
    gGlobalTimer++;
}

RECOMP_PATCH void select_gfx_pool(void) {
    gBiggerGfxPool = &gBiggerGfxPools[gGlobalTimer % ARRAY_COUNT(gBiggerGfxPools)];
    set_segment_base_addr(1, gBiggerGfxPool->buffer);
    gGfxSPTask = &gBiggerGfxPool->spTask;
    gDisplayListHead = gBiggerGfxPool->buffer;
    gGfxPoolEnd = (u8*) (gBiggerGfxPool->buffer + GFX_POOL_SIZE);

    gEXEnable(gDisplayListHead++);

    // @recomp Send the current framerate to RT64, including any extra VI interrupt periods.
    gEXSetRefreshRate(gDisplayListHead++, 60 / 2);
}
#endif


// Using this function patch as is from the decomp makes the game hang up
#if 0
extern void (*gGoddardVblankCallback)(void);

RECOMP_PATCH void display_and_vsync(void)  {
    profiler_log_thread5_time(BEFORE_DISPLAY_LISTS);
    
    osRecvMesg(&gGfxVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    
    if (gGoddardVblankCallback != NULL) {
        gGoddardVblankCallback();
        gGoddardVblankCallback = NULL;
    }
    
    exec_display_list(&gBiggerGfxPool->spTask);
    profiler_log_thread5_time(AFTER_DISPLAY_LISTS);
    osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    osViSwapBuffer((void *) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[sRenderedFramebuffer]));
    profiler_log_thread5_time(THREAD5_END);
    osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);

    if (++sRenderedFramebuffer == 3) {
        sRenderedFramebuffer = 0;
    }
    if (++sRenderingFramebuffer == 3) {
        sRenderingFramebuffer = 0;
    }
    gGlobalTimer++;
}
#endif
