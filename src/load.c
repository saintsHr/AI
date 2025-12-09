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