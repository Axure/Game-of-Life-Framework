//
// Created by 郑虎 on 2016-02-06.
//

#include "Game.h"

/**
 *
 */
Buffer<STATE, 2> Game::defaultBoard_ =
    BufferFactory::createBuffer<STATE, Game::defaultWidth, Game::defaultHeight>(
        {
          {STATE::ALIVE, STATE::ALIVE, STATE::ALIVE, STATE::ALIVE, STATE::DEAD},
          {STATE::DEAD, STATE::DEAD, STATE::DEAD, STATE::DEAD, STATE::DEAD, STATE::DEAD}
        });

/**
 * TODO: do we need dependency injection for, like, the timer and the logger?
 * @method run
 * Start the game.
 */
void Game::run() {
  /**
   * Set the state.
   */

  /**
   * Get the screen.
   */
  this->pCurses_->fitSize();

  /**
   * Register the key handlers.
   */

  /**
   * Register the sound.
   */

  /**
   * Loop, and render.
   */

  /**
   * Run the logic.
   * TODO: we really need a dispatcher between the model and the view. We also need to pass back events to the model. The dispatcher should be this game itself or a separate one? No, no, no, the events are already registered in the handlers I'm afraid...
   * TODO: complex event handling like RxJS.
   */
  on_ = true;

  pWorkingThread_ = std::make_shared<std::thread>([&]() {
    pCurses_->run();
    do {
      /**
       * Evolve?
       */
      render_();
      evolve_();
      pDelayer_->delay();

    } while (on_);
  });

//  std::thread wow([&] () {
//    pCurses_->run();
//  });

  pWorkingThread_->join();
//  wow.join();
}

void Game::init() {

}

/**
 * Constructor of the game.
 * @param width
 */
Game::Game(size_t width, size_t height, double frequency, Buffer<STATE, 2> initialBoard) :
    width_(width), height_(height), frequency_(frequency),
    board_(initialBoard), neighborCount_({width, height}),
    pLogger_(LoggerFactory::getSingletonLogger()),
    pDelayer_(new Delayer(frequency_))
{
  refreshNeighborCount_();
}

/**
 * TODO: how to ensure that something would never return to the visible part of the screen?
 * @method evolve_  A protected method, which makes the game evolve one step further.
 */
void Game::evolve_() {
  /**
   * Rules:
   *   1. With fewer than 2 neighbors -> die
   *   2. With 2 or 3 neighbors -> live
   *   3. With more than 3 neighbors -> die
   *   4. With exactly 3 neighbors -> revive
   */
  for (int i = 0; i < width_; ++i) {
    for (int j = 0; j < height_; ++j) {
      switch (neighborCount_.get(i, j)) {
        case 0:
        case 1:
          board_.set(STATE::DEAD, i, j);
          break;
        case 2:

          break;
        case 3:
          board_.set(STATE::ALIVE, i, j);
          break;
        default:
          board_.set(STATE::DEAD, i, j);
      }
    }
  }
  refreshNeighborCount_();
}

/**
 * Better separate this into a standalone model part.
 */
void Game::refreshNeighborCount_() {
  neighborCount_.fill(0);
  for (int i = 0; i < width_; ++i) {
    for (int j = 0; j < height_; ++j) {
      if (board_.get(i, j) == STATE::ALIVE) {
        neighborCount_.headlessTransform([](int x) {
          return x + 1;
        }, i, j);
      }
    }
  }
}

void Game::stop() {
  /**
   * Set the state to off.
   */
  on_ = false;
  /**
   * Detach/shutdown the screen.
   */

  /**
   * Freeze the model.
   */

}

/**
 * Get all the dialogs, and render the from the bottom up.
 */
void Game::render_() {
  for (auto i = 0; i < width_; ++i) {
    for (auto j = 0; j < height_; ++j) {
      char c;
      switch (this->board_.get(i, j)) {
        case STATE::ALIVE:
          c = '*';
          break;
        case STATE::DEAD:
          c = ' ';
      }
      pCurses_->putCharAt(i, j, c);
    }
  }
  /**
   * Calculate every pixel.
   */
  for (auto &dialog: dialogs_) { // TODO: each dialog should have its own rendering method.
    /**
     * Write to screen. (Actually to its buffer.)
     */

    /**
     * Send finish writing signal? Maybe screen should expose such an interface.
     */
  }
}
