#include "patches.h"

#include "PR/R4300.h"
#include "PR/ultratypes.h"
#include "PR/os.h"
#include "misc_funcs.h"

int dummy;
int dummy3 = 0;

u32 __osSetFpcCsr(u32);
u32 __osSetFpcCsr(u32);
void* setSPToEnd(u8* stack, u32 size);
void mainproc(void* args);
void osInitialize(void);
int recomp_printf(const char* fmt, ...);
s32 boot_osPiRawStartDma(s32, u32, void*, u32);
void osCreateThread(OSThread*, OSId, void (*)(void*), void*, void*, OSPri);

extern u8 sp_main[32768];
extern OSThread mainThread;
extern s32* stack_pointer;

#if 1
RECOMP_PATCH void init(void) {
    s32 inflate_code_size;
    s32* stack_pointer;

    // TODO: Validate if this is even needed.
#if 1
    recomp_load_overlays((u32) 0x00001050, (void*) 0x80000450, 0x20860);

    // recomp_printf("recomp_load_overlays: %x, vAddr: %x, size: %x\n", (u32) 0x00101000, 0x80100400,
    //               inflate_code_size - (0x101000 - 0x1050));
    inflate_code_size = (s32) ((u32) 0x00033260 - (u32) 0x00001050);
    if (inflate_code_size > (0x101000 - 0x1050)) {
        boot_osPiRawStartDma(OS_READ, (u32) 0x00101000, (void*) 0x80100400, inflate_code_size - (0x101000 - 0x1050));
    }
#endif

    osInitialize();

    /**
     * Needed for the runtime as the function cache is insuficient,
     * so we treat the whole section as an overlay
     */
    recomp_load_overlays((u32) 0x0005F3C0, (void*) 0x80400000, ((u32) 0xE2D50));

    boot_osPiRawStartDma(OS_READ, (u32) 0x0005F3C0, (void*) 0x80400000, ((u32) 0xE2D50));

    stack_pointer = setSPToEnd(sp_main, sizeof(sp_main));
    osCreateThread(&mainThread, (OSId) 3, &mainproc, NULL, stack_pointer, (OSPri) 10);
    osStartThread(&mainThread);
}
#endif

/**
 * Stubbing this since it causes the runtime to crash while trying to resolve rmonMain
 * It isn't needed for the game anyway
 */
#if 1
RECOMP_PATCH void rmonCreateThread(void) {
    return;
}
#endif
