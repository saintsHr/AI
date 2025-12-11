#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"
#include "config.h"
#include "bmp.h"
#include "load.h"

void forwardPass(float inputNeurons[], 
                 float l1Neurons[],
                 float l2Neurons[],
                 float outputNeurons[],
                 float inputWeights[][imageSize], 
                 float l1Weights[][l1Size],
                 float l2Weights[][l2Size],
                 float l1Bias[],
                 float l2Bias[],
                 float outputBias[]) 
{
    for (int i = 0; i < l1Size; i++)
        l1Neurons[i] = sigmoid(wSum(inputNeurons, inputWeights[i], imageSize) + l1Bias[i]);

    for (int i = 0; i < l2Size; i++)
        l2Neurons[i] = sigmoid(wSum(l1Neurons, l1Weights[i], l1Size) + l2Bias[i]);

    for (int i = 0; i < 10; i++)
        outputNeurons[i] = sigmoid(wSum(l2Neurons, l2Weights[i], l2Size) + outputBias[i]);
}

void initNetwork(float inputWeights[l1Size][imageSize],
                 float l1Weights[l2Size][l1Size],
                 float l2Weights[10][l2Size],
                 float l1Bias[l1Size],
                 float l2Bias[l2Size],
                 float outputBias[10])
{
    srand((unsigned int)time(NULL));

    // input -> l1
    for (int i = 0; i < l1Size; i++) {
        l1Bias[i] = 0.0f;
        for (int j = 0; j < imageSize; j++)
            inputWeights[i][j] = ((float)rand() / RAND_MAX - 0.5f) * 0.1f;
    }

    // l1 -> l2
    for (int i = 0; i < l2Size; i++) {
        l2Bias[i] = 0.0f;
        for (int j = 0; j < l1Size; j++)
            l1Weights[i][j] = ((float)rand() / RAND_MAX - 0.5f) * 0.1f;
    }

    // l2 -> output
    for (int i = 0; i < 10; i++) {
        outputBias[i] = 0.0f;
        for (int j = 0; j < l2Size; j++)
            l2Weights[i][j] = ((float)rand() / RAND_MAX - 0.5f) * 0.1f;
    }
}

int getPredicted(float out[]){
    int p = 0;
    for(int i = 1; i < 10; i++){
        if(out[i] > out[p]) p = i;
    }
    return p;
}

float cost(float out, float wanted){
    return pow((out - wanted), 2);
}

void backprop(
    float inputNeurons[],
    float l1Neurons[],
    float l2Neurons[],
    float outputNeurons[],
    unsigned char label,

    float inputWeights[l1Size][imageSize],
    float l1Weights[l2Size][l1Size],
    float l2Weights[10][l2Size],

    float l1Bias[l1Size],
    float l2Bias[l2Size],
    float outputBias[10],

    float lr
)
{
    float target[10] = {0};
    target[label] = 1.0f;

    float deltaOut[10];
    float deltaL2[l2Size];
    float deltaL1[l1Size];

    for (int k = 0; k < 10; k++) {
        float error = outputNeurons[k] - target[k];
        deltaOut[k] = error * (outputNeurons[k] * (1 - outputNeurons[k]));
    }

    for (int j = 0; j < l2Size; j++) {
        float sum = 0.0f;
        for (int k = 0; k < 10; k++)
            sum += deltaOut[k] * l2Weights[k][j];

        deltaL2[j] = sum * (l2Neurons[j] * (1 - l2Neurons[j]));
    }

    for (int j = 0; j < l1Size; j++) {
        float sum = 0.0f;
        for (int k = 0; k < l2Size; k++)
            sum += deltaL2[k] * l1Weights[k][j];

        deltaL1[j] = sum * (l1Neurons[j] * (1 - l1Neurons[j]));
    }

    for (int k = 0; k < 10; k++) {
        for (int j = 0; j < l2Size; j++) {
            l2Weights[k][j] -= lr * deltaOut[k] * l2Neurons[j];
        }
        outputBias[k] -= lr * deltaOut[k];
    }

    for (int j = 0; j < l2Size; j++) {
        for (int i = 0; i < l1Size; i++) {
            l1Weights[j][i] -= lr * deltaL2[j] * l1Neurons[i];
        }
        l2Bias[j] -= lr * deltaL2[j];
    }

    for (int j = 0; j < l1Size; j++) {
        for (int i = 0; i < imageSize; i++) {
            inputWeights[j][i] -= lr * deltaL1[j] * inputNeurons[i];
        }
        l1Bias[j] -= lr * deltaL1[j];
    }
}

int main(int argc, char* argv[]){

// =======================================================================================//

    // checks correct usage
    if (argc < 2 || argc > 3){
        printf(
            "============================================================================\n"
            "Incorrect Usage\n"
            "Usage: ./ai <train/test/file/init> [image]\n"
            "============================================================================\n"
            "<train> -> trains the AI\n"
            "<test> -> test the AI's accuracy\n"
            "<file> -> feeds the AI a .bmp image\n"
            "<init> -> inits the network with random numbers\n"
            "[image] -> (optional) image to feed the AI, only used when arg 1 is 'file'\n"
            "============================================================================\n"
        );
        return 1;
    }

// =======================================================================================//

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

// =======================================================================================//

    if (strcmp(argv[1], "train") == 0 && argc == 2){

        float* images;
        uint8_t* labels;
        int count;

        loadNetwork("data/network.bin",
                    inputWeights,
                    l1Weights,
                    l2Weights,
                    l1Bias,
                    l2Bias,
                    outputBias
        );

        if (loadMNIST("data/train/train-images-idx3-ubyte",
                       "data/train/train-labels-idx1-ubyte",
                       &images,
                       &labels,
                       &count) != 0)
        return 2;

        int correct = 0;
        
        printf("training...\n");
        for (int i = 0; i < count; i++){
            memcpy(inputNeurons, &images[i * imageSize], imageSize * sizeof(float));

            forwardPass(
                inputNeurons,
                l1Neurons,
                l2Neurons,
                outputNeurons,

                inputWeights,
                l1Weights,
                l2Weights,

                l1Bias,
                l2Bias,
                outputBias
            );

            backprop(
                inputNeurons,
                l1Neurons,
                l2Neurons,
                outputNeurons,
                labels[i],
                inputWeights,
                l1Weights,
                l2Weights,
                l1Bias,
                l2Bias,
                outputBias,
                0.01f
            );

            int pred = getPredicted(outputNeurons);
            unsigned char real = labels[i];
            if (pred == real) correct++;
        }

        printf("\nAccuracy: %.2f%%\n", (correct * 100.0f) / count);

        saveNetwork("data/network.bin",
                    inputWeights,
                    l1Weights,
                    l2Weights,
                    l1Bias,
                    l2Bias,
                    outputBias
        );

    } else 
    if (strcmp(argv[1], "test") == 0 && argc == 2){
        
        float* images;
        uint8_t* labels;
        int count;

        loadNetwork("data/network.bin",
                    inputWeights,
                    l1Weights,
                    l2Weights,
                    l1Bias,
                    l2Bias,
                    outputBias
        );

        if (loadMNIST("data/test/t10k-images-idx3-ubyte",
                       "data/test/t10k-labels-idx1-ubyte",
                       &images,
                       &labels,
                       &count) != 0)
        return 2;

        int correct = 0;
        
        printf("testing...\n");
        for (int i = 0; i < count; i++){
            memcpy(inputNeurons, &images[i * imageSize], imageSize * sizeof(float));

            forwardPass(
                inputNeurons,
                l1Neurons,
                l2Neurons,
                outputNeurons,

                inputWeights,
                l1Weights,
                l2Weights,

                l1Bias,
                l2Bias,
                outputBias
            );

            int pred = getPredicted(outputNeurons);
            unsigned char real = labels[i];
            if (pred == real) correct++;
        }

        printf("\nAccuracy: %.2f%%\n", (correct * 100.0f) / count);

    } else
    if (strcmp(argv[1], "init") == 0 && argc == 2){
        initNetwork(inputWeights,
                    l1Weights,
                    l2Weights,
                    l1Bias,
                    l2Bias,
                    outputBias
        );
        saveNetwork("data/network.bin",
                    inputWeights,
                    l1Weights,
                    l2Weights,
                    l1Bias,
                    l2Bias,
                    outputBias
        );
    } else 
    if (strcmp(argv[1], "file") == 0 && argc == 3){

        loadNetwork("data/network.bin",
                    inputWeights,
                    l1Weights,
                    l2Weights,
                    l1Bias,
                    l2Bias,
                    outputBias
        );

        if (loadBMP(argv[2], inputNeurons) != 0) return 3;

        forwardPass(
            inputNeurons,
            l1Neurons,
            l2Neurons,
            outputNeurons,

            inputWeights,
            l1Weights,
            l2Weights,

            l1Bias,
            l2Bias,
            outputBias
        );

        // prints outputs
        printf("\n");
        for (int i = 0; i < 10; i++)
            printf("%d -> %.2f%%\n", i, outputNeurons[i] * 100);

        // prints predicted number
        int predicted = getPredicted(outputNeurons);
        printf("\nPredicted: %d\n\n", predicted);
        
    } else {
        printf(
            "============================================================================\n"
            "Incorrect Usage\n"
            "Usage: ./ai <train/test/file/init> [image]\n"
            "============================================================================\n"
            "<train> -> trains the AI\n"
            "<test> -> test the AI's accuracy\n"
            "<file> -> feeds the AI a .bmp image\n"
            "<init> -> inits the network with random numbers\n"
            "[image] -> (optional) image to feed the AI, only used when arg 1 is 'file'\n"
            "============================================================================\n"
        );
        return 4;
    }

// =======================================================================================//

    return 0;
}