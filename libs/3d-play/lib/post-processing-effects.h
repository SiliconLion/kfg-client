#pragma once


typedef struct {
    float kernel[9];
    float offset;
} KernelEffectParamaters3x3;

KernelEffectParamaters3x3 IDENTITY_KERNEL;
KernelEffectParamaters3x3 BLUR_KERNEL;
KernelEffectParamaters3x3 SHARPEN_KERNEL;
KernelEffectParamaters3x3 WEIRD_KERNEL;

KernelEffectParamaters3x3 rand_kernel_effect_param();

KernelEffectParamaters3x3 rand_kernel_effect_param_v2();