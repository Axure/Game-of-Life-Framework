//
// Created by 郑虎 on 2016-02-06.
//

#ifndef GAMEOFLIF_GAME_H
#define GAMEOFLIF_GAME_H

#include <vector>
#include "Screen.h"

enum STATE {
  ALIVE, DEAD
};

class Game {
 public:
  Game(std::size_t width = 8,
       std::size_t height = 8,
       double frequency = 1,
       Buffer<STATE, 2> initialBoard = defaultBoard);

  void start();
  void evolve();
 private:
  void init();
//  static const Buffer<STATE, 2> defaultBoard = {{ALIVE, DEAD}};
  std::size_t width, height;
  double frequency;
  Buffer<STATE, 2> board;
  Buffer<int, 2> neighborCount;
  void initNeighborCount();
};

#endif //GAMEOFLIF_GAME_H
