#ifndef AD9833_H
#define AD9833_H

#include <SPI.h>
#include "LiquidCrystal_I2C.h"

class AD9833
{
  public:
    AD9833(int _FSYNC, unsigned long _mclk);
    void setFreq(unsigned long _freq);
    unsigned long getFreq();
    void setPhase(int _phase);
    int getPhase();

    void setCtrlReg(unsigned long _controlRegister);
    int getCtrlReg();
    void writeData(int data);
    void writeCtrlReg();
    void sleep(int s);
    void reset(int r);
    void mode(int m);
    void setFPRegister(int r);
    void lcdDebugInit(LiquidCrystal_I2C *);
    void lcdDebug(String);

  private:
    int FSYNC;          //FSYNC Pin of AD9833 has to be connected to a GPIO pin
    unsigned long freq; //Output frequency
    int phase;          //Signal phase shift
    unsigned long mclk; //External oscillator frequency
    int fqRegister;    //Switch between Frequency and Phase register 1/0
    int pRegister;
    int controlRegister;
    LiquidCrystal_I2C *debugger; //Debugging stuff through I2C
    boolean debugging;
};

#endif
