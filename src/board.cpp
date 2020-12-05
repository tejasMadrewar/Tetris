#include "board.h"

void board::drawAt(int x, int y, color col) {
  if (x >= 0 and x < w and y >= 0 and y < h)
    bo[y][x] = col;
  else {
    // cout << "drawAt out of bound :" << x << ", " << y << "\n";
  }
}

color board::get(int x, int y) {
  if (x >= 0 and x < w and y >= 0 and y < h)
    return bo[y][x];
  else {
    // cout << "get out of bound :" << x << ", " << y << "\n";
  }

  return blank;
}

void board::clearBoard() {
  for (auto &a : bo) {
    fill(a.begin(), a.end(), blank);
  }
}

int board::updateBoard() {
  int CompleteRow = 0;
  int level = 1;
  int score = 0;
  for (int i = 0; i < bo.size(); i++) {

    int count = 0;
    for (int j = 0; j < bo[i].size(); j++) {
      if (bo[i][j] != blank)
        count++;
    }

    if (count == bo[i].size()) { // row is filled
      fill(bo[i].begin(), bo[i].end(), blank);
      rotate(bo.begin() + i, bo.begin() + i + 1, bo.end() - 1);
      i--;
      CompleteRow++;
    }
  }

  // https://tetris.wiki/Scoring#Recent_guideline_compatible_games
  switch (CompleteRow) {
  case 1:
    score = 100 * level;
    break;
  case 2:
    score = 300 * level;
    break;
  case 3:
    score = 500 * level;
    break;
  case 4:
    score = 800 * level;
    break;
  }

  // return CompleteRow * bo[0].size();
  return score;
}

bool board::isGameOver() {
  auto last_row = bo[bo.size() - 1];
  for (auto i : last_row) {
    if (i != blank) {
      return true;
    }
  }
  return false;
}
