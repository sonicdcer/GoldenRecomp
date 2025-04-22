#include "patches.h"

#include "PR/R4300.h"
#include "PR/ultratypes.h"
#include "PR/os.h"
#include "misc_funcs.h"

int dummy;
int dummy3 = 0;

void osCreateThread(OSThread*, OSId, void (*)(void*), void*, void*, OSPri);
u32 __osSetFpcCsr(u32);
u32 __osSetFpcCsr(u32);
void* setSPToEnd(u8* stack, u32 size);
void mainproc(void* args);
void osInitialize(void);
int recomp_printf(const char* fmt, ...);
s32 boot_osPiRawStartDma(s32, u32, void*, u32);

extern u8 sp_main[32768];
extern OSThread mainThread;
extern s32* stack_pointer;

#if 1
RECOMP_PATCH void init(void) {
    s32 inflate_code_size;
    s32* stack_pointer;

    // TODO: Validate if this is even needed.
#if 1
    recomp_load_overlays((u32) 0x00001050, (void*) 0x80000450, 0x205F0);

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
    recomp_load_overlays((u32) 0x0005F0F0, (void*) 0x80400000, ((u32) 0xE2D50));

    boot_osPiRawStartDma(OS_READ, (u32) 0x0005F0F0, (void*) 0x80400000, ((u32) 0xE2D50));

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

// TODO: This needs to be solved. Stubbing the audio thread as it crashes the game.
#if 1
RECOMP_PATCH void amMain(void* arg) {
    return;
}
#endif

/**
 *  These are very literal C implementations of __f_to_ll and __ll_to_d
 *  I used them for testing instead of the ones i wrote in recomp_api.cpp
 *  they seem to have the same result.
 */
#if 0
long long __f_to_ll_nointrinsic(float f) {
    // Interpret float bits as unsigned int
    union {
        float f;
        unsigned u;
    } bits;
    bits.f = f;

    unsigned sign = bits.u >> 31;
    unsigned exponent = (bits.u >> 23) & 0xFF;
    unsigned fraction = bits.u & 0x7FFFFF;

    // Handle special values (NaN, Inf)
    if (exponent == 0xFF) {
        if (sign) {
            return -9223372036854775807LL - 1; // LLONG_MIN
        } else {
            return 9223372036854775807LL; // LLONG_MAX
        }
    }

    int e = (int) exponent - 127;

    // If less than 0, the value is < 1.0, so return 0
    if (e < 0) {
        return 0;
    }

    // Add the implicit leading 1 for normalized values
    unsigned mantissa = fraction | 0x800000;

    long long result;

    if (e > 31) {
        // e - 23 could be >= 41 here, careful with shift
        if (e >= 63) {
            return sign ? -9223372036854775807LL - 1 : 9223372036854775807LL;
        }
        result = (long long) mantissa << (e - 23);
    } else {
        result = (long long) (mantissa >> (23 - e));
    }

    return sign ? -result : result;
}

double __ll_to_d_nointrinsic(long long x) {
    union {
        double d;
        unsigned int u[2]; // u[0] = high, u[1] = low on big-endian
    } result;

    if (x == 0) {
        result.u[0] = 0;
        result.u[1] = 0;
        return result.d;
    }

    unsigned int sign = 0;
    unsigned long long ux;
    if (x < 0) {
        sign = 1;
        ux = (unsigned long long) (-x);
    } else {
        ux = (unsigned long long) (x);
    }

    // Find the highest bit set (equivalent to log2)
    int shift = 63;
    while ((ux >> shift) == 0) {
        shift--;
    }

    // Exponent with bias for double (bias = 1023)
    int exponent = shift + 1023;

    // Normalize: keep 53 bits (1.ffff...) and round
    unsigned long long mantissa;
    if (shift <= 52) {
        mantissa = ux << (52 - shift);
    } else {
        int extra = shift - 52;
        unsigned long long round_mask = (1ULL << extra) - 1;
        unsigned long long round_bits = ux & round_mask;
        mantissa = ux >> extra;

        // Round to nearest, tie to even
        if ((round_bits > (1ULL << (extra - 1))) || ((round_bits == (1ULL << (extra - 1))) && (mantissa & 1))) {
            mantissa++;
            if (mantissa == (1ULL << 53)) {
                mantissa >>= 1;
                exponent++;
            }
        }
    }

    // Drop implicit 1 (bit 52)
    mantissa &= ~(1ULL << 52);

    // Compose high and low 32-bit words
    unsigned int high = (sign << 31) | ((unsigned int) exponent << 20) | (unsigned int) (mantissa >> 32);
    unsigned int low = (unsigned int) (mantissa & 0xFFFFFFFF);

    result.u[0] = high; // big-endian
    result.u[1] = low;

    return result.d;
}
#endif
