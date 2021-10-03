#include "drivers/ULN2003.h"

ULN2003::ULN2003(PinName pinOut1, PinName pinOut2, PinName pinOut3, PinName pinOut4, int full_turn_steps) : _out1(pinOut1), _out2(pinOut2), _out3(pinOut3), _out4(pinOut4)
{
    _pos = 0;
    _full_turn_steps = full_turn_steps;
    relax();
}

void ULN2003::_set_output()
{
    switch (_pos)
    {
    case 0:
    {
        _out1 = 1;
        _out2 = 0;
        _out3 = 0;
        _out4 = 0;
        break;
    }
    case 1:
    {
        _out1 = 1;
        _out2 = 1;
        _out3 = 0;
        _out4 = 0;
        break;
    }
    case 2:
    {
        _out1 = 0;
        _out2 = 1;
        _out3 = 0;
        _out4 = 0;
        break;
    }
    case 3:
    {
        _out1 = 0;
        _out2 = 1;
        _out3 = 1;
        _out4 = 0;
        break;
    }
    case 4:
    {
        _out1 = 0;
        _out2 = 0;
        _out3 = 1;
        _out4 = 0;
        break;
    }
    case 5:
    {
        _out1 = 0;
        _out2 = 0;
        _out3 = 1;
        _out4 = 1;
        break;
    }
    case 6:
    {
        _out1 = 0;
        _out2 = 0;
        _out3 = 0;
        _out4 = 1;
        break;
    }
    case 7:
    {
        _out1 = 1;
        _out2 = 0;
        _out3 = 0;
        _out4 = 1;
        break;
    }
    }
}

void ULN2003::stepForward()
{
    _pos++;
    if (_pos > 7)
        _pos = 0;
    _set_output();
}

void ULN2003::stepReverse()
{
    _pos--;
    if (_pos < 0)
        _pos = 7;
    _set_output();
}

void ULN2003::moveForward(int steps, float speed)
{
    int delay_us_value = (1.0 / speed) * 1000000; // speed is in Hz = steps/sec

    for (int i = 0; i < steps; i++)
    {
        stepForward();
        wait_us(delay_us_value);
    }
    relax();
}

void ULN2003::moveReverse(int steps, float speed)
{
    int delay_us_value = (1.0 / speed) * 1000000; // speed is in Hz = steps/sec

    for (int i = 0; i < steps; i++)
    {
        // Fix a bug from the original library where this was stepForward()
        stepReverse();
        wait_us(delay_us_value);
    }
    relax();
}

void ULN2003::turnForward(float turns, float speed)
{
    moveForward(_full_turn_steps * turns, _full_turn_steps * speed);
}

void ULN2003::turnReverse(float turns, float speed)
{
    // Fix a bug from the original library where this was moveForward()
    moveReverse(_full_turn_steps * turns, _full_turn_steps * speed);
}

void ULN2003::relax()
{
    _out1 = 0;
    _out2 = 0;
    _out3 = 0;
    _out4 = 0;
}