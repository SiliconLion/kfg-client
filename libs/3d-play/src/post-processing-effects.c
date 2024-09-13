#include "post-processing-effects.h"
#include "utilities.h"

KernelEffectParamaters3x3 IDENTITY_KERNEL = {
        .kernel = {
                0.f, 0.f, 0.f,
                0.f, 1.f, 0.f,
                0.f, 0.f, 0.f
        },
        .offset = 1.f / 100.f //shouldnt really matter
};

KernelEffectParamaters3x3 BLUR_KERNEL = {
        .kernel = {
                1.0 / 16, 2.0 / 16, 1.0 / 16,
                2.0 / 16, 4.0 / 16, 2.0 / 16,
                1.0 / 16, 2.0 / 16, 1.0 / 16
        },
        .offset = 1.f / 400
};

KernelEffectParamaters3x3 SHARPEN_KERNEL = {
        .kernel = {
                -1.f, -1.f, -1.f,
                -1.f, 9.f, -1.f,
                -1.f, -1.f, -1.f
        },
        .offset = 1.f / 300
};

KernelEffectParamaters3x3 WEIRD_KERNEL = {
        .kernel = {
                1.f, -1.f, 1.f,
                -1.f, 1.f, 2.f,
                1.f, -2.f, -1.f
        },
        .offset = 1.f / 300
};

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