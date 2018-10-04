#pragma once
#include <stdexcept>

namespace lab3Exceptions
{
  class BadValueException :
    public std::length_error
  {
  public:
    BadValueException(std::string &what);
    BadValueException(const char *cstring);
  };
}

