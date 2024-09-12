#include "post-processing-effects.h"
#include "utilities.c"

KernelEffectParamaters3x3 rand_kernel_effect_param() {
    KernelEffectParamaters3x3 k;
    k.offset = 1.f / 300;

    float total = 0;
    for(int i = 0; i < 9; i++) {
        float weight = 9 * norm_rand();
        k.kernel[i] = weight;
        total += weight;
    }
    for(int i = 0; i < 9; i++) {
        k.kernel[i] *= 1.f / total;
    }
    return k;
}

KernelEffectParamaters3x3 rand_kernel_effect_param_v2() {
    KernelEffectParamaters3x3 k;
    k.offset = 1.f / 300;

    float total = 0;
    for(int i = 0; i < 9; i++) {
        float weight = norm_rand() - 0.2f;
        k.kernel[i] = weight;
        total += weight;
    }
    for(int i = 0; i < 9; i++) {
        k.kernel[i] *= 1.f / total;
    }
    return k;
}