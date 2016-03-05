//
// Created by 郑虎 on 2016-02-06.
//

#ifndef GAMEOFLIF_GAME_H
#define GAMEOFLIF_GAME_H

#include <vector>
#include <unordered_map>
#include <set>
#include "../Engine/Curses.h"
#include "Dialog.h"
#include "../utils/Timer.h"

enum class STATE {
  ALIVE, DEAD
};

/**
 * TODO: infinite scrolling and moving? Infinite canvas?
 * TODO: Just like "Passengers" the game?
 *
 * @class Game
 *
 * The class representing a game.
 *
 * TODO: central message dispatcher, and relationship design refinement.
 * TODO: logging can be turned on in screen mode.
 */
class Game {
 public:
  Game(std::size_t width = Game::defaultWidth,
       std::size_t height = Game::defaultHeight,
       double frequency = Game::defaultFrequency,
       Buffer<STATE, 2> initialBoard = defaultBoard_);

  void run();
  void stop();

 /**
  * Dependency injection.
  */
 public:
  void setCurses(std::shared_ptr<Curses> pCurses) {
    this->pCurses_ = pCurses;
    /**
     *
     */
    pCurses_->attach([&] {
      pDelayer_->delay();
      return true;
    });
  }

 protected:
  /**
   * Make the model evolve one step further.
   * TODO: probably it should not be here.
   */
  void evolve_();
  void init(); // TODO: ??
  /**
   * Configurations of the board.
   */
  std::size_t width_, height_;
  double frequency_;
  /**
   * The board.
   */
  Buffer<STATE, 2> board_;
  /**
   *
   */
  static Buffer<STATE, 2> defaultBoard_;
  static constexpr std::size_t defaultWidth = 10, defaultHeight = 10;
  static constexpr double defaultFrequency = 1;
  /**
   *
   */
  Buffer<int, 2> neighborCount_;
  void refreshNeighborCount_();
  /**
   *
   */
  bool on_;
  /**
   * The curses interface.
   */
  std::shared_ptr<Curses> pCurses_;
  /**
   * Threads.
   */
  std::shared_ptr<std::thread> pWorkingThread_;
  /**
   * Dependency injection.
   */
  std::shared_ptr<Logger> pLogger_;
  std::shared_ptr<Delayer> pDelayer_;
  /**
   * Handlers of the keys.
   * KeyHandler: key -> event, which is void.
   * The handler receives a key, then check and fire the event.
   */
  std::unordered_map<int, std::function<void(int)>> keyHandlers_;
  /**
   * The dialogs of the content. The game should function as an application (in Windows API?).
   */
  std::set<Dialog, Dialog::Compare> dialogs_;
  /**
   * Render one frame of the contents provided.
   */
  void render_();
};

#endif //GAMEOFLIF_GAME_H
