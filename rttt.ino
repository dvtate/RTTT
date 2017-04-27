#include <Arduino.h>
#include <FastLED.h>
#include "grid.hpp"

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

void setup() {

  // assuming we have an unconnected analog pin...
  randomSeed(analogRead(0));
}

void loop() {

}
