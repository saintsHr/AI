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

    return 0;
}