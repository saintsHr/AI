#pragma once

#include <stdint.h>
#include <math.h>

static inline float sigmoid(float x){
    return 1.0f / (1.0f + expf(-x));
}

static inline float wSum(float v[], float w[], int size){
    float result = 0;
    for (int i = 0; i < size; i++){
        result += (v[i] * w[i]);
    }
    return result;
}

static inline float neuronActivation(float inputs[], float weights[], int size, float bias){
    float sum = wSum(inputs, weights, size);
    sum += bias;
    return sigmoid(sum);
}