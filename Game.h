//
// Created by 郑虎 on 2016-02-06.
//

#ifndef GAMEOFLIF_GAME_H
#define GAMEOFLIF_GAME_H

#include "Screen.h"

class Game {
 public:
  Game();
  Game(float frequency);

  void start();
 private:
  void init();

};

#endif //GAMEOFLIF_GAME_H
