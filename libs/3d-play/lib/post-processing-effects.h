#pragma once


typedef struct {
    float kernel[9];
    float offset;
} KernelEffectParamaters3x3;

extern KernelEffectParamaters3x3 IDENTITY_KERNEL;
extern KernelEffectParamaters3x3 BLUR_KERNEL;
extern KernelEffectParamaters3x3 SHARPEN_KERNEL;
extern KernelEffectParamaters3x3 WEIRD_KERNEL;

extern KernelEffectParamaters3x3 rand_kernel_effect_param();

extern KernelEffectParamaters3x3 rand_kernel_effect_param_v2();