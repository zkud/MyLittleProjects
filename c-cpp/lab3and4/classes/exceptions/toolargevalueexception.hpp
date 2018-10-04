#ifndef __TOOLARGEVALUE_HPP__
#define __TOOLARGEVALUE_HPP__
#include <stdexcept>

namespace lab3Exceptions
{
  class TooLargeValueException : public std::length_error
  {
  public:
    TooLargeValueException(const std::string &what);
  };
} // namespace lab34.exceptions

#endif