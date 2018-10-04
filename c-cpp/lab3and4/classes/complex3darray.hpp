#ifndef __VECTOR3D__HPP__
#define __VECTOR3D__HPP__
#include "shape.hpp"
#include "complex.hpp"
#include <vector>
#include "exceptions/toosmallvalueexception.hpp"
#include <list>
#include <iostream>

namespace lab34
{
  using Data = std::vector<std::vector<std::vector<Complex>>>;

  class Complex3dArray;

  Complex3dArray & operator -- (Complex3dArray & array);
  Complex3dArray & operator -- (Complex3dArray & array, int i);
  Complex3dArray operator + (const Complex3dArray & complexArray, const Data & vector);
  Complex3dArray operator - (const Complex3dArray & complexArray, const Data & vector);
  Complex3dArray operator + (const Data & vector, const Complex3dArray & complexArray);
  Complex3dArray operator - (const Data & vector, const Complex3dArray & complexArray);
  bool operator == (const Complex3dArray & complexArray, const Data & vector);
  bool operator == (const Data & vector, const Complex3dArray & complexArray);
  bool operator != (const Complex3dArray & complexArray, const Data & vector);
  bool operator != (const Data & vector, const Complex3dArray & complexArray);

  std::ostream & operator << (std::ostream & stream, const Complex3dArray & array);
  std::istream & operator >> (std::istream & stream, Complex3dArray & array);

  // my little 'containtainer'
  class Complex3dArray
  {
    //// ----------------------------------------------------------------------- friends operators
    //// ariphmetical
    friend Complex3dArray & operator -- (Complex3dArray & array);
    friend Complex3dArray & operator -- (Complex3dArray & array, int i);
    friend Complex3dArray operator + (const Complex3dArray & complexArray, const Data & vector);
    friend Complex3dArray operator - (const Complex3dArray & complexArray, const Data & vector);
    friend Complex3dArray operator + (const Data & vector, const Complex3dArray & complexArray);
    friend Complex3dArray operator - (const Data & vector, const Complex3dArray & complexArray);

    //// comparasion
    friend bool operator == (const Complex3dArray & complexArray, const Data & vector);
    friend bool operator == (const Data & vector, const Complex3dArray & complexArray);
    friend bool operator != (const Complex3dArray & complexArray, const Data & vector);
    friend bool operator != (const Data & vector, const Complex3dArray & complexArray);

    //// stream i/o
    friend std::ostream & operator << (const std::ostream & stream, const Complex3dArray & array);
    friend std::istream & operator >> (std::istream & stream, Complex3dArray & array);
  public:
    // constructors/destr
    // std
    Complex3dArray();
    // initialize
    Complex3dArray(const Shape &shape, const std::vector<Complex> &data);
    // initialize 2
    Complex3dArray(const Shape &shape);
    // copy
    Complex3dArray(const Complex3dArray &value);
    ~Complex3dArray();

    //-------------------------------------------------------------------------------- operators
    // reinitialization
    const Complex3dArray& operator = (const Complex3dArray &value);
    void operator () (const std::vector<Complex> &data);
    // !!ATTENTION!!: method can't check end - start value (due to InputIterator interface)!!!
    // It means, that you can init only a part of array in some cases
    template <class InputIterator>
    void operator () (InputIterator start, InputIterator end);


    // indexing
    // feature indexing
    // returns reference on vector3d[row][column][deep]
    // supports Python-like indexes, for ex. end is (-1, -1, -1)
    Complex & operator () (
      const int &row,
      const int &column,
      const int &deep
      ) const;
    // simple indexing without features
    std::vector<std::vector<Complex>> & operator [] (int i) const;

    // comparasion
    bool operator == (const Complex3dArray &value) const;
    bool operator != (const Complex3dArray &value) const;
    bool operator > (const Complex3dArray &value) const;
    bool operator < (const Complex3dArray &value) const;
    bool operator >= (const Complex3dArray &value) const;
    bool operator <= (const Complex3dArray &value) const;

    //// ariphmetic
    //// unary
    Complex3dArray & operator ++ ();
    Complex3dArray & operator ++ (int i);
    //// binary
    Complex3dArray operator + (const Complex3dArray &value);
    Complex3dArray operator - (const Complex3dArray &value);

    //// allocation
    void * operator new (std::size_t size);
    void * operator new[](std::size_t size);
    void operator delete(void * pointer);
    void operator delete[](void * pointer);

    //// cast
    // returns standart deviation
    operator double();
    operator Shape();
    //-------------------------------------------------------------------------------- operators
    // support methods
    std::string toString() const;
    void sort();

    // map trought complex3darray and container
    // WARNING: DOESN'T CHECK SIZE OF  A CONTAINER!!!
    template <class Functor, class Container>
    static void twistedMap(
      const Complex3dArray &value1,
      const Container &value2,
      Functor &functor
    );

    // the same as twistedMap, but stops when functor returns true
    template <class Functor, class Container>
    static void twistedBoolMap(
      const Complex3dArray &value1,
      const Container &value2,
      Functor &functor
    );

    // creates new 3darray
    // every new 3darray element is a result of function(left, right)
    // left and right are on the same positions
    // WARNING: DOESN'T CHECK SIZE OF  A CONTAINER!!!
    template <class Function, class Container>
    static Complex3dArray twistedBinaryOperation
    (const Complex3dArray &left, const Container &right, Function function);

    // std getters/setters
    static const Shape &getStandartShape();
    static void setStandartShape(const Shape &shape);

    // getters/setters
    const Shape &getShape() const;
  private:
    // support methods
    double meanMedian();

    template <class Function>
    Complex3dArray binaryOperation(const Complex3dArray &value, Function function);

    // allocates data with such shape
    static Data * allocData(const Shape &shape);
    static void * alloc(std::size_t size);

    // beta log
    static void stupidLog(std::string &msg, const bool &error = false);
    static void stupidLog(const char * msg, const bool &error = false);

    // iterate through 2 vectors content and use functor(x, y)
    // (uses first shape)
    template <class Functor>
    static void myZipMap(
      const Complex3dArray &value1,
      const Complex3dArray &value2,
      Functor &functor
    );

    // the same as myZipMap, but if functor returns true, it will stop
    template <class Functor>
    static void myBoolZipMap(
      const Complex3dArray &value1,
      const Complex3dArray &value2,
      Functor &functor
    );

    // iterate through vectors content and use functor(x) on every element
    template <class Functor>
    void myMap(Functor &functor);

    // the same as myMap, 1but if functor returns true, it will stop
    template <class Functor>
    void myBoolMap(Functor &functor);

    // gets strings repr. of content[0 - row][0 - column][deep]
    const std::string & deepToString(const int &deep, std::string &output) const;

    // gets strings repr. of content[row][0 - column][deep]
    const std::string & rowToString(const int &row, const int &deep, std::string &output) const;

    // support methods for Python-like indexes
    static std::size_t makeRealIndex(int index, std::size_t size);
    static bool isBadIndex(int index, std::size_t size);

    // std shape of vector using in std constructor
    static Shape standartShape;

    // vector X vector X vector for content
    Data *content;

    // shape of matrix
    Shape shape;
  };

  template <class Function>
  Complex3dArray Complex3dArray::binaryOperation(const Complex3dArray &value, Function function)
  {
    // check for unequal shapes
    if (this->shape != value.shape)
    {
      throw lab3Exceptions::UnsummableShapesException(
        "shapes" + this->shape.toString() + " and " + value.shape.toString() +
        "aren't equal"
      );
    }
    
    // alloc vector with results
    std::vector<Complex> values(value.shape[0] * value.shape[1] * value.shape[2]);

    // init it
    auto initLambda = [iterator = values.begin(), &function]
    (const Complex &left, const Complex &right) mutable
    {
      *iterator++ = function(left, right);
    };
    myZipMap(*this, value, initLambda);

    // alloc result (copy vector) and return it
    return Complex3dArray(value.shape, values);
  }
  
  template <class InputIterator>
  void Complex3dArray::operator() (InputIterator start, InputIterator end)
  {
    auto initLambda = [&](Complex & reference) mutable
    {
      // init element
      reference = *start++;

      // check if it's end
      if (start == end)
      {
        return true;
      }
      else
      {
        return false;
      }
    };

    myBoolMap(initLambda);
  }

  template <class Functor>
  void Complex3dArray::myMap(Functor &functor)
  {
    for (std::size_t deep = 0; deep < shape[2]; ++deep)
    {
      for (std::size_t row = 0; row < shape[0]; ++row)
      {
        for (std::size_t column = 0; column < shape[1]; ++column)
        {
          functor(this->operator()(row, column, deep));
        }
      }
    }
  }

  template <class Functor>
  void Complex3dArray::myBoolMap(Functor &functor)
  {
    for (std::size_t deep = 0; deep < shape[2]; ++deep)
    {
      for (std::size_t row = 0; row < shape[0]; ++row)
      {
        for (std::size_t column = 0; column < shape[1]; ++column)
        {
          if (functor(this->operator()(row, column, deep)))
            return;
        }
      }
    }
  }

  template <class Functor>
  void Complex3dArray::myZipMap(
    const Complex3dArray &value1,
    const Complex3dArray &value2,
    Functor &functor)
  {
    for (std::size_t deep = 0; deep < value1.getShape()[2]; ++deep)
    {
      for (std::size_t row = 0; row < value1.getShape()[0]; ++row)
      {
        for (std::size_t column = 0; column < value1.getShape()[1]; ++column)
        {
          functor(value1(row, column, deep), value2(row, column, deep));
        }
      }
    }
  }

  template <class Functor>
  void Complex3dArray::myBoolZipMap(
    const Complex3dArray &value1,
    const Complex3dArray &value2,
    Functor &functor)
  {
    for (std::size_t deep = 0; deep < value1.getShape()[2]; ++deep)
    {
      for (std::size_t row = 0; row < value1.getShape()[0]; ++row)
      {
        for (std::size_t column = 0; column < value1.getShape()[1]; ++column)
        {
          if (functor(value1(row, column, deep), value2(row, column, deep)))
            return;
        }
      }
    }
  }

  template <class Functor, class Container>
  void Complex3dArray::twistedMap(
    const Complex3dArray &value1,
    const Container &value2,
    Functor &functor
  )
  {
    for (std::size_t deep = 0; deep < value1.getShape()[2]; ++deep)
    {
      for (std::size_t row = 0; row < value1.getShape()[0]; ++row)
      {
        for (std::size_t column = 0; column < value1.getShape()[1]; ++column)
        {
          functor(value1(row, column, deep), value2[row][column][deep]);
        }
      }
    }
  }

  template <class Functor, class Container>
  void Complex3dArray::twistedBoolMap(
    const Complex3dArray &value1,
    const Container &value2,
    Functor &functor
  )
  {
    for (std::size_t deep = 0; deep < value1.getShape()[2]; ++deep)
    {
      for (std::size_t row = 0; row < value1.getShape()[0]; ++row)
      {
        for (std::size_t column = 0; column < value1.getShape()[1]; ++column)
        {
          if(functor(value1(row, column, deep), value2[row][column][deep]))
            return;
        }
      }
    }
  }

  template <class Function, class Container>
  Complex3dArray Complex3dArray::twistedBinaryOperation
  (const Complex3dArray &left, const Container &right, Function function)
  {
    // alloc vector with results
    std::vector<Complex> values(left.shape[0] * left.shape[1] * left.shape[2]);

    // init it
    auto initLambda = [iterator = values.begin(), &function]
    (const Complex &left, const Complex &right) mutable
    {
      *iterator++ = function(left, right);
    };
    twistedMap<decltype(initLambda), Container>(left, right, initLambda);

    // alloc result (copy vector) and return it
    return Complex3dArray(left.shape, values);
  }
} // namespace lab34

#endif