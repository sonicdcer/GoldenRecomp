#include "patches.h"

#if 1

#define PI 3.14159265358979323846f
#define TWO_PI (2.0f * PI)

float sinf_recomp(float x) {
    while (x > PI) x -= TWO_PI;
    while (x < -PI) x += TWO_PI;
    float x2 = x * x;
    float result = x;
    float term = x;

    term *= -x2 / (2.0f * 3.0f);
    result += term;

    term *= -x2 / (4.0f * 5.0f);
    result += term;

    term *= -x2 / (6.0f * 7.0f);
    result += term;

    term *= -x2 / (8.0f * 9.0f);
    result += term;

    return result;
}

float cosf_recomp(float x) {
    return sinf_recomp(x + (PI / 2.0f));
}
#endif