/*
* The MIT License (MIT)
*
* Copyright (c) JEMRF
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
********************************************************************************
*
* Based on the Arduino library by Jonathan Evans
* https://github.com/JemRF/max7219
*/

#include "drivers/max7219.hpp"

MAX7219::MAX7219(PinName mosi, PinName sclk, PinName cs, int submoduleCount)
{
  _spi = new SPI(mosi, NC, sclk);
  _cs = new DigitalOut(cs);
  _submoduleCount = submoduleCount;

  for (int i = 0; i < _submoduleCount; i++)
  {
    Begin(i);
  }
}

void MAX7219::MAX7219_ShutdownStart(int submodule)
{
  MAX7219_Write(REG_SHUTDOWN, 0, submodule);
}

void MAX7219::MAX7219_DisplayTestStart(int submodule)
{
  MAX7219_Write(REG_DISPLAY_TEST, 1, submodule);
}

void MAX7219::MAX7219_DisplayTestStop(int submodule)
{
  MAX7219_Write(REG_DISPLAY_TEST, 0, submodule);
}

void MAX7219::Clear(int submodule)
{
  for (int i = 0; i < 8; i++)
  {
    MAX7219_Write(i + 1, 0x00, submodule);
  }
}

void MAX7219::MAX7219_SetBrightness(char brightness, int submodule)
{
  brightness &= 0x0f;
  MAX7219_Write(REG_INTENSITY, brightness, submodule);
}

unsigned char MAX7219::MAX7219_LookupCode(char character, unsigned int dp)
{
  int i;
  unsigned int d = 0;
  if (dp)
    d = 1;
  if (character >= 35 && character <= 44)
  {
    character += 13;
    d = 1;
  }
  for (i = 0; MAX7219_Font[i].ascii; i++)
    if (character == MAX7219_Font[i].ascii)
    {
      if (d)
      {
        d = MAX7219_Font[i].segs;
        d |= (1 << 7);
        return (d);
      }
      else
      {
        return MAX7219_Font[i].segs;
      }
    }

  return 0;
}

void MAX7219::DisplayText(char *text, int justify)
{
  int decimal[16];
  char trimStr[16] = "";
  int x, y = 0;
  int s;

  s = strlen(text);
  if (s > 16)
    s = 16;
  for (x = 0; x < s; x++)
  {
    if (text[x] == '.')
    {
      decimal[y - 1] = 1;
    }
    else
    {
      trimStr[y] = text[x];
      decimal[y] = 0;
      y++;
    }
  }
  if (y > 8)
    y = 8;
  for (x = 0; x < y; x++)
  {
    if (justify == 0)
      DisplayChar((int)(y - x + 7 - y), trimStr[x], decimal[x]);
    if (justify == 1)
    {
      DisplayChar((int)(y - x + 7 - y - (8 - y)), trimStr[x], decimal[x]);
    }
  }
}

void MAX7219::MAX7219_Write(volatile int opcode, volatile int data, volatile int submodule)
{
  // A chained set of modules is basically just chained shift registers.
  // When sending data out to a specific module it's important to make sure
  // all the other submodules, both before and after the target submodule,
  // receive no-op commands. This tracks how many modules got written to.
  int modulesWrittenTo = 0;

  _cs->write(0);
  _spi->lock();

  // Send no-op commands to the submodules before the requested momdule
  for (int i = 0; i < submodule; i++)
  {
    _spi->write(REG_NOOP);
    _spi->write(0);
    modulesWrittenTo++;
  }

  // Send the command to the requested module
  _spi->write(opcode);
  _spi->write(data);
  modulesWrittenTo++;

  // Send noop commands to the remaining modules
  for (int i = modulesWrittenTo; i < _submoduleCount; i++)
  {
    _spi->write(REG_NOOP);
    _spi->write(0);
  }

  _spi->unlock();
  _cs->write(1);
}

void MAX7219::DisplayChar(int digit, char value, bool dp, int submodule)
{
  MAX7219_Write(digit + 1, MAX7219_LookupCode(value, dp), submodule);
}

void MAX7219::MAX7219_ShutdownStop(int submodule)
{
  MAX7219::MAX7219_Write(REG_SHUTDOWN, 1, submodule);
}

void MAX7219::Begin(int submodule)
{
  MAX7219_Write(REG_SCAN_LIMIT, 7, submodule);
  MAX7219_Write(REG_DECODE, 0x00, submodule);
  MAX7219_ShutdownStop(submodule);
  MAX7219_DisplayTestStop(submodule);
  Clear(submodule);
  MAX7219_SetBrightness(INTENSITY_MAX, submodule);
}