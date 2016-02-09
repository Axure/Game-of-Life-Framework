#include <iostream>
#include "../../src/main/Game.h"



using namespace std;

int main() {
  cout << "Hello, World!" << endl;

  auto game = Game();
  game.start();

  // Use factory method to avoid duplicate declaration.
  auto buffer = Buffer<char, 3>(std::array<std::size_t, 3>({{1, 2, 3}}));
  auto bufferFactory = BufferFactory();
  auto buffer2 = bufferFactory.createBuffer<char>(std::array<std::size_t, 3>({{1, 2, 3}}));
  Buffer<char, 3> buffer3 = bufferFactory.createBuffer<char, 3>({1, 2, 3});

  cout << buffer3.getTotalSize() << " " << 3;
  auto screen = Screen();
  screen.attach();
//  for(;;)
//    screen.print("%d, %d\n", screen.getHeight(), screen.getWidth());

  return 0;
}