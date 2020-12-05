#ifndef BOARD_H
#define BOARD_H

#include <algorithm>
#include <iostream>
#include <vector>

#include "color.h"

#include <vector>

using namespace std;

class board {
public:
  board() {
    bo.resize(h);
    for (auto &a : bo)
      a.resize(w);
    clearBoard();
  }
  ~board() {}

  void drawAt(int x, int y, color col);
  color get(int x, int y);
  void clearBoard();
  int updateBoard();
  bool isGameOver();

  // const int l = (460 / 5) + 1, w = (300 / 5) + 1;
  const int pixelSize = 30;
  const int h = 20, w = 10;
  const int pixelHeight = h * pixelSize, pixelWidth = w * pixelSize;

private:
  vector<vector<color>> bo;
};

#endif
