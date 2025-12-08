#include <stdio.h>

#include "utils.h"
#include "config.h"
#include "bmp.h"

int main(int argc, char* argv[]){
    
    // checks correct usage
    if (argc != 2) return 1;

    // Neurons
    float inputNeurons[imageSize];
    float l1Neurons[l1Size];
    float l2Neurons[l2Size];
    float outputNeurons[10];

    // Weights
    float inputWeights[l1Size][imageSize];
    float l1Weights[l2Size][l1Size];
    float l2Weights[10][l2Size];

    // Biases
    float l1Bias[l1Size];
    float l2Bias[l2Size];
    float outputBias[10];

    // tryes to open the image
    FILE* img = fopen(argv[1], "rb");
    if (!img) return 2;

    // .bmp headers
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    // fills headers
    fread(&fileHeader, sizeof(fileHeader), 1, img);
    fread(&infoHeader, sizeof(infoHeader), 1, img);

    // checks if file is .bmp
    if (fileHeader.bfType != 0x4D42) return 3;
    // checks grayscale
    if (infoHeader.biBitCount != 8) return 4;
    // checks correct size
    if (infoHeader.biWidth != imgW || infoHeader.biHeight != imgH) return 5;

    // jump to pixels position
    fseek(img, fileHeader.bfOffBits, SEEK_SET);

    uint8_t buffer[imageSize];

    // fills buffer with pixel data (inverted)
    for (int row = imgH - 1; row >= 0; row--) {
        fread(buffer + (row * imgW), 1, imgW, img);
    }

    fclose(img);

    // fills input layer with buffer
    for (int i = 0; i < imageSize; i++) {
        inputNeurons[i] = buffer[i] / 255.0f;
    }

    // processes input -> layer 1
    for (int i = 0; i < l1Size; i++) {
        l1Neurons[i] = sigmoid(wSum(inputNeurons, inputWeights[i], imageSize) + l1Bias[i]);
    }

    // processes layer 1 -> layer 2
    for (int i = 0; i < l2Size; i++) {
        l2Neurons[i] = sigmoid(wSum(l1Neurons, l1Weights[i], l1Size) + l2Bias[i]);
    }

    // processes layer 2 -> output
    for (int i = 0; i < 10; i++) {
        outputNeurons[i] = sigmoid(wSum(l2Neurons, l2Weights[i], l2Size) + outputBias[i]);
    }

    // prints output (for debug)
    for (int i = 0; i < 10; i++){
        printf("%f\n", outputNeurons[i]);
    }

    return 0;
}