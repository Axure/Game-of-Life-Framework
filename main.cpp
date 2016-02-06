#include <iostream>
#include "Game.h"



using namespace std;

int main() {
  cout << "Hello, World!" << endl;

  auto game = Game();
  game.start();

  auto screen = Screen();
  screen.attach();
  for(;;)
    screen.print("%d", 3);

  return 0;
}