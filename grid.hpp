#ifndef GRID_HPP
#define GRID_HPP

#include <inttypes.h>

extern CRGB leds[81];

// who owns the square?
enum possession_t { NONE = 0, RED = 1, BLUE = 2 };

// location
struct coord_t {
  unsigned int x : 2, y : 2;

  coord_t(uint8_t _x, uint8_t _y): x(_x), y(_y) {}
};

// 3v3 tictactoe grid
class Grid {
public: 
  // the 3 
  possession_t b00, b01, b02,
               b10, b11, b12,
               b20, b21, b22;

  // locations on LED array
  unsigned int v00 : 7, v01 : 7, v02 : 7,
               v10 : 7, v11 : 7, v12 : 7,
               v20 : 7, v21 : 7, v22 : 7;

  // current position
  struct suggestion_t {
    coord_t pos;
    possession_t color;
    suggestion_t(coord_t loc, possession_t clr):
      pos(loc), color(clr) {}
  } suggestion;

  // did anyone win?
  bool isWon : 1;
    
  Grid(uint8_t _v00, uint8_t _v01, uint8_t _v02,
       uint8_t _v10, uint8_t _v11, uint8_t _v12,
       uint8_t _v20, uint8_t _v21, uint8_t _v22):
    b00(NONE), b01(NONE), b02(NONE),
    b10(NONE), b11(NONE), b12(NONE),
    b20(NONE), b21(NONE), b22(NONE),
    v00(_v00), v01(_v01), v02(_v02),
    v10(_v10), v11(_v11), v12(_v12),
    v20(_v20), v21(_v21), v22(_v22),
    suggestion(coord_t(3, 3), NONE),
    isWon(false)
  {
  }

  void setRandPos(const possession_t clr){
    
    suggestion.color = clr;
    
    // pick a random empty space to put block in
    uint8_t randnum = random((unsigned long)-1) % numEmptySpaces();

  
    int i = 0;
    coord_t tmp { 0, 0 };
    for (;tmp.x < 3; tmp.x++)
      for (;tmp.y < 3; tmp.y++) {
        if (getBox(tmp) == NONE)
          i++;
        if (i == randnum) {
          suggestion.pos = tmp;
          return;
        }
      }
  }

  uint8_t numEmptySpaces(){
    uint8_t ret = 0;
    coord_t tmp { 0, 0 };
    for (;tmp.x < 3; tmp.x++)
      for (;tmp.y < 3; tmp.y++)
        if (getBox(tmp) == NONE)
          ret++;

    return ret;
    
  }

  possession_t& getBox(const coord_t coord){
    switch (coord.x) {
    case 0:
      switch (coord.y) {
        case 0: return b00;
        case 1: return b01;
        case 2: return b02;
      }
    case 1: 
      switch (coord.y) {
        case 0: return b10;
        case 1: return b11;
        case 2: return b12;
      }
    case 2:
      switch (coord.y) {
        case 0: return b20;
        case 1: return b21;
        case 2: return b22;
      }
    }
  }

  uint8_t getLed(const coord_t coord){
    switch (coord.x) {
    case 0:
      switch (coord.y) {
        case 0: return v00;
        case 1: return v01;
        case 2: return v02;
        default: return 100;
      }
    case 1: 
      switch (coord.y) {
        case 0: return v10;
        case 1: return v11;
        case 2: return v12;
        default: return 100;
      }
    case 2:
      switch (coord.y) {
        case 0: return v20;
        case 1: return v21;
        case 2: return v22;
        default: return 100;
      }
    }
  }


  // assign colors based on data
  void bindToLeds(const bool showSuggestion = true){ 
    leds[v00] = b00 == NONE ? CRGB::Black
                : b00 == RED ? CRGB::Red : CRGB::Blue;
    leds[v01] = b01 == NONE ? CRGB::Black
                : b01 == RED ? CRGB::Red : CRGB::Blue;
    leds[v02] = b02 == NONE ? CRGB::Black
                : b02 == RED ? CRGB::Red : CRGB::Blue;

    leds[v10] = b10 == NONE ? CRGB::Black
                : b10 == RED ? CRGB::Red : CRGB::Blue;
    leds[v11] = b11 == NONE ? CRGB::Black
                : b11 == RED ? CRGB::Red : CRGB::Blue;
    leds[v12] = b12 == NONE ? CRGB::Black
                : b12 == RED ? CRGB::Red : CRGB::Blue;

    leds[v20] = b20 == NONE ? CRGB::Black
                : b20 == RED ? CRGB::Red : CRGB::Blue;
    leds[v21] = b21 == NONE ? CRGB::Black
                : b21 == RED ? CRGB::Red : CRGB::Blue;
    leds[v22] = b22 == NONE ? CRGB::Black
                : b22 == RED ? CRGB::Red : CRGB::Blue;

    // bind the suggested led
      if (showSuggestion && suggestion.pos.x < 3 && suggestion.pos.y < 3)
        leds[getLed(suggestion.pos)] = 
          suggestion.color == NONE ? CRGB::Black
          : suggestion.color == RED ? CRGB::Red : CRGB::Blue;

    
  }

  // is the board completely filled?
  bool isFull() {
    for (uint8_t x = 0; x < 3; x++)
      for (uint8_t y = 0; y < 3; y++)
        if (getBox(coord_t(x, y)) == NONE) 
          return false;
    
    return true;
    
  }

  bool gameIsOver() {
    possession_t winner = NONE;
    bool over = false;
    
    if (isFull())
      over = true;


    // winning pattern
    if (b00 != NONE && b00 == b01 && b01 == b02) {
      winner = b00;
      over = true;
    } else if (b00 != NONE && b00 == b10 && b10 == b20) {
      winner = b00;
      over = true;
    } else if (b20 != NONE && b20 == b21 && b21 == b22) {
      winner = b20;
      over = true;
    } else if (b02 != NONE && b02 == b12 && b12 == b22) {
      winner = b02;
      over = true;
    } else if (b00 != NONE && b00 == b11 && b11 == b22) {
      winner = b00;
      over = true;
    } else if (b02 != NONE && b02 == b11 && b11 == b20) {
      winner = b02;
      over = true;
    } else if (b01 != NONE && b01 == b11 && b11 == b21) {
      winner = b01;
      over = true;
    } else if (b10 != NONE && b10 == b11 && b11 == b12) {
      winner = b10;
      over = true;
    }

    if (over && winner == NONE) {
      resetGame();
      return false;
    } else {
      
    }
     
  }

  void resetGame(){
    // clear board
    b00 = b10 = b20 =
    b01 = b11 = b21 =
    b02 = b12 = b22 = NONE;

    
    
  }

  void keepSuggestion() {
    if (gameIsOver()) {}
  }
  
};


#endif
