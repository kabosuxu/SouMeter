/*sevensegLED.cpp - Library operate 7segLED code.
  Created by Wataru Ikeda, November 28, 2021.
  Released into the public domain.*/
#include <Arduino.h>
#include "sevensegLED.h"

const byte digit[] = //7seg light up pattern(Anode Common)
{
  B11000000, //0
  B11111001, //1
  B10100100, //2
  B10110000, //3
  B10011001, //4
  B10010010, //5
  B10000010, //6
  B11111000, //7
  B10000000, //8
  B10010000, //9
  B10111111  //-
};
const byte segment_tmp[] =
{
  B00000100, //Temperature data-10th place(Blue)
  B00000010, //Temperature data-1th place(Orange)
  B00000001, //Temperature data-0.1th place(Yellow)
};
const byte segment_hum[] =
{
  B01000000, //Humidity data-10th place(Blue)
  B00100000, //Humidity data-1th place(Orange)
  B00010000, //Humidity data-0.1th place;
};

sevensegLED::sevensegLED(int datapin, int latchpin, int clockpin){
  _datapin = datapin;
  _latchpin = latchpin;
  _clockpin = clockpin;
}

void sevensegLED::begin(){
  pinMode(_datapin, OUTPUT);
  pinMode(_latchpin, OUTPUT);
  pinMode(_clockpin, OUTPUT);
}

void sevensegLED::Show_seg(int nKeta, int nValue ,int tmp_or_hum){
  Set_seg(nKeta, nValue ,tmp_or_hum);
  Out_digit(nKeta, nValue);
}

void sevensegLED::Set_seg(int nKeta, int nValue ,int tmp_or_hum){
  digitalWrite(_latchpin, LOW);
  switch(tmp_or_hum){
    //Tmp
    case 1:
      shiftOut(_datapin, _clockpin, MSBFIRST,segment_tmp[nKeta]);
      //Serial.println("tmp");
      break;
    //Hum
    case 2:
      shiftOut(_datapin, _clockpin, MSBFIRST,segment_hum[nKeta]);
      //Serial.println("hum");
      break;
    default:
      Serial.println("Error");
  }
}

void sevensegLED::Out_digit(int nKeta, int nValue)
{
  if (nKeta == 1) 
    shiftOut(_datapin, _clockpin, MSBFIRST, digit[nValue] ^ B10000000); // period
  else
    shiftOut(_datapin, _clockpin, MSBFIRST, digit[nValue]); // sensing data
  digitalWrite(_latchpin, HIGH);
  delay(1);
}
