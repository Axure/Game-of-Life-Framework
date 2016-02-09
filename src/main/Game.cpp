//
// Created by 郑虎 on 2016-02-06.
//

#include "Game.h"


void Game::start() {
  auto a = {{ALIVE, DEAD}};
  a;

}

void Game::init() {

}

Game::Game(size_t width, size_t height, double frequency, Board<STATE> initialBoard):
  width(width), height(height), frequency(frequency), board(initialBoard)
{

}


void Game::evolve() {
  /**
   * Rules:
   *   1. With fewer than 2 neighbors -> die
   *   2. With 2 or 3 neighbors -> live
   *   3. With more than 3 neighbors -> die
   *   4. With exactly 3 neighbors -> revive
   */

}

void Game::initNeighborCount() {
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      if (board[i][j] )
    }
  }
}
