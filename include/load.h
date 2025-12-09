#pragma once

int loadBMP(const char* filename, float inputNeurons[]);
int loadNetwork(const char* filename,
                float inputWeights[l1Size][imageSize],
                float l1Weights[l2Size][l1Size],
                float l2Weights[10][l2Size],
                float l1Bias[l1Size],
                float l2Bias[l2Size],
                float outputBias[10]);
void saveNetwork(const char* filename,
                 float inputWeights[l1Size][imageSize],
                 float l1Weights[l2Size][l1Size],
                 float l2Weights[10][l2Size],
                 float l1Bias[l1Size],
                 float l2Bias[l2Size],
                 float outputBias[10]);