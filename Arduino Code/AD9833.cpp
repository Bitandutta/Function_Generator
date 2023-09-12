#include <SPI.h>
#include "AD9833.h"

AD9833::AD9833(int _FSYNC, unsigned long _mclk) {
  FSYNC = _FSYNC;
  pinMode(FSYNC, OUTPUT);     
  mclk = _mclk;              
  controlRegister = 0x2000;   
  fqRegister = 0x4000;      
  pRegister = 0xC000;         
  SPI.begin();              
  SPI.setDataMode(SPI_MODE2); // Set SPI in mode2, this should be moved in
}

void AD9833::writeData(int data) {
  digitalWrite(FSYNC, LOW);
  SPI.transfer(highByte(data));
  SPI.transfer(lowByte(data));
  digitalWrite(FSYNC, HIGH);
  if (debugging) {
    lcdDebug(String(data, HEX));
  }
}

void AD9833::setFreq(unsigned long _freq) {
  unsigned long freqReg;
  if (_freq < 0) {
    freqReg = 0;
    freq = 0;
  }
  else if (_freq > mclk) {
    freqReg = pow(2, 28) - 1;
    freq = mclk / 2;
  }
  else {
    freq = _freq;
    freqReg = (freq * pow(2, 28)) / mclk;
  }
  int MSW = ((int)(freqReg >> 14)) | fqRegister; // Take out the first 14bits

  int LSW = ((int)(freqReg & 0x3FFF)) |
            fqRegister; // Take only the last 14bits using a mask and set D15 to

  writeData(LSW);
  writeData(MSW);
}
unsigned long AD9833::getFreq() {
  return freq;
}
void AD9833::setPhase(int _phase) {

  if (_phase < 0) {
    phase = 0;
  }

  else if (_phase >= 4096) {
    phase = 4096 - 1;
  }

  else {
    phase = _phase;
  }

  int phaseData = phase | pRegister;
  int LSW = (phase & 0x3FFF) | pRegister;
  writeData(phaseData);
}
int AD9833::getPhase() {
  return phase;
}

void AD9833::setCtrlReg(unsigned long _controlRegister) {
  controlRegister = _controlRegister & 0x3FFF;
  writeCtrlReg();
}

int AD9833::getCtrlReg() {
  return controlRegister;
}

void AD9833::writeCtrlReg() {
  writeData(controlRegister);
}

void AD9833::sleep(int s) {
  switch (s) {

    case 0: {
        controlRegister &= 0xFF3F; // No power-down: D7-0 and D6-0
      } break;

    case 1: {
        controlRegister &= 0xFF7F; // DAC powered down: D7-0 and D6-1
        controlRegister |= 0x0040;
      } break;

    case 2: {
        controlRegister &= 0xFFBF; // Internal clock disabled: D7-1 and D6-0
        controlRegister |= 0x0080;
      } break;

    case 3: {
        controlRegister |=
          0x00C0; // Both DAC powered down and internal clock disabled
      } break;
  }
  // Update the control register
  writeCtrlReg();
}

void AD9833::reset(int r) {
  if (r == 0) {
    controlRegister &= 0xFEFF; // Set D8 to 0
  }

  else if (r == 1) {
    controlRegister |= 0x0100; // Set D8 to 1
  }
  writeCtrlReg();
}

void AD9833::mode(int m) {
  switch (m) {
    case 0: {
        controlRegister &= 0xFFDD; // Output sine: D5-0 and D1-0
      } break;
    case 1: {
        controlRegister &= 0xFFDF; // Output triangle: D5-0 and D1-1
        controlRegister |= 0x0002;
      } break;
    case 2: {
        controlRegister &= 0xFFFD; // Output clock (rectangle): D5-1 and D1-0
        controlRegister |= 0x0020;
      } break;
  }
  writeCtrlReg();
}

void AD9833::setFPRegister(int r) {
  if (r == 0) {
    controlRegister &= 0xF3FF; // Set D11 and D10 in control register to 0
    fqRegister = 0x4000; // Set D15 to 0 and D14 to 1 in a variable that will
    pRegister =
      0xC000; // Set D15 to 1 and D14 to 1 and D13 to 0 for the PHASE register
  } else if (r == 1) {
    controlRegister |= 0x0C00; // Set D11 and D10 in control register to 1
    fqRegister = 0x8000; // Set D15 to 1 and D14 to 0 in a variable that will
    pRegister =
      0xD000; // Set D15 to 1 and D14 to 1 and D13 to 1 for the PHASE register
  }
  writeCtrlReg();
}

void AD9833::lcdDebugInit(LiquidCrystal_I2C *_debugger) {
  debugger = _debugger;
  debugging = true;
}

void AD9833::lcdDebug(String message) {
  if (debugging) {
    debugger->setCursor(0, 1);
    debugger->print(message);
  }
}
