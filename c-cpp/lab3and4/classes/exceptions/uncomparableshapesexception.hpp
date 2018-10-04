#ifndef __UNCOMPARABLESHAPESEXCEPTION_HPP__
#define __UNCOMPARABLESHAPESEXCEPTION_HPP__
#include <stdexcept>

namespace lab3Exceptions
{
  class UncomparableShapeException : public std::length_error
  {
  public:
    UncomparableShapeException(std::string & what);
  };
}
#endif