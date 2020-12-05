#pragma once

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

#include "color.h"

#define DEBUG_TETRIMINO 0

using namespace std;

enum tetriminoType {
  tet_O,
  tet_I,
  tet_T,
  tet_Z,
  tet_S,
  tet_J,
  tet_L,
  totalTetriminos
};

struct tetrimino_data {
  tetriminoType type;
  int size = 0;
  color col = blank;
  vector<bool> points;
};

// https://tetris.fandom.com/wiki/SRS
class tetrimino {
public:
  tetrimino();
  tetrimino(int x, int y);
  void rotate();
  void rotateClockwise();
  void rotateAntiClockwise();

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();

  void nextTetrimino();

  color getData(int i, int j);
  color getNextPieceData(int i, int j);
  color getColor() { return col; }
  vector<pair<int, int>> getWallKickData(int cur, int next);
  int getX();
  int getY();
  int getRotation();
  void setCoordinates(int x, int y);
  void setRotation(int r);
  int getPieceSize();
  int getNextPieceSize();

protected:
  void initTetriminoData();
  void initRandom();
  vector<tetrimino_data> data;
  vector<int> tetriminoBag;
  vector<int>::iterator curTetriminoBag;

  std::mt19937 gen;

private:
  int CurTet = 0;
  int NextTet = 0;
  int rotation = 0;
  std::pair<int, int> pos;
  color col = blank;
  // wall kick data
  // https://tetris.wiki/Super_Rotation_System#Wall_Kicks
  const vector<vector<pair<int, int>>> JLSTZOffsetData = {
      {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
      {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
      {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
      {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
  };

  const vector<vector<pair<int, int>>> IOffsetData = {
      {{0, 0}, {-1, 0}, {2, 0}, {-1, 0}, {2, 0}},
      {{-1, 0}, {0, 0}, {0, 0}, {0, 1}, {0, -2}},
      {{-1, 1}, {1, 1}, {-2, 1}, {1, 0}, {-2, 0}},
      {{0, 1}, {0, 1}, {0, 1}, {0, -1}, {0, 2}},
  };

  const vector<vector<pair<int, int>>> OOffsetData = {
      {{0, 0}},
      {{0, -1}},
      {{-1, -1}},
      {{-1, 0}},
  };
};
