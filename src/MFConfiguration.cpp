#include <map>
#include <mbed.h>

#include "MFConfiguration.hpp"

void MFConfiguration::Load()
{
}

void MFConfiguration::Erase()
{
}

std::ostream &operator<<(std::ostream &os, const MFConfiguration &obj)
{
  for (const auto &[key, value] : obj.outputs)
  {
    os << value << ":";
  }
  return os;
}