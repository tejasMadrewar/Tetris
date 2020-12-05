#include "game.h"
#include <string>

game::game() {
  s = new SDL_Helper(b.pixelWidth + b.pixelSize * 9, b.pixelHeight, "Tetris");
  s->clear();
  s->openFont("clacon.ttf", 30);
  moveToStart(currentPiece);
}

game::~game() { delete s; }

void game::run() {

  SDL_Event e;

  while (!input.stop) {

    // keyboard input
    while (SDL_PollEvent(&e)) {

      if (e.type == SDL_QUIT) {
        input.stop = true;
      } else if (e.type == SDL_KEYDOWN) {

        switch (e.key.keysym.sym) {
        case SDLK_RIGHT:
          input.right = true;
          break;

        case SDLK_LEFT:
          input.left = true;
          break;

        case SDLK_DOWN:
          input.down = true;
          break;

          ///        case SDLK_UP:
          ///          input.up = true;
          ///          break;
          ///
          ///        case SDLK_c:
          ///          input.changeTetrimino = true;
          ///          break;
          ///
          ///        case SDLK_x:
          ///          input.rotateClockwise = true;
          ///          break;

        case SDLK_UP:
          input.rotateClockwise = true;
          break;

        case SDLK_z:
          input.rotateAntiClockwise = true;
          break;

        case SDLK_ESCAPE:
          input.pause = not input.pause;
          break;
        }
      }
    }

    if (!input.pause) {
      currentTicks = SDL_GetTicks();
      deltaTicks = currentTicks - previousTicks;
      if (deltaTicks > 1000) {
        previousTicks = currentTicks;
        checkMoveDown();
      }
    }

    update();
  }
}

void game::drawPixel(int x, int y, color col) {
  SDL_Rect temp = (SDL_Rect){0, 0, b.pixelSize, b.pixelSize};

  temp.x = x * temp.w;
  temp.y = (b.h - 1 - y) * temp.h;

  SDL_Color c;
  c = colorToSDL_Color[col];

  int border = 1;
  temp.x += border;
  temp.y += border;
  temp.w -= (2 * border);
  temp.h -= (2 * border);

  s->drawRectFill(temp, c);
}

void game::drawTetrimino(tetrimino &t) {

  color cc = blank;
  for (int i = 0; i < t.getPieceSize(); i++) {
    for (int j = 0; j < t.getPieceSize(); j++) {
      cc = t.getData(i, j);
      if (cc == blank)
        continue;
      drawPixel(t.getX() + i, t.getY() + j, cc);
    }
  }

  // draw ghost piece
#if GHOST_PIECE
  drawGhostPiece(t);
#endif
  drawNextTetrimino(t);
}

void game::drawNextTetrimino(tetrimino &t) {

  color cc = blank;
  for (int i = 0; i < t.getNextPieceSize(); i++) {
    for (int j = 0; j < t.getNextPieceSize(); j++) {
      cc = t.getNextPieceData(i, j);
      if (cc == blank)
        continue;
      drawPixel(b.w + i + 2, (b.h / 2) + j, cc);
    }
  }
}

void game::drawGhostPiece(tetrimino &t) {
  // draw ghost piece
  color col = t.getColor();
  SDL_Color lCol = colorToSDL_Color[col];

  int y = t.getY();
  int border = 1;

  // reach bottom
  while (checkCollision(t)) {
    t.moveDown();
  }

  for (int i = 0; i < t.getPieceSize(); i++) {
    for (int j = 0; j < t.getPieceSize(); j++) {
      color c = t.getData(i, j);
      if (c == blank)
        continue;

      SDL_Rect temp = (SDL_Rect){0, 0, b.pixelSize, b.pixelSize};
      temp.x = (t.getX() + i) * temp.w;
      temp.y = (b.h - 2 - (t.getY() + j)) * temp.h;

      int border = 1;
      temp.x += border;
      temp.y += border;
      temp.w -= (2 * border);
      temp.h -= (2 * border);

      // s->drawRectFill(temp, lCol);
      s->drawRectDraw(temp, lCol);
    }
  }

  // restore piece
  t.setCoordinates(t.getX(), y);
}

void game::writeToBoard(tetrimino &t) {
  // draw tetrimino to board
  for (int i = 0; i < t.getPieceSize(); i++) {
    for (int j = 0; j < t.getPieceSize(); j++) {
      if (t.getData(i, j) != blank) {
        int x = t.getX() + i;
        int y = t.getY() + j;
        b.drawAt(x, y, t.getData(i, j));
      }
    }
  }
  t.nextTetrimino();
  moveToStart(t);
  // update board and score
  GameScore += b.updateBoard();
  HighGameScore = max(HighGameScore, GameScore);

  // check game over
  if (b.isGameOver()) {
    cout << "Game Over\n";
    GameScore = 0;
    b.clearBoard();
    currentPiece.nextTetrimino();
  }
}

void game::moveToStart(tetrimino &t) {
  currentPiece.setCoordinates((b.w / 2) - (t.getPieceSize() / 2) - 1, b.h);
}

void game::checkRotateClockwise() { SuperRotationSystem(true); }

void game::checkRotateAntiClockwise() { SuperRotationSystem(false); }

bool game::SuperRotationSystem(bool clockwise) {

  int x = currentPiece.getX();
  int y = currentPiece.getY();
  int cur = currentPiece.getRotation();
  int next = 0;
  bool rotateSuccess = false;
  vector<pair<int, int>> offsets;

  if (clockwise) {
    next = cur + 1;
  } else {
    next = cur - 1;
    if (next == -1) {
      next = 3;
    }
  }

  offsets = currentPiece.getWallKickData(cur, next);
  currentPiece.setRotation(next);

  // check each kick offset
  for (auto &offset : offsets) {
    currentPiece.setCoordinates(x + offset.first, y + offset.second);
    if (checkCollision(currentPiece)) {
      rotateSuccess = true;
    }

    if (rotateSuccess)
      break;
  }

  if (!rotateSuccess) {
    // cout << "roation not possible\n";
    currentPiece.setRotation(cur);
    currentPiece.setCoordinates(x, y);
  }

  return rotateSuccess;
}

void game::checkMoveUp() {
  currentPiece.moveUp();

  if (!checkCollision(currentPiece)) {
    currentPiece.moveDown();
  }
}

void game::checkMoveDown() {
  currentPiece.moveDown();

  if (!checkCollision(currentPiece)) {
    currentPiece.moveUp();
    writeToBoard(currentPiece);
  }
}

void game::checkMoveLeft() {
  currentPiece.moveLeft();

  if (!checkCollision(currentPiece)) {
    currentPiece.moveRight();
  }
}

void game::checkMoveRight() {
  currentPiece.moveRight();

  if (!checkCollision(currentPiece)) {
    currentPiece.moveLeft();
  }
}

bool game::checkCollision(tetrimino &t) {
  // false if collision, true if not collision

  for (int i = 0; i < t.getPieceSize(); i++) {
    for (int j = 0; j < t.getPieceSize(); j++) {
      if (t.getData(i, j) != blank) {
        int x = t.getX() + i;
        int y = t.getY() + j;

        // out of board
        if (x >= 0 and x < b.w and y >= 0) { // and y < b.h) {
        } else {
          return false;
        }

        // collision with other
        if (b.get(x, y) != blank) {
          return false;
        }
      }
    }
  }
  return true;
}

void game::redrawBoard() {

  if (b.isGameOver()) {
    cout << "Game Over\n";
    GameScore = 0;
    b.clearBoard();
    currentPiece.nextTetrimino();
  }

  // draw board
  for (int i = 0; i < b.w; i++) {
    for (int j = 0; j < b.h; j++) {
      drawPixel(i, j, b.get(i, j));
    }
  }

  // draw tetrimino
  drawTetrimino(currentPiece);

  // draw text
  std::string scoreText = "Score: " + std::to_string(GameScore);
  // s->drawText(scoreText, 5, 10, fontColor);
  s->drawText(scoreText, b.pixelSize * (b.w + 4), 10, fontColor);
  scoreText = "High Score: " + std::to_string(HighGameScore);
  s->drawText(scoreText, b.pixelSize * (b.w + 4), 40, fontColor);
  scoreText = "Next Piece";
  s->drawText(scoreText, b.pixelSize * (b.w + 4), b.pixelSize * (b.w / 2),
              fontColor);
  //  scoreText = "FPS Ticks: " + std::to_string(FpsDeltaTicks);
  //  s->drawText(scoreText, 5, 70, fontColor);
}

void game::update() {

  if (!input.pause) {
    s->clear();

    if (input.left) {
      this->checkMoveLeft();
      input.left = false;
    }

    if (input.right) {
      this->checkMoveRight();
      input.right = false;
    }

    if (input.rotateClockwise) {
      this->checkRotateClockwise();
      input.rotateClockwise = false;
    }

    if (input.rotateAntiClockwise) {
      this->checkRotateAntiClockwise();
      input.rotateAntiClockwise = false;
    }

    if (input.down) {
      this->checkMoveDown();
      input.down = false;
    }

    if (input.up) {
      this->checkMoveUp();
      input.up = false;
    }

    if (input.changeTetrimino) {
      currentPiece.nextTetrimino();
      input.changeTetrimino = false;
    }

    redrawBoard();
    s->renderPresent();

  } else {

    this->pauseBanner();
    input.up = false;
    input.down = false;
    input.left = false;
    input.right = false;
    input.changeTetrimino = false;
    input.rotateClockwise = false;
    input.rotateAntiClockwise = false;
  }
}

void game::pauseBanner() {
  SDL_Color White{255, 255, 255, 0};
  int row = 0;

  s->drawText("PRESS ESC TO START/PAUSE,", (s->getWidth()) / 2,
              s->getHeight() / 2 + row * 30, White);
  row++;

  // s->drawText("ARROW KEYS TO MOVE,", (s->getWidth()) / 2,
  s->drawText("LEFT AND RIGHT KEYS TO SHIFT,", (s->getWidth()) / 2,
              s->getHeight() / 2 + row * 30, White);
  row++;

  s->drawText("DOWN TO SOFT DROP,", (s->getWidth()) / 2,
              s->getHeight() / 2 + row * 30, White);
  row++;

  s->drawText("UP TO ROTATE CLOCKWISE,", (s->getWidth()) / 2,
              s->getHeight() / 2 + row * 30, White);
  row++;

  s->drawText("Z TO ROTATE COUNTER CLOCKWISE.", (s->getWidth()) / 2,
              s->getHeight() / 2 + row * 30, White);
  row++;
  s->renderPresent();
}
