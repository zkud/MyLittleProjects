#include "badvalueexception.hpp"


namespace lab3Exceptions
{
  BadValueException::BadValueException(std::string & what)
    : length_error(what)
  {
  }
}

