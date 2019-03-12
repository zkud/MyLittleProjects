// the same as calc.py, but asm variant
#include <time.h>

#include "vectors.h"
#include "functions.h"
#include "io.h"


// calculate time in seconds from start to end
double calculate_elapsed_time(const clock_t start,
  const clock_t end)
{
  return (double) (end - start) / CLK_TCK;
}

// tests function and return elapsed time in seconds
double test(void (*function)(char*, char*, char*, long long),
            char* vector1, char* vector2, char* vector3,
            long long vectorSize)
{
  // variables for timing
  clock_t start;
  clock_t end;
  
  // test function
  start = clock();
  function(vector1, vector2, vector3, vectorSize);
  end = clock();

  return calculate_elapsed_time(start, end);
}

int main(void)
{
  #define VECTOR_LENGTH 10000000
  #define TESTS_COUNT 3
  char * outputFileNames[TESTS_COUNT] = {"c.txt", "asm.txt", "mmx.txt"};
  char * variantName[TESTS_COUNT] = { "usial C", "asm", "mmx" };
 
  void(*functions[TESTS_COUNT]) (char*, char*, char*, long long) = {
    defaultOr, asmOr, mmxOr
  };

  // allocate and init vectors
  char* vectors[3];
  if (!allocateVectors(vectors, VECTOR_LENGTH, 3))
  {
    puts("error:can't allocate vectors");
    return;
  };

  randomiseVector(vectors[0], VECTOR_LENGTH);
  randomiseVector(vectors[1], VECTOR_LENGTH);

  saveVectorToFile("input1.txt", vectors[0], VECTOR_LENGTH);
  saveVectorToFile("input2.txt", vectors[1], VECTOR_LENGTH);

  // test functions and show results
  printBenchmarkHeader();
  for (int testCount = 0; testCount < TESTS_COUNT; testCount++)
  {
    double time = test(functions[testCount], vectors[0], vectors[1], vectors[2], VECTOR_LENGTH);
    saveVectorToFile(outputFileNames[testCount], vectors[2], VECTOR_LENGTH);

    printBenchmarkLine(variantName[testCount], time, outputFileNames[testCount]);
  }
    
  freeVectors(vectors, 3);
  getc(stdin);
  return 0;
}