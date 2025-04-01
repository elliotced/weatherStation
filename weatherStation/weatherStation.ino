/*
* Name: Interactive Weather Station
* Author: Elliot Cederlund
* Date: 2025-03-18
* Description: This project displays different types of weather-related data onto a screen. The user can control which data is displayed.
*/

// include libraries
#include "U8glib.h"
#include <DHT11.h>
#include <RotaryEncoder.h>

// include constants
const int datas = 3; //number of data that can be displayed, starts at 0

const int dhtPin = 2;
const int lightPin = 3;
const uint8_t  rotaryPin1 = A1;
const uint8_t  rotaryPin2 = A2;
const uint8_t  rotaryPin3 = A3;

// init global variables

// construct objects
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);	// Display which does not send AC
DHT11 dht11(dhtPin);
RotaryEncoder encoder(rotaryPin2, rotaryPin3, RotaryEncoder::LatchMode::TWO03);

void setup(void) {
  // init software
  Serial.begin(9600);
  u8g.setFont(u8g_font_helvB12);
}

void loop(void) {
  static int index = 0;
  static int lastPos = 0;
  
  static bool mode = false;

  encoder.tick();
  int step = encoder.getPosition();
  
  int stepsPerDetent = 2; // Adjust this value based on your encoder's specs
  int newPos = step / stepsPerDetent;

  if (newPos != lastPos) {

    index += 1 * int(encoder.getDirection());
    if (index < 1) {
      index = datas;
    }
    else if (index > datas) {
      index = 1;
    }

    if (!mode){
      menuWrite(index);
    }

    lastPos = newPos;
  }

  static bool pressed = false;

  if (digitalRead(rotaryPin1) == LOW) {
    if (!pressed) {
      if (mode) {
        u8g.setFont(u8g_font_helvB12);
        mode = false;
        menuWrite(index);
      }
      else {
        u8g.setFont(u8g_font_helvB24);
        mode = true;
      }
      pressed = true;
    }
  }
  else {
    pressed = false;
  }

  if (mode) {
    dataWrite("20 C");
  }
}

void menuWrite(int index){
  u8g.firstPage();  
  do {
    u8g.drawStr(0,20*index,">");
    
    u8g.drawStr(20,20,"Temperature");
    u8g.drawStr(20,40,"Humidity");
    u8g.drawStr(20,60,"Light Level");
  } while(u8g.nextPage());
}

void dataWrite(String data){
  u8g.firstPage();  
  do {
    u8g.drawStr(20,46,data.c_str());
  } while( u8g.nextPage() );
}


