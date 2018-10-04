#pragma once
#include <stdexcept>

namespace lab3Exceptions
{
  class UnsummableShapesException :
    public std::length_error
  {
  public:
    UnsummableShapesException(std::string &what);
  };
}

