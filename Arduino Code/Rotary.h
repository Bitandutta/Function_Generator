#ifndef rotary_h
#define rotary_h

#include "Arduino.h"

//#define HALF_STEP

#define ENABLE_PULLUPS

#define DIR_NONE 0x0

#define DIR_CW 0x10

#define DIR_CCW 0x20

class Rotary
{
  public:
    Rotary(char, char);
    // Process pin(s)
    unsigned char process();
  private:
    unsigned char state;
    unsigned char pin1;
    unsigned char pin2;
};

#endif
 
