/*7segLED.h - Library for operate 7segLED code.
  Created by Wataru Ikeda, November 28, 2021.
  Released into the public domain.*/

#include <Arduino.h>
#ifndef sevensegLED_h
#define sevensegLED_h

class sevensegLED{
 public:
  sevensegLED(int datapin, int latchpin, int clockpin);
  void begin();
  void Show_seg(int nKeta, int nValue ,int tmp_or_hum);
  void Set_seg(int nKeta, int nValue ,int tmp_or_hum);
  void Out_digit(int nKeta, int nValue);
 private:
  int _datapin;
  int _latchpin;
  int _clockpin;
};

#endif
