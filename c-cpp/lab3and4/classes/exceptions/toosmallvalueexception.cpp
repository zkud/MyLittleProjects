#include "toosmallvalueexception.hpp"


lab3Exceptions::TooSmallValueException::TooSmallValueException
(const std::string & what) : length_error(what.c_str())
{
  
}
