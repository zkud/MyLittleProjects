#include "shape.hpp"
#include <cmath>
#include <string>
#include <iostream>
#include <iterator>

// todo add logging
namespace lab34
{
  Shape::Shape(const std::vector<std::size_t> &coefs)
    : content(coefs), coefsCount(coefs.size())
  {
  }

  Shape::~Shape()
  {
  }

  Shape::Shape(const Shape &value)
    : content(value.content), coefsCount(value.coefsCount)
  {
  }

  bool Shape::operator==(const Shape &value) const
  {
    return !(*this != value);
  }

  bool Shape::operator!=(const Shape &value) const
  {
    std::size_t size1 = this->getCoefsCount();
    std::size_t size2 = value.getCoefsCount();

    // check for equal sizes
    if (size1 != size2)
      return true;
    else
    {
      //// check for equal coefs
      //for (std::size_t index = 0; index < size1; index++)
      //  if ((*this)[index] != value[index])
      //    return true;
      
      // check for equal coefs v2
      typedef std::vector<std::size_t>::const_iterator iter;
      for (iter left = content.begin(), right = value.content.begin(); left != content.end(); ++left, ++right)
      {
        if (*left != *right)
        {
          return true;
        }
      }

      return false;
    }
  }

  const Shape &Shape::operator=(const Shape &value)
  {
    // check for self-asignment
    if (this == &value)
      return *this;
    else
    {
      this->content = value.content;
      this->coefsCount = value.coefsCount;
      return *this;
    }
  }

  const std::size_t &Shape::operator[](int index) const
  {
    // check for index value
    if ((std::size_t)index >= coefsCount || (std::size_t)abs(index) > coefsCount)
      throw lab3Exceptions::TooLargeValueException(
        "to large index (" + std::to_string(index) +
        ") for number of dimentions = " + std::to_string(coefsCount)
      );

    // form index for content
    std::size_t realIndex = index >= 0 ? index : coefsCount + index;

    return content[realIndex];
  }

  std::string Shape::toString() const
  {
    std::string output = "(";

    // add all coefs
    for (std::size_t coef : content)
      output += std::to_string(coef) + ", ";

    // clear from unnecessary space
    output.pop_back();
    output.pop_back();

    return std::string(output + ")");
  }

  const std::size_t &Shape::getCoefsCount() const
  {
    return coefsCount;
  }
} // namespace lab34