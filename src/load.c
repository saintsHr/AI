#include <stdio.h>
#include <stdint.h>
#include "bmp.h"
#include "config.h"

int loadBMP(const char* filename, float inputNeurons[]){
    FILE* img = fopen(filename, "rb");
    if (!img) return 1;

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    fread(&fileHeader, sizeof(fileHeader), 1, img);
    fread(&infoHeader, sizeof(infoHeader), 1, img);

    if (fileHeader.bfType != 0x4D42){
        fclose(img);
        return 2;
    }

    if (infoHeader.biBitCount != 8){
        fclose(img);
        return 3;
    }

    if (infoHeader.biWidth != imgW || infoHeader.biHeight != imgH){
        fclose(img);
        return 4;
    }

    fseek(img, fileHeader.bfOffBits, SEEK_SET);

    uint8_t buffer[imageSize];
    for (int row = imgH - 1; row >= 0; row--)
        fread(buffer + (row * imgW), 1, imgW, img);

    fclose(img);

    for (int i = 0; i < imageSize; i++)
        inputNeurons[i] = buffer[i] / 255.0f;

    return 0;
}

void saveNetwork(const char* filename,
                 float inputWeights[l1Size][imageSize],
                 float l1Weights[l2Size][l1Size],
                 float l2Weights[10][l2Size],
                 float l1Bias[l1Size],
                 float l2Bias[l2Size],
                 float outputBias[10])
{
    FILE* f = fopen(filename, "wb");
    if (!f) return;

    fwrite(inputWeights, sizeof(float), l1Size * imageSize, f);
    fwrite(l1Weights, sizeof(float), l2Size * l1Size, f);
    fwrite(l2Weights, sizeof(float), 10 * l2Size, f);

    fwrite(l1Bias, sizeof(float), l1Size, f);
    fwrite(l2Bias, sizeof(float), l2Size, f);
    fwrite(outputBias, sizeof(float), 10, f);

    fclose(f);
}

int loadNetwork(const char* filename,
                float inputWeights[l1Size][imageSize],
                float l1Weights[l2Size][l1Size],
                float l2Weights[10][l2Size],
                float l1Bias[l1Size],
                float l2Bias[l2Size],
                float outputBias[10])
{
    FILE* f = fopen(filename, "rb");
    if (!f) return 1;

    fread(inputWeights, sizeof(float), l1Size * imageSize, f);
    fread(l1Weights, sizeof(float), l2Size * l1Size, f);
    fread(l2Weights, sizeof(float), 10 * l2Size, f);

    fread(l1Bias, sizeof(float), l1Size, f);
    fread(l2Bias, sizeof(float), l2Size, f);
    fread(outputBias, sizeof(float), 10, f);

    fclose(f);
    return 0;
}
