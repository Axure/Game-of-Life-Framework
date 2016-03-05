//
// Created by 郑虎 on 2016-02-09.
//

#include <Game/Game.h>
// TODO: mock test.
int main() {
  auto game = Game();
  auto pCurses = std::make_shared<Curses>();
  game.setCurses(pCurses);
  game.run();

  return 0;
}