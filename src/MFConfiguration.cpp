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
  // TODO: This results in an extra trailing : that shouldn't be there. Figure out how to get rid of it.
  // Run through all the buttons
  for (const auto &[key, value] : obj.outputs)
  {
    os << value;
  }
  return os;
}