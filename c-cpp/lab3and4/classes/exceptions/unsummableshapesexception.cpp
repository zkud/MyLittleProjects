#include "unsummableshapesException.hpp"


namespace lab3Exceptions
{
  UnsummableShapesException::UnsummableShapesException(std::string & what)
    : length_error(what)
  {
  }
}

