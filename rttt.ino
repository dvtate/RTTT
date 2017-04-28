#include <Arduino.h>
#include <FastLED.h>
#include "grid.hpp"

#define LED_DATA_PIN 10
CRGB leds[81];

Grid boards[3][3] {
  {
    Grid(0, 1, 2, 27, 28, 29, 54, 55, 56),
    Grid(3, 4, 5, 30, 31, 32, 57, 58, 59),
    Grid(6, 7, 8, 33, 34, 35, 60, 61, 62)
  }, {
    Grid(9, 10, 11, 36, 37, 38, 63, 64, 65),
    Grid(12, 13, 14, 39, 40, 41, 66, 67, 68),
    Grid(15, 16, 17, 42, 43, 44, 69, 70, 71)
  }, {
    Grid(18, 19, 20, 45, 46, 47, 72, 73, 74),
    Grid(21, 22, 23, 48, 49, 50, 75, 76, 77),
    Grid(24, 25, 26, 51, 52, 53, 78, 79, 80)
  },
};

uint8_t buttonPins[3][3] {
  { 1, 2, 3 }, { 4, 5, 6 },
  { 7, 8, 9 }
};

void setup() {

  // assuming we have an unconnected analog pin...
  randomSeed(analogRead(0));

  // mark button pins as input
  for (uint8_t x = 0; x < 3; x++)
    for (uint8_t y = 0; y < 3; y++)
      pinMode(buttonPins[x][y]);

  
  // link the colors to the LEDs
  FastLED.addLeds <WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);

}

void loop() {

  static possession_t curPlayer = RED;

  // suggest a random square for each grid
  for (uint8_t x = 0; x < 3; x++)
    for (uint8_t y = 0; y < 3; y++)
      boards[x][y].setRandPos(curPlayer);

  // render and check for input
  for (uint8_t i = 0; i < 3; i++) {
    // set colors
    for (uint8_t x = 0; x < 3; x++)
      for (uint8_t y = 0; y < 3; y++)
        boards[x][y].bindToLeds(i % 2);
    FastLED.show();

    // check for input
    bool inputRecieved = false;
    for (uint8_t g = 0; g < 100 && !inputRecieved; g++) {
      
      for (uint8_t x = 0; x < 3 && !inputRecieved; x++)
        for (uint8_t y = 0; y < 3; y++)
          if (digitalRead(buttonPins[x][y])) {
            boards[x][y].keepSuggestion();
            inputRecieved = true;
          }
          
      delay(5);
    }    
    if (inputRecieved)
      goto next_player;

    
  }

next_player:
  if (curPlayer == RED)
    curPlayer = BLUE;
  else
    curPlayer = RED;
}
