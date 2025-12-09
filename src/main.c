#include <stdio.h>
#include <string.h>

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

int getPredicted(float outputNeurons[]){
    int predicted = -1;
    float maxVal = outputNeurons[0];
    int countMax = 1;

    for (int i = 1; i < 10; i++){
        if (outputNeurons[i] > maxVal){
            maxVal = outputNeurons[i];
            predicted = i;
            countMax = 1;
        } else if (outputNeurons[i] == maxVal){
            countMax++;
        }
    }

    if (countMax == 1) {
        return predicted;
    } else {
        return -1;
    }
}

int main(int argc, char* argv[]){

// =======================================================================================//

    // checks correct usage
    if (argc < 2 || argc > 3) {
        printf(
            "======================================\n"
            "Incorrect Usage\n"
            "Usage: ./ai <train/test/file> [image]\n"
            "======================================\n"
            "<train> -> trains the AI\n"
            "<test> -> test the AI's accuracy\n"
            "<file> -> feeds the AI a .bmp image\n"
            "[image] -> (optional) image to feed the AI, only used when arg 1 is 'file'\n"
            "======================================\n"
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

    } else 
    if (strcmp(argv[1], "test") == 0 && argc == 2){
        
    } else
    if (strcmp(argv[1], "file") == 0 && argc == 3){

        if (loadBMP(argv[2], inputNeurons) != 0) return 2;

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

        // prints output (for debug)
        printf("\n");
        for (int i = 0; i < 10; i++)
            printf("%f\n", outputNeurons[i]);

        // prints predicted number
        int predicted = getPredicted(outputNeurons);
        if (predicted < 0){
            printf("\nPredicted: None\n\n");
        } else {
            printf("\nPredicted: %d\n\n", predicted);
        }
        
    } else {
        printf(
            "================================================================\n"
            "Incorrect Usage\n"
            "  Usage: ./ai <train/test/file> [image]\n"
            "================================================================\n"
            "<train>  ->  trains the AI\n"
            "<test>   ->  test the AI's accuracy\n"
            "<file>   ->  feeds the AI a .bmp image\n"
            "[image]  ->  image to feed the AI, needed when arg 1 is 'file'\n"
            "================================================================\n"
        );
        return 2;
    }

// =======================================================================================//

    return 0;
}