#pragma once

#include <iostream>
#include <string>

#define TTF_ENABLE 1

#include "../third_party/SDL2/include/SDL.h"

#if TTF_ENABLE
#include "../third_party/SDL2/include/SDL_ttf.h"
#endif

#undef main

class SDL_Helper {
public:
  SDL_Helper();
  SDL_Helper(int W, int H, std::string s = "Window Name");
  ~SDL_Helper();
  void clear();
  void drawRectFill(SDL_Rect &rect, SDL_Color &col);
  void drawRectDraw(SDL_Rect &rect, SDL_Color &col);
  void renderPresent() { SDL_RenderPresent(renderer); }
  int getWidth() { return WIDTH; };
  int getHeight() { return HEIGHT; };
#if TTF_ENABLE
  void drawText(std::string text, int x, int y, SDL_Color &col);
  void openFont(std::string fontName, int pSize = 40);
#endif

private:
  void initSDL();

  SDL_Renderer *renderer = nullptr;
  SDL_Window *window = nullptr;
  SDL_Surface *surface = nullptr;
  SDL_Texture *texture = nullptr;
  SDL_Rect tempRect;

#if TTF_ENABLE
  TTF_Font *gFont;
#endif

  int WIDTH = 480;
  int HEIGHT = 320;
  std::string windowName = "Window name";
};
