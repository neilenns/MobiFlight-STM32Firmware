#include <map>
#include <mbed.h>

#include "MFConfiguration.hpp"

// Based on the code at https://os.mbed.com/docs/mbed-os/v6.14/apis/littlefilesystem.html
void MFConfiguration::Save()
{
}

void MFConfiguration::Load()
{
}

void MFConfiguration::Erase()
{
}

std::ostream &operator<<(std::ostream &os, const MFConfiguration &obj)
{
  // Run through all the buttons
  for (const auto &[key, value] : obj.outputs)
  {
    os << value;
  }
  return os;
}