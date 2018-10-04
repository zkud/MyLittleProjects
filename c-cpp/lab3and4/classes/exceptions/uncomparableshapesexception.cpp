#include "uncomparableshapesexception.hpp"

lab3Exceptions::UncomparableShapeException::UncomparableShapeException
(std::string & what) : length_error(what.c_str())
{
}
