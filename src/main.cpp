#include "game.h"
#include <iostream>

#define RELEASE 0

using namespace std;

#if RELEASE
int WinMain(int argc, char *argv[]) {
#else
int main(int argc, char *argv[]) {
#endif

  game g;
  g.run();

  return 0;
}
