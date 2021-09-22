#include <map>
#include <mbed.h>

#include "MFConfiguration.hpp"

void MFConfiguration::Load()
{
}

void MFConfiguration::Erase()
{
}

// This nonsense comes from https://codereview.stackexchange.com/questions/185302/elegant-output-separators
// I have no idea how it works.
template <typename T = bool, T initial = true, T fallback = false>
struct once
{
  mutable T flag = initial;

  operator T() const { return std::exchange(flag, fallback); }
};

template <typename T = char const *>
struct one_off
{
  mutable once<> flag;

  T first, other;
  one_off(T first, T other) : first(std::move(first)), other(std::move(other)) {}

  operator T() const { return flag ? first : other; }
};

std::ostream &operator<<(std::ostream &os, const MFConfiguration &obj)
{
  auto separator = one_off("", ":");

  for (const auto &[key, value] : obj.outputs)
  {
    os << separator << value;
  }
  return os;
}