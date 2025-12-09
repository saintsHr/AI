#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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

static uint32_t read_be_uint32(FILE* f){
    uint8_t b[4];
    fread(b, 1, 4, f);
    return (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
}

int loadMNIST(
    const char* imageFile,
    const char* labelFile,
    float** images_out,
    uint8_t** labels_out,
    int* count_out)
{
    // label reading
    FILE* fl = fopen(labelFile, "rb");
    if (!fl) return 1;

    uint32_t magic_lbl = read_be_uint32(fl);
    if (magic_lbl != 2049) { fclose(fl); return 2; }

    uint32_t num_labels = read_be_uint32(fl);

    uint8_t* labels = malloc(num_labels);
    if (!labels) { fclose(fl); return 3; }

    fread(labels, 1, num_labels, fl);
    fclose(fl);

    // image reading
    FILE* fi = fopen(imageFile, "rb");
    if (!fi) return 4;

    uint32_t magic_img = read_be_uint32(fi);
    if (magic_img != 2051) { fclose(fi); return 5; }

    uint32_t num_images = read_be_uint32(fi);
    uint32_t rows       = read_be_uint32(fi);
    uint32_t cols       = read_be_uint32(fi);

    int img_size = rows * cols;

    if (num_images != num_labels) {
        fclose(fi);
        free(labels);
        return 6;
    }

    float* images = malloc(sizeof(float) * num_images * img_size);
    if (!images) {
        fclose(fi);
        free(labels);
        return 7;
    }

    for (uint32_t i = 0; i < num_images; i++) {
        for (int p = 0; p < img_size; p++) {
            unsigned char px;
            fread(&px, 1, 1, fi);
            images[i * img_size + p] = px / 255.0f;
        }
    }

    fclose(fi);

    *images_out = images;
    *labels_out = labels;
    *count_out = num_images;
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
