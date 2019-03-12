#include "io.h"

void saveVectorToFile(char * filename, char * vector, long long vectorSize)
{
  // init file
  FILE* file;
  fopen_s(&file, filename, "wb");

  if (!file)
  {
    printf("error: can't create file with name \"%s\" to save result\n", filename);
    return;
  }

  // save vector
  fwrite(vector, sizeof(char), vectorSize, file);

  // close file
  fclose(file);
}

void printBenchmarkHeader()
{
  puts("variant name | time in seconds | output file with results");
}

void printBenchmarkLine(char * variantName, double time, char * resultFileName)
{
  printf("%s\t\t%lf\t\t%s\n", variantName, time, resultFileName);
}
