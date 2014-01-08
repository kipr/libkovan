#include <kovan/depth_exception.hpp>

depth::Exception::Exception(const std::string &msg)
  : std::runtime_error(msg)
{
}