#include <iostream>
#include <Game/Game.h>

using namespace std;

int main() {
  cout << "Hello, World!" << endl;

  auto game = Game();
  game.run();

  // Use factory method to avoid duplicate declaration.
  auto buffer = Buffer<char, 3>(std::array<std::size_t, 3>({{1, 2, 3}}));
  auto bufferFactory = BufferFactory();
  auto buffer2 = bufferFactory.createBufferDynamic<char>(std::array<std::size_t, 3>({{1, 2, 3}}));
  Buffer<char, 3> buffer3 = bufferFactory.createBufferDynamic<char, 3>({1, 2, 3});

  cout << buffer3.getTotalSize() << " " << 3;
  auto screen = Curses();
  screen.attach([]() { return false; });
//  for(;;)
//    screen.print("%d, %d\n", screen.getHeight(), screen.getWidth());

  return 0;
}