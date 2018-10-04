#include "toolargevalueexception.hpp"

lab3Exceptions::TooLargeValueException::TooLargeValueException
(const std::string & what) : length_error(what.c_str())
{

}
