#pragma once

#include <stdlib.h>
#include <stdbool.h>

#define MAX_ALLOCATIONS 10

// allocates vector with size vectorSize
// returns null if can't allocate
char* allocateVector(long long vectorSize);

// allocates vectorCount vectors with size vectorSize
bool allocateVectors(char** vectors, long long vectorSize, short vectorsCount);

// frees vectorCount vectors
void freeVectors(char** vectors, short vectorsCount);

// make random content in vector
void randomiseVector(char* vector, long long vectorSize);
