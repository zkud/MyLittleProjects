#include "functions.h"

void defaultOr(char * vector1, char * vector2, char * resultVector, long long length)
{
  for (long long count = 0; count < length; count++)
  {
    resultVector[count] = vector1[count] | vector2[count];
  } 
}

void asmOr(char * vector1, char * vector2, char * resultVector, long long length)
{
  long * mask1 = (long*) vector1;
  long * mask2 = (long*) vector2;
  long * mask3 = (long*) resultVector;

  for (long long count = 0; count < length; count+=4, mask1++, mask2++, mask3++)
  {
    __asm
    {
      mov eax, [mask1]
      or eax, [mask2]
      mov [mask3], eax
    }
  }
}

void mmxOr(char * vector1, char * vector2, char * resultVector, long long length)
{
 /* long long* mask1 = (long long*) vector1;
  long long* mask2 = (long long*) vector2;
  long long* mask3 = (long long*) resultVector;
*/

  for (long long count = 0; count < length; count += 32, vector1 += 32, vector2 += 32, resultVector += 32)
  {
    __asm
    {
    //  // load info to registers
    //  movq mm0, [mask1]
    //  movq mm1, [mask2]

    //  // make bitwise operations
    //  por mm0, mm1

    //  // save result
    //  movq [mask3], mm0
    //
      movq mm0, [vector1]
      movq mm1, [vector1 + 8]
      movq mm2, [vector1 + 16]
      movq mm3, [vector1 + 24]

      movq mm4, [vector2]
      movq mm5, [vector2 + 8]
      movq mm6, [vector2 + 16]
      movq mm7, [vector2 + 24]

      por mm0, mm4
      por mm1, mm5
      por mm2, mm6
      por mm3, mm7

      movq [resultVector], mm0
      movq [resultVector + 8], mm1
      movq [resultVector + 16], mm2
      movq [resultVector + 24], mm3
    }
  }

  // end multimedia state
  __asm
  {
    emms
  }
}
