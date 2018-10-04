#include "complex3darray.hpp"
#include <set>
#include <cmath>
#include "complex.hpp"
#include <string>
#include "exceptions/toolargevalueexception.hpp"
#include "exceptions/uncomparableshapesexception.hpp"
#include "exceptions/unsummableshapesexception.hpp"
#include "exceptions\badvalueexception.hpp"
#include <stdexcept>
#include <list>


namespace lab34
{
  // ------------------------------------------------------------------------------------- friends
  std::ostream & operator << (std::ostream & stream, const Complex3dArray & array)
  {
    stream << array.toString();
    return stream;
  }

  std::istream & operator >> (std::istream & stream, Complex3dArray & array)
  {
    // lambda for stream input
    auto inputLambda = [&stream, real = (double)0, imaginary = (double)0]
    (Complex &reference) mutable
    {
      // while input is incorrect, repeat input again
      while (true)
      {
        std::cout << "input value:" << std::endl;

        stream >> real >> imaginary;
        
        if(stream.fail())
        {
          stream.clear();
          stream.ignore();
          std::cout << "incorrect input, try again" << std::endl;
        }
        
        break;
      }

      reference = Complex(real, imaginary);
    };

    // apply to the all 3darray
    array.myMap(inputLambda);

    return stream;
  }

  Shape shapeOfData(const Data &data)
  {
    // alloc values of the shape
    std::vector<std::size_t> values(3);
    
    // init values
    values[0] = data.size();
    values[1] = data[0].size();
    values[2] = data[0][0].size();

    // return result shape
    return Shape(values);
  }

  Complex3dArray & operator--(Complex3dArray & array)
  {
    // equal to reference --
    auto decrementFunctor = [](Complex & reference)
    {
      reference -= Complex(1, 1);
    };

    // apply to the all vector elements
    array.myMap(decrementFunctor);

    return array;
  }

  Complex3dArray & operator--(Complex3dArray & array, int i)
  {
    return operator -- (array);
  }

  Complex3dArray operator + (const Complex3dArray & complexArray, const Data & vector)
  {
    // check shapes
    Shape vectorShape = shapeOfData(vector);
    if (complexArray.getShape() != vectorShape)
    {
      throw lab3Exceptions::UnsummableShapesException(
        "can't sum (sub) with shapes " + complexArray.getShape().toString() +
        " and " + vectorShape.toString()
      );
    }


    return Complex3dArray::twistedBinaryOperation(
      complexArray,
      vector,
      [](const Complex &left, const Complex &right)
      {
        return left + right;
      }
    );
  }

  Complex3dArray operator - (const Complex3dArray & complexArray, const Data & vector)
  {
    // check shapes
    Shape vectorShape = shapeOfData(vector);
    if (complexArray.getShape() != vectorShape)
    {
      throw lab3Exceptions::UnsummableShapesException(
        "can't sum (sub) with shapes " + complexArray.getShape().toString() +
        " and " + vectorShape.toString()
      );
    }


    return Complex3dArray::twistedBinaryOperation(
      complexArray,
      vector,
      [](const Complex &left, const Complex &right)
      {
        return left - right;
      }
    );
  }

  Complex3dArray operator + (const Data & vector, const Complex3dArray & complexArray)
  {
    return operator + (complexArray, vector);
  }

  Complex3dArray operator - (const Data & vector, const Complex3dArray & complexArray)
  {
    // check shapes
    Shape vectorShape = shapeOfData(vector);
    if (complexArray.getShape() != vectorShape)
    {
      throw lab3Exceptions::UnsummableShapesException(
        "can't sum (sub) with shapes " + complexArray.getShape().toString() +
        " and " + vectorShape.toString()
      );
    }


    return Complex3dArray::twistedBinaryOperation(
      complexArray,
      vector,
      [](const Complex &left, const Complex &right)
      {
        return right - left;
      }
    );
  }

  bool operator == (const Complex3dArray & complexArray, const Data & vector)
  {
    return !(complexArray != vector);
  }

  bool operator == (const Data & vector, const Complex3dArray & complexArray)
  {
    return !(complexArray != vector);
  }

  bool operator != (const Complex3dArray & complexArray, const Data & vector)
  {
    // check shapes
    Shape vectorShape = shapeOfData(vector);
    if (complexArray.getShape() != vectorShape)
    {
      return true;
    }

    // check elements
    bool result = false;
    auto checkLambda = [&result](const Complex &left, const Complex &right)
    {
      if (left != right) {
        result = true;
        return true;
      }
      else
      {
        return false;
      }
    };
    Complex3dArray::twistedBoolMap(complexArray, vector, checkLambda);

    // return comparation result
    return result;
  }

  bool operator != (const Data & vector, const Complex3dArray & complexArray)
  {
    return complexArray != vector;
  }

  // ------------------------------------------------------------------------------------- friends

  Shape Complex3dArray::standartShape = Shape(std::vector<std::size_t>(3, 3));

  // -------------------------------------------------------------------------------------- constr/destr
  // default constructor
  Complex3dArray::Complex3dArray()
    : shape(standartShape), content(allocData(standartShape))
  {
    // log
    stupidLog(
      "default costructor : shape" + shape.toString() +
      ", content:" + this->toString());
  }

  // init constuctor
  Complex3dArray::Complex3dArray(const Shape &shape,
    const std::vector<Complex> &data)
    : shape(shape), content(allocData(shape))
  {
    // handle bad size of shape
    if (shape.getCoefsCount() != 3)
      throw lab3Exceptions::TooSmallValueException(
        "coefs attempt of shape isn't 3"
      );

    // init content
    this->operator ()(data);

    // log
    stupidLog(
      "init costructor : shape " + shape.toString() +
      " , content:" + this->toString());
  }

  Complex3dArray::Complex3dArray(const Shape & shape)
    : shape(shape), content(allocData(shape))
  {
  }

  // copy constructor
  Complex3dArray::Complex3dArray(const Complex3dArray &value)
    : shape(value.getShape()), content(new Data(*value.content))
  {
    // log
    stupidLog(
      "copy costructor : shape " + shape.toString() +
      ", content:" + this->toString());
  }

  Complex3dArray::~Complex3dArray()
  {
    // log
    stupidLog(
      "destructor : shape" + shape.toString() +
      ", content:" + this->toString());
    delete content;
  }
  // -------------------------------------------------------------------------------------- constr/destr
  // -------------------------------------------------------------------------------------- operators
  const Complex3dArray &Complex3dArray::operator=(const Complex3dArray &value)
  {
    // log
    stupidLog(
      "operator =: shape" + value.toString() +
      ", content:" + value.toString());

    // check for self-assignment
    if (this == &value)
      return *this;

    // copy content of value
    delete this->content;
    this->content = new Data(*(value.content));

    // copy shape of value
    this->shape = value.getShape();

    return *this;
  }

  void Complex3dArray::operator()(const std::vector<Complex>& data)
  {
    // handle bad size of data vector
    if (data.size() < shape[0] * shape[1] * shape[2])
      throw lab3Exceptions::TooSmallValueException(
        "value of data (" + std::to_string(data.size()) +
        ") is too small to fill in Complex3dArray" + shape.toString());
    
    stupidLog("init content in op () (shape, data)");
    // init the all arrays bins using data
    std::vector<Complex>::const_iterator dataIterator = data.begin();
    myMap(
      [&data, &dataIterator] (Complex & reference) mutable
      {
        reference = *dataIterator;
        ++dataIterator;
      }
    );
  }

  Complex &Complex3dArray::operator()(
    const int &row,
    const int &column,
    const int &deep) const
  {
    std::size_t rrow = makeRealIndex(row, shape[0]);
    std::size_t rcolumn = makeRealIndex(column, shape[1]);
    std::size_t rdeep = makeRealIndex(deep, shape[2]);

    return content->at(rrow)[rcolumn][rdeep];
  }

  std::vector<std::vector<Complex>>& Complex3dArray::operator[](int i) const
  {
    return content->operator[](i);
  }

  bool Complex3dArray::operator==(const Complex3dArray &value) const
  {
    // log
    stupidLog(
      "operator == : shape " + shape.toString() + ", content:" + this->toString() + " and " +
      "shape" + value.getShape().toString() + ", content" + value.toString());
    return !((*this) != value);
  }

  bool Complex3dArray::operator!=(const Complex3dArray &value) const
  {
    stupidLog(
      "operator != : shape " + shape.toString() + ", content:" + this->toString() + " and " +
      "shape" + value.getShape().toString() + ", content" + value.toString());

    // first check shapes
    if (shape != value.getShape())
      return true;

    // then check content
    bool result;
    myBoolZipMap(*this, value, [&](const auto &val1, const auto &val2) mutable {
      if (val1 != val2) {
        result = true;
        return true;
      }
      else
      {
        result = false;
        return false;
      }
    });

    return result;
  }

  bool Complex3dArray::operator>(const Complex3dArray &value) const
  {
    stupidLog(
      "operator > : shape " + shape.toString() + ", content:" + this->toString() + " and " +
      "shape" + value.getShape().toString() + ", content" + value.toString());

    // handle unuqual shapes
    if (shape != value.getShape())
      throw lab3Exceptions::UncomparableShapeException(
        std::string("can't compare shapes " + this->getShape().toString() +
          " and " + value.getShape().toString()));

    // check content using closure
    bool result;
    myBoolZipMap((*this), value, [&](auto &value1, auto &value2) mutable {
      if (value1 > value2)
      {
        result = true;
        return true;
      }

      if (value1 < value2)
      {
        result = false;
        return true;
      }

      return false;
    });

    return result;
  }

  bool Complex3dArray::operator<(const Complex3dArray &value) const
  {
    stupidLog(
      "operator < : shape " + shape.toString() + ", content:" + this->toString() + " and " +
      "shape" + value.getShape().toString() + ", content" + value.toString());

    // handle unuqual shapes
    if (shape != value.getShape())
      throw lab3Exceptions::UncomparableShapeException(
        "can't compare shapes " + this->getShape().toString() +
        " and " + value.getShape().toString());

    // check content using closure
    bool result;
    myBoolZipMap((*this), value, [&](auto &value1, auto &value2) mutable {
      if (value1 < value2)
      {
        result = true;
        return true;
      }

      if (value1 > value2)
      {
        result = false;
        return true;
      }

      return false;
    });

    return result;
  }

  bool Complex3dArray::operator>=(const Complex3dArray &value) const
  {
    stupidLog(
      "operator >= : shape " + shape.toString() + ", content:" + this->toString() + " and " +
      "shape" + value.getShape().toString() + ", content" + value.toString());

    return !((*this) < value);
  }

  bool Complex3dArray::operator<=(const Complex3dArray &value) const
  {
    stupidLog(
      "operator <= : shape " + shape.toString() + ", content:" + this->toString() + " and " +
      "shape" + value.getShape().toString() + ", content" + value.toString());

    return !((*this) > value);
  }

  Complex3dArray & Complex3dArray::operator++()
  {
    // increment lambda
    auto increment = [](Complex & reference) mutable
    {
      reference += Complex(1, 1);
    };

    // apply to the all vector bins
    myMap(increment);

    return *this;
  }

  Complex3dArray & Complex3dArray::operator++(int i)
  {
    return operator++();
  }

  Complex3dArray Complex3dArray::operator+(const Complex3dArray & value)
  {
    return binaryOperation(
      value,
      [](const Complex &left, const Complex &right)
      {
        return left + right;
      }
    );
  }

  Complex3dArray Complex3dArray::operator-(const Complex3dArray & value)
  {
    return binaryOperation(
      value,
      [](const Complex &left, const Complex &right)
      {
        return left - right;
      }
    );
  }

  void * Complex3dArray::operator new(std::size_t size)
  {
    return alloc(size);
  }

  void * Complex3dArray::operator new[](std::size_t size)
  {
    return alloc(size);
  }

  void Complex3dArray::operator delete(void * pointer)
  {
    free(pointer);
  }

  void Complex3dArray::operator delete[](void * pointer)
  {
    free(pointer);
  }

    Complex3dArray::operator double()
  {
    // check sizes
    std::size_t count = shape[0] * shape[1] * shape[2];
    if (count <= 1)
    {
      throw lab3Exceptions::TooSmallValueException(
        "shape of 3darray" + shape.toString() + "is too small to calc std deviation"
      );
    }

    double squaresSum = 0;

    // calc sum of squares
    myMap(
      [&squaresSum, mean = this->meanMedian()](const Complex &reference)
      {
        squaresSum += pow(reference.length() - mean, 2);
      }
    );

    // return std deviation
    return pow(squaresSum / (count - 1), 0.5);
  }

  Complex3dArray::operator Shape()
  {
    return shape;
  }
  // -------------------------------------------------------------------------------------- operators
  // -------------------------------------------------------------------------------------- std setters/getters
  void Complex3dArray::setStandartShape(const Shape &shape)
  {
    standartShape = shape;
  }

  const Shape &Complex3dArray::getStandartShape()
  {
    return standartShape;
  }
  // -------------------------------------------------------------------------------------- std setters/getters
  // -------------------------------------------------------------------------------------- support methods
  std::string Complex3dArray::toString() const
  {
    std::string output = "";

    // iterate in deeps and add them to string output
    for (std::size_t deep = 0; deep < shape[2]; deep++)
    {
      output += deepToString(deep, std::string());
    }

    // delete unneccesary spaces
    output.pop_back();
    output.pop_back();

    return std::string("<content>\n" + output + "\n</content>");
  }

  // beta
  void Complex3dArray::sort()
  {
    stupidLog(
      "sort (before): shape " + shape.toString() + ", content:" + this->toString());
    std::multiset<Complex> set;

    // add all vectors content to set
    myMap([&set](const Complex &arg) mutable {
      set.insert(arg);
    });

    // return content back
    this->operator()(set.begin(), set.end());

    stupidLog(
      "sort (result): shape " + shape.toString() + ", content:" + this->toString());
  }
  // -------------------------------------------------------------------------------------- support methods
  // -------------------------------------------------------------------------------------- setters/getters
  const Shape &Complex3dArray::getShape() const
  {
    return shape;
  }

  // -------------------------------------------------------------------------------------- setters/getters
  // -------------------------------------------------------------------------------------- support methods

  double Complex3dArray::meanMedian()
  {
    // return mean median
    double accumulator = 0;

    myMap(
      [&](const Complex & reference) mutable
      {
        accumulator += reference.length();
      }
    );

    return pow(accumulator, 1. / (shape[0] * shape[1] * shape[2]));
  }

  Data * Complex3dArray::allocData(const Shape & shape)
  {
    return new std::vector<std::vector<std::vector<Complex>>>(
      shape[0],
      std::vector<std::vector<Complex>>(
        shape[1],
        std::vector<Complex>(shape[2], Complex())
        )
      );
  }

  void * Complex3dArray::alloc(std::size_t size)
  {
    // pointer on allocated memory
    void * pointer;

    // try 10 times to allocate memory for object
    for (std::size_t attempt = 10; attempt; attempt--)
    {
      // check, that memory has been allocated
      if (pointer = malloc(size))
      {
        return pointer;
      }
    }

    // if memory hasn't been allocated since 10 times, throw exception
    throw std::bad_alloc();

    return nullptr;
  }

  void Complex3dArray::stupidLog(std::string &msg, const bool &error)
  {
    // no log
    if (true)
      return;

    if (error)
    {
      std::cerr << "log:ERROR:" << std::string(msg) << std::endl;
    }
    else
    {
      std::cout << "log:" << msg << std::endl;
    }
  }

  void Complex3dArray::stupidLog(const char * msg, const bool &error)
  {
    // no log
    if (true)
      return;

    if (error)
    {
      std::cerr << "log:ERROR:" << std::string(msg) << std::endl;
    }
    else
    {
      std::cout << "log:" << msg << std::endl;
    }
  }

  const std::string &Complex3dArray::deepToString(const int &deep, std::string &output) const
  {
    // buffer
    output = "deep " + std::to_string(deep) + ":\n";

    // iterate in deep and add all rows (which columns) to string output
    for (std::size_t row = 0; row < shape[0]; row++)
    {
      output += rowToString(row, deep, std::string());
    }

    return output += "\n";
  }

  const std::string &Complex3dArray::rowToString(
    const int &row,
    const int &deep,
    std::string &output) const
  {
    // buffer
    output = "|";

    // iterate in row and add all all columns to string output
    for (std::size_t column = 0; column < shape[1]; column++)
    {
      output += Complex3dArray::operator()(row, column, deep).toString() + ", ";
    }

    // delete unnecessary end ", "
    output.pop_back();
    output.pop_back();

    return output += "|\n";
  }

  std::size_t Complex3dArray::makeRealIndex(int index, std::size_t size)
  {
    // handle if index exceeds range (0, size)
    if (isBadIndex(index, size))
      throw lab3Exceptions::TooLargeValueException(
        "index (" + std::to_string(index) +
        ") exceeds range (0, " + std::to_string(size) + ")");

    return index >= 0 ? index : size + index;
  }

  // return true if index is bad
  bool Complex3dArray::isBadIndex(int index, std::size_t size)
  {
    return (std::size_t)index == size || (std::size_t)abs(index) > size;
  }
  // -------------------------------------------------------------------------------------- support methods
} // namespace lab34