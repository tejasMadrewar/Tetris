#pragma once

#include "SDL_Helper.h"

#include "board.h"
#include "color.h"
#include "tetrimino.h"

#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#define GHOST_PIECE 1

using namespace std;

typedef struct s_gameInput {
  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;
  bool rotateClockwise = false;
  bool rotateAntiClockwise = false;
  bool changeTetrimino = false;
  bool pause = true;
  bool stop = false;
} gameInput;

class game {
public:
  game();
  ~game();
  void run();

private:
  void update();
  void redrawBoard();
  void drawPixel(int x, int y, color col);
  void drawTetrimino(tetrimino &t);
  void drawNextTetrimino(tetrimino &t);
  void drawGhostPiece(tetrimino &t);
  void writeToBoard(tetrimino &t);

  void moveToStart(tetrimino &t);
  bool checkCollision(tetrimino &t);
  void checkMoveUp();
  void checkMoveDown();
  void checkMoveRight();
  void checkMoveLeft();
  void checkRotateClockwise();
  void checkRotateAntiClockwise();
  bool SuperRotationSystem(bool clockwise);

  void pauseBanner();

  tetrimino currentPiece;

  int stop = 0;
  int pause = 0;
  int ds = 1000;
  int fpsRate = 60;

  SDL_Rect rect;
  board b;
  SDL_Helper *s;

  gameInput input;
  uint32_t currentTicks = 0, previousTicks = 0, deltaTicks = 0;

  // score
  int GameScore = 0;
  int HighGameScore = 0;

  // theme
  SDL_Color CYellow{254, 203, 0, 255}, CLightBlue{0, 159, 218, 255},
      CPurple{149, 45, 152, 255}, COrange{255, 121, 0, 255},
      CDarkBlue{0, 101, 189, 255}, CGreen{105, 190, 40, 255},
      CRed{237, 41, 57, 255}, CEmpty{20, 20, 20, 255};

  // Font Color
  SDL_Color fontColor{100, 100, 100, 255};

  map<color, SDL_Color> colorToSDL_Color = {
      {yellow, CYellow}, {lightBlue, CLightBlue}, {purple, CPurple},
      {orange, COrange}, {darkBlue, CDarkBlue},   {green, CGreen},
      {red, CRed},       {blank, CEmpty},
  };
};
