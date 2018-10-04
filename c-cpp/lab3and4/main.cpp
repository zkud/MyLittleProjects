#include <iostream>
#include <string>
#include <vector>
#include "classes/complex3darray.hpp"
#include "classes/complex.hpp"
using std::cerr;
using std::cin;
using std::cout;
using std::endl;


void stupidLog(const std::string &msg, const bool &isError = false)
{
  if (isError)
  {
    cerr << std::string("main:log:Error:") + msg << endl;
  }
  else
  {
    cout << "main:log:" + msg << endl;
  }
}

namespace transit
{
  lab34::Complex3dArray transit1(lab34::Complex3dArray vector)
  {
    return vector;
  }

  const lab34::Complex3dArray &transit2(const lab34::Complex3dArray &vector)
  {
    return vector;
  }

  const lab34::Complex3dArray *transit3(lab34::Complex3dArray *vector)
  {
    return vector;
  }
} // namespace transit

void part0() {
  using lab34::Complex3dArray;
  using lab34::Shape;
  using lab34::Complex;
  using lab34::operator<<;
  using lab34::operator>>;
  using lab34::operator+;
  using lab34::operator-;
  using lab34::operator==;
  using lab34::operator!=;

  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~part 0~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  // create
  Complex3dArray array;
  
  // show
  cout << "before init:" << endl << array << endl;

  // init
  array(std::vector<Complex>(27, Complex(1, 1)));

  // show
  cout << "after init:" << endl << array << endl;

  // update some elements and show
  array(-1, -1, -1) = Complex(-1, -1);
  array[0][1][0] += array[0][0][0];
  cout << "after using indexing + updation:" << endl << array << endl;

  // increment / decrement the all elements
  array++;
  cout << "post-increment:" << endl << array << endl;

  ++array;
  cout << "pre-increment:" << endl << array << endl;

  array--;
  cout << "post-decrement:" << endl << array << endl;

  --array;
  cout << "pre-decrement:" << endl << array << endl;

  // init other:
  Complex3dArray array2(array);
  array2++;

  // sum
  cout << "sum (sub) left arg:" << endl << array << endl;
  cout << "sum (sub) right arg:" << endl << array2 << endl;
  cout << "sum:" << endl << array + array2 << endl;
  cout << "sub:" << endl << array - array2 << endl;

  // new / delete
  Complex3dArray * array3 = new Complex3dArray;
  cout << "allocated Complex3dArray:" << endl << *array3 << endl;
  delete array3;

  array3 = new Complex3dArray[2];
  cout << "allocated array of Complex3dArray:" << endl << array3[0]
                                               << array3[1] << endl;
  delete[] array3;

  // global operators
  lab34::Data array4(
    3,
    std::vector<std::vector<Complex>>(
      3,
      std::vector<Complex>(
        3,
        Complex(228, 1337)
      )
    )
  );

  cout << "global sum:" << endl << array + array4 << endl;
  cout << "global sub:" << endl << array - array4 << endl;
  cout << "reversed global sum:" << endl << array4 + array << endl;
  cout << "reversed global sub:" << endl << array4 - array << endl;

  // comparison
  cout << "judjemt array equals to array4 is " <<
    (array == array4 ? "true" : "false") << endl;
  cout << "judjemt array4 equals to array is " <<
    (array4 == array ? "true" : "false") << endl;
  cout << "judjemt array is not equal to array4 is " <<
    (array != array4 ? "true" : "false") << endl;
  cout << "judjemt array4 is not equal to array is " <<
    (array4 != array ? "true" : "false") << endl;


  // casts
  cout << "array:" << endl << array << endl;
  Shape shape = (Shape)array;
  cout << "shape of array:" << endl << shape.toString() << endl;
  cout << "std deviation:" << endl << (double)array << endl;

  // istream
  cout << "array for input content:" << endl << array << endl;
  cin >> array;
  cout << "now content is:" << endl << array << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~part 0~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

void part1()
{
  using lab34::Complex3dArray;
  using lab34::Complex;
  using lab34::Shape;

  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~part 1~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  // create array and its copy
  Complex3dArray array1(
    Shape(std::vector<std::size_t>(3, 3)),
    std::vector<Complex>(27, Complex(1, 1))
  );

  Complex3dArray array2(array1);
  
  // show contents of array1 and array2
  cout << "array1 content:" + array1.toString() << endl;
  cout << "array2 content:" + array2.toString() << endl;

  // show state before playing
  cout << "array1 before tuned:" + array1.toString() << endl;

  // play a bit with content of array1
  for(std::size_t count = 0; count < 3; count++)
  {
    array1(count, count, count) *= Complex(30, 30);
  }
  array1(-2, -2, -2) *= array1(-2, -2, -2);

  // show state after playing
  cout << "array1 after tuned:" + array1.toString() << endl;

  // sort&show state
  array1.sort();
  cout << "array1 after sorted:" + array1.toString() << endl;

  // check comparation
  cout << "array1" << array1.toString() << endl;
  cout << "array2" << array2.toString() << endl;
  cout << "judjemt array1 > array2 is " << (array1 > array2 ? "true" : "false") << endl;
  cout << "judjemt array1 equals to array2 is " <<
          (array1 == array2 ? "true" : "false") << endl;
  cout << "judjemt array1 < array2 is " << (array1 < array2 ? "true" : "false") << endl;

  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~part 1~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

void part2()
{
  using lab34::Complex3dArray;
  using lab34::Complex;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~part 2~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

  Complex3dArray array;

  cout << "start:" + array.toString() << endl;

  cout << "1:" + array.toString() + transit::transit1(array).toString() << endl;
  cout << "2:" + array.toString() + transit::transit2(array).toString() << endl;
  cout << "3:" + array.toString() + transit::transit3(&array)->toString() << endl;

  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~part 2~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

int main()
{
  part0();
  part1();
  part2();

  cout << "enter any char to end" << endl;
  char endChar;
  cin >> endChar;
  return 0;
}