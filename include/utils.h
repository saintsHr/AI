#pragma once

#include <stdint.h>
#include <math.h>

static inline float sigmoid(float x){
    return 1.0f / (1.0f + expf(-x));
}

static inline float sigmoidDerv(float a){
    return a * (1 - a);
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

static inline void shuffleDataset(float* images, uint8_t* labels, int count, int imageSize){
    for (int i = count - 1; i > 0; i--){
        int j = rand() % (i + 1);

        uint8_t tempLabel = labels[i];
        labels[i] = labels[j];
        labels[j] = tempLabel;

        for (int p = 0; p < imageSize; p++){
            float tmp = images[i * imageSize + p];
            images[i * imageSize + p] = images[j * imageSize + p];
            images[j * imageSize + p] = tmp;
        }
    }
}