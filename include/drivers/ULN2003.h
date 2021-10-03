/**
 *  @file ULN2003.h
 *  Library to control 4 phase stepper motors using ULN2003
 *
 *  @author Fernando Cosentino
*/

#ifndef __ULN2003
#define __ULN2003

#include "mbed.h"

/**
 *  ULN2003 Class
 * 
 *  @author Fernando Cosentino
 */
class ULN2003 {
public:
    /**
     *  Creates an instance.
     *
     *  @param pinOut1 Pin connected to IN1 in ULN2003 board
     *  @param pinOut2 Pin connected to IN2 in ULN2003 board
     *  @param pinOut3 Pin connected to IN3 in ULN2003 board
     *  @param pinOut4 Pin connected to IN4 in ULN2003 board
     *  @param full_turn_steps Amount of steps for a complete turn (4096 for 28BYJ-48)
     */
    ULN2003(PinName pinOut1, PinName pinOut2, PinName pinOut3, PinName pinOut4, int full_turn_steps = 4096);
    
    /**
     *  Moves the motor one step clockwise, and leaves the motor powered.
     */
    void stepForward();

    /**
     *  Moves the motor one step counter-clockwise, and leaves the motor powered.
     */
    void stepReverse();
    
    /**
     *  Moves the motor a number of steps clockwise, in the given speed. Leaves
     *  the motor in relaxed state (no power in any coil).
     *
     *  @param steps Number of steps to move
     *  @param speed Speed in steps per second
     *  @note 28BYJ-48 can't handle speeds higher than 833 steps per second
     */
    void moveForward(int steps, float speed = 500);

    /**
     *  Moves the motor a number of steps counter-clockwise, in the given speed. 
     *  Leaves the motor in relaxed state (no power in any coil).
     *
     *  @param steps Number of steps to move
     *  @param speed Speed in steps per second
     *  @note 28BYJ-48 can't handle speeds higher than 833 steps per second
     */
    void moveReverse(int steps, float speed = 500);
    
    /**
     *  Moves the motor a number of turns clockwise, in the given speed. 
     *  Leaves the motor in relaxed state (no power in any coil).
     *
     *  @param turns Number of complete turns to move (can be fractional)
     *  @param speed Speed in complete turns per second
     *  @note 28BYJ-48 can't handle speeds higher than 0.2 turns per second
     */
    void turnForward(float turns, float speed = 0.2); 

    /**
     *  Moves the motor a number of turns counter-clockwise, in the given speed. 
     *  Leaves the motor in relaxed state (no power in any coil).
     *
     *  @param turns Number of complete turns to move (can be fractional)
     *  @param speed Speed in complete turns per second
     *  @note 28BYJ-48 can't handle speeds higher than 0.2 turns per second
     */
    void turnReverse(float turns, float speed = 0.2); 
    
    
    /**
     *  Removes power from all coils, so no energy is spent and the motor is
     *  free to be moved passively.
     */
     void relax();

private:    
    void _set_output();
    DigitalOut _out1;
    DigitalOut _out2;
    DigitalOut _out3;
    DigitalOut _out4;
    int _pos;
    int _full_turn_steps;
};

#endif
