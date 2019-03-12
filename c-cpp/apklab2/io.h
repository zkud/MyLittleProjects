#pragma once
#include <stdio.h>

// creates file with filename and saves vector to it
void saveVectorToFile(char* filename, char* vector, long long vectorSize);

void printBenchmarkHeader();
void printBenchmarkLine(char* variantName, double time, char* resultFileName);
