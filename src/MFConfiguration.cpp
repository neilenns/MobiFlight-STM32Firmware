#include <map>
#include <mbed.h>

#include "MFConfiguration.hpp"

#define MAX_BUFFER_SIZE 40
char buffer[40];

void MFConfiguration::Load()
{
}

void MFConfiguration::Erase()
{
}

void MFConfiguration::Serialize()
{
  for (auto &[key, value] : outputs)
  {
    value.Serialize(buffer, sizeof(buffer));
    printf(buffer);
    printf(":");
  }
}