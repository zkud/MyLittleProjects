#ifndef __TOOSMALLVALUE_HPP__
#define __TOOSMALLVALUE_HPP__
#include <stdexcept>

namespace lab3Exceptions
{
  class TooSmallValueException : public std::length_error
  {
  public:
    TooSmallValueException(const std::string & what);
  };
} // namespace lab34.exceptions

#endif