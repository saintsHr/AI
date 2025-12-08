#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "config.h"
#include "bmp.h"

int main(int argc, char* argv[]){

// =======================================================================================//

    // checks correct usage
    if (argc < 2 || argc > 3) return 1;

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

    if (strcmp(argv[1], "train")){

    }
    if (strcmp(argv[1], "test")){
        
    }
    if (strcmp(argv[1], "file")){
        
    }

// =======================================================================================//

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

// =======================================================================================//

    return 0;
}