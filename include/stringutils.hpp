/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>

#include "Globals.hpp"

namespace stringutils
{
  /**
   * @brief Splits a string into a vector of substrings.
   * 
   * @param str The string to split.
   * @param delimiter The delimiter to split on.
   * @param substrings The vector to fill with the substrings
   */
  void split(const std::string &str, const std::string &delimiter, std::vector<std::string> &substrings)
  {
    auto start = 0U;
    auto end = str.find(delimiter);
    while (end != std::string::npos)
    {
      substrings.push_back(str.substr(start, end - start));
      start = end + delimiter.length();
      end = str.find(delimiter, start);
    }

    substrings.push_back(str.substr(start, end));
  }
}
