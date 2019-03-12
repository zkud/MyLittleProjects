#include "vectors.h"


char * allocateVector(long long vectorSize)
{
  char* result;

  // try max allocations times ot allocate memory for vector
  for (int attemptCount = 0; attemptCount < MAX_ALLOCATIONS; ++attemptCount)
  {
    result = (char*)malloc(sizeof(char) * vectorSize);

    if (result)
    {
      return result;
    }
  }

  return result;
}

bool allocateVectors(char ** vectors, long long vectorSize, short vectorsCount)
{
  // allocate vectorsCount vectors
  for (int count = 0; count < vectorsCount; count++)
  {
    vectors[count] = allocateVector(vectorSize);
    
    // handle allocation errors
    if (!vectors[count])
    {
      freeVectors(vectors, count);
      return false;
    }
  }

  return true;
}

void freeVectors(char ** vectors, short vectorsCount)
{
  short count = 0;

  while (count < vectorsCount)
  {
    free(vectors[count]);
    count++;
  }
}


void randomiseVector(char * vector, long long vectorSize)
{
  int * mask = (int *)vector;

  for (long long counter = 0; counter < vectorSize; counter += sizeof(int), mask++)
  {
    *mask = rand();
  }
}
