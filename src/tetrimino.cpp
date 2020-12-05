#include "tetrimino.h"
#include <random>

tetrimino::tetrimino() {
  pos.first = 0;
  pos.second = 0;
  initTetriminoData();

  initRandom();

  std::uniform_int_distribution<unsigned> distrib(0, data.size() - 1);
  NextTet = distrib(gen);

  nextTetrimino();
}

tetrimino::tetrimino(int x, int y) {
  pos.first = x;
  pos.second = y;
  initTetriminoData();

  initRandom();

  std::uniform_int_distribution<unsigned> distrib(0, data.size() - 1);
  NextTet = distrib(gen);

  nextTetrimino();
}

void tetrimino::initRandom() {

  std::random_device rd;
  std::mt19937::result_type seed =
      rd() ^
      ((std::mt19937::result_type)
           std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::system_clock::now().time_since_epoch())
               .count() +
       (std::mt19937::result_type)
           std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::high_resolution_clock::now().time_since_epoch())
               .count());

#if DEBUG_TETRIMINO
  cout << "seed :" << seed << "\n";
#endif
  gen.seed(seed);
}

void tetrimino::rotate() {
  rotation++;
  rotation = rotation % 4;
}

void tetrimino::rotateClockwise() {
  rotation++;
  rotation = rotation % 4;
}

void tetrimino::rotateAntiClockwise() {
  rotation--;
  if (rotation < 0)
    rotation += 4;
  rotation = rotation % 4;
}

void tetrimino::moveLeft() { pos.first = pos.first - 1; }
void tetrimino::moveRight() { pos.first = pos.first + 1; }
void tetrimino::moveUp() { pos.second = pos.second + 1; }
void tetrimino::moveDown() { pos.second = pos.second - 1; }

int tetrimino::getX() { return pos.first; }
int tetrimino::getY() { return pos.second; }
int tetrimino::getRotation() { return rotation; }

vector<pair<int, int>> tetrimino::getWallKickData(int cur, int next) {
  cur %= 4;
  next %= 4;

  if (cur < 0)
    cur += 4;
  if (next < 0)
    cur += 4;

  vector<pair<int, int>> temp;
  vector<pair<int, int>> fi;
  vector<pair<int, int>> se;
  tetriminoType curTet = (tetriminoType)CurTet;

  switch (curTet) {

  case tet_O:
    fi = OOffsetData[cur];
    se = OOffsetData[next];
    break;

  case tet_I:
    fi = IOffsetData[cur];
    se = IOffsetData[next];
    break;

  default:
    fi = JLSTZOffsetData[cur];
    se = JLSTZOffsetData[next];
    break;
  }

  for (int i = 0; i < fi.size(); i++) {
    temp.push_back({fi[i].first - se[i].first, fi[i].second - se[i].second});
  }

  return temp;
}

void tetrimino::setCoordinates(int x, int y) {
  pos.first = x;
  pos.second = y;
}

void tetrimino::setRotation(int r) { rotation = r % 4; }

int tetrimino::getPieceSize() { return data[CurTet].size; }
int tetrimino::getNextPieceSize() { return data[NextTet].size; }

void tetrimino::nextTetrimino() {
#if 0
  std::uniform_int_distribution<unsigned> distrib(0, data.size() - 1);

  rotation = 0;
  CurTet = NextTet;
  NextTet = distrib(gen);

  col = data[CurTet].col;
#else
  rotation = 0;
  CurTet = NextTet;
  NextTet = *curTetriminoBag;

  col = data[CurTet].col;

  if (++curTetriminoBag == tetriminoBag.end()) {
    shuffle(tetriminoBag.begin(), tetriminoBag.end(), gen);
    curTetriminoBag = tetriminoBag.begin();
  }

#endif
}

color tetrimino::getData(int i, int j) {
  if (i >= 0 and i < data[CurTet].size and j >= 0 and j < data[CurTet].size) {

    int a, b, c;
    vector<bool> temp;
    int size = data[CurTet].size;
    temp = data[CurTet].points;
    color tempCol = data[CurTet].col;

    a = (data[CurTet].size * data[CurTet].size) - 1;                 // 15
    b = (data[CurTet].size * data[CurTet].size) - data[CurTet].size; // 12
    c = data[CurTet].size - 1;                                       // 3

    switch (rotation) {
    case 0:
      return temp[b + i - (j * size)] ? tempCol : blank; // 12
      break;

    case 1:
      return temp[a - (i * size) - j] ? tempCol : blank; // 15
      break;

    case 2:
      return temp[c - i + (j * size)] ? tempCol : blank; // 3
      break;

    case 3:
      return temp[i * size + j] ? tempCol : blank;
      break;

    default:
      return temp[b + i - (j * size)] ? tempCol : blank; // 12
      break;
    }

  } else {
    cout << "piece: out of bound access\n";
    return blank;
  }
}

color tetrimino::getNextPieceData(int i, int j) {
  color tempCol = blank;
  int t = rotation;

  rotation = 0;
  swap(CurTet, NextTet);

  tempCol = getData(i, j);

  swap(CurTet, NextTet);
  rotation = t;

  return tempCol;
}

void tetrimino::initTetriminoData() {
  tetrimino_data d;

  data.resize(totalTetriminos);

  // O
  d.type = tet_O;
  d.size = 3;
  d.col = yellow;
  d.points = {
      false, true,  true,  //
      false, true,  true,  //
      false, false, false, //
  };
  data[tet_O] = d;

  // I
  d.type = tet_I;
  d.size = 5;
  d.col = lightBlue;
  d.points = {
      false, false, false, false, false, //
      false, false, false, false, false, //
      false, true,  true,  true,  true,  //
      false, false, false, false, false, //
      false, false, false, false, false, //
  };
  data[tet_I] = d;

  // T
  d.type = tet_T;
  d.size = 3;
  d.col = purple;
  d.points = {
      false, true,  false, //
      true,  true,  true,  //
      false, false, false, //
  };
  data[tet_T] = d;

  // Z
  d.type = tet_Z;
  d.size = 3;
  d.col = red;
  d.points = {
      true,  true,  false, //
      false, true,  true,  //
      false, false, false, //
  };
  data[tet_Z] = d;

  // S
  d.type = tet_S;
  d.size = 3;
  d.col = green;
  d.points = {
      false, true,  true,  //
      true,  true,  false, //
      false, false, false, //
  };
  data[tet_S] = d;

  // J
  d.type = tet_J;
  d.size = 3;
  d.col = darkBlue;
  d.points = {
      true,  false, false, //
      true,  true,  true,  //
      false, false, false, //
  };
  data[tet_J] = d;

  // L
  d.type = tet_L;
  d.size = 3;
  d.col = orange;
  d.points = {
      false, false, true,  //
      true,  true,  true,  //
      false, false, false, //
  };
  data[tet_L] = d;

  // init tetriminoBag
  for (int i = 0; i < data.size(); i++) {
    tetriminoBag.push_back(i);
  }
  // shuffle tetriminoBag
  shuffle(tetriminoBag.begin(), tetriminoBag.end(), gen);

  curTetriminoBag = tetriminoBag.begin();
}
