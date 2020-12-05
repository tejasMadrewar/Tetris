#include "SDL_Helper.h"

SDL_Helper::SDL_Helper()
    : window(nullptr), renderer(nullptr), windowName("Window Name") {
  initSDL();
}

SDL_Helper::SDL_Helper(int W, int H, std::string s)
    : WIDTH(W), HEIGHT(H), windowName(s) {
  initSDL();
}

SDL_Helper::~SDL_Helper() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  if (texture)
    SDL_DestroyTexture(texture);
  if (surface)
    SDL_FreeSurface(surface);
#if TTF_ENABLE
  TTF_CloseFont(gFont);
  TTF_Quit();
#endif
  SDL_Quit();
}

void SDL_Helper::initSDL() {
  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
  renderer = SDL_CreateRenderer(window, -1, 0);
#if TTF_ENABLE
  TTF_Init();
  gFont = TTF_OpenFont("clacon.ttf", 30);
#endif
  clear();
}

void SDL_Helper::clear() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
}

void SDL_Helper::drawRectFill(SDL_Rect &rect, SDL_Color &col) {
  SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
  SDL_RenderFillRect(renderer, &rect);
}
void SDL_Helper::drawRectDraw(SDL_Rect &rect, SDL_Color &col) {
  SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
  SDL_RenderDrawRect(renderer, &rect);
}

#if TTF_ENABLE
void SDL_Helper::openFont(std::string fontName, int pSize) {
  TTF_CloseFont(gFont);
  gFont = TTF_OpenFont(fontName.c_str(), pSize);
}

void SDL_Helper::drawText(std::string text, int x, int y, SDL_Color &color) {
  surface = TTF_RenderText_Solid(gFont, text.c_str(), color);
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_QueryTexture(texture, NULL, NULL, &tempRect.w, &tempRect.h);
  tempRect.x = x - (tempRect.w / 2);
  tempRect.y = y - (tempRect.h / 2);
  SDL_RenderCopy(renderer, texture, NULL, &tempRect);
  SDL_DestroyTexture(texture);
  texture = nullptr;
  SDL_FreeSurface(surface);
  surface = nullptr;
}
#endif
