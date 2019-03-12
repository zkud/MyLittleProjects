#pragma once

// result vector cell[i] = vector1[i] | vector2[i]
void defaultOr(char* vector1, char* vector2, char* resultVector, long long length);

// the same as defaultOr
void asmOr(char* vector1, char* vector2, char* resultVector, long long length);

// the same as defaultOr
void mmxOr(char* vector1, char* vector2, char* resultVector, long long length);
