#include <stdio.h>

#include "utils.h"
#include "config.h"

int main(int argc, char* argv[]){
    
    // checks correct usage
    if (argc != 2) return 1;

    // Neurons
    float inputNeurons[imageSize];
    float l1Neurons[l1Size];
    float l2Neurons[l2Size];
    float outputNeurons[10];

    // Weights
    float inputWeights[imageSize][l1Size];
    float l1Weights[l1Size][l2Size];
    float l2Weights[l2Size][10];

    // Biases
    float l1Bias[l1Size];
    float l2Bias[l2Size];
    float outputBias[10];

    // tryes to open the image
    FILE* img = fopen(argv[1], "rb");
    if (!img) return 2;

    // fills a buffer with pixel data
    uint8_t imgBuffer[imageSize];
    size_t bytesRead = fread(imgBuffer, 1, imageSize, img);
    if (bytesRead != imageSize) return 3;

    // normalizes the buffer into input layer
    for (int i = 0; i < imageSize; i++){
        inputNeurons[i] = normalize(imgBuffer[i]);
    }

    fclose(img);
    return 0;
}