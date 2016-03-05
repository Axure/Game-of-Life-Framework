//
// Created by 郑虎 on 2016-02-06.
//

#include "Curses.h"
#ifdef WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <ncurses.h>
#endif
#include <iostream>
#include <thread>
#include "../utils/LoggerFactory.h"

using std::size_t;

/**
 * Initialize the screen.
 * Set state `on` to false.
 */
Curses::Curses() : on(false) {
  std::array<size_t, 2> sizes({{maxWidth, maxHeight}});
  oldWidth = maxWidth;
  oldHeight = maxHeight;
  buffer = std::make_shared<Buffer<char, 2>>(
      std::move(sizes));
}

/**
 * Fit the size of the screen to that of the surrounding terminal.
 */
void Curses::fitSize() {
  auto logger = LoggerFactory::getSingletonLogger();
  std::tie(width, height) = Curses::getTerminalSize();
  if (width > oldHeight || height > oldHeight) {
//    getmaxyx();
    buffer->rescale({width, height});
//    logger->logf(LogLevel::VALUES::INFO,
//                 "Rescaled with \{%d, %d\}!", width, height);
    std::stringstream sMessage;
    sMessage << "Rescaled with {"
        << width << ", " << height
        << "}.";
    logger->delayedLog(sMessage.str(), LogLevel::VALUES::INFO);
    oldHeight = height;
    oldWidth = width;
  }
}

/**
 *
 */
void Curses::attach(std::function<bool()> ifContinue) {
//  initscr();
  this->on = true;
  this->ifContinue_ = ifContinue;
  this->fitSize();
//  printw("Haha");

}

/**
 *
 */
void Curses::refresh() {
//  for
  clear();
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      printw("%c", (*buffer).get(i, j));
    }
  }
  refresh();
}

/**
 *
 */
void Curses::detach() {
  stop();
//  endwin();
}

/**
 *
 */
void Curses::stop() {
  this->ifContinue_ = [] { return false; };
  this->on = false;
}

/**
 * @deprecated
 */
template<unsigned int length, class ...Types>
void Curses::printAt(size_t x, size_t y, const char (&format)[length], Types...content) {
  move(x, y);
  printw(format, content...);
}

/**
 * Get the size of the terminal using the ncurses API.
 */
std::tuple<int, int> Curses::getTerminalSize() {

  int x, y;
  getmaxyx(stdscr, y, x);
  if (y < 0 || x < 0) {
    initscr();
    getmaxyx(stdscr, y, x);
    endwin();
  }

  return std::make_tuple(std::move(x), std::move(y));
}

/**
 * @deprecated
 */
template<unsigned int length, class ...Types>
void Curses::print(const char (&format)[length], Types &&...args) {
  printw(format, std::forward<Types>(args)...);
  refresh();
};

/**
 *
 */
Curses::~Curses() {

}

/**
 *
 */
void Curses::fillWithChar(char charToFill) {
//  std::cout << ("Filling with ") << charToFill << std::endl;
  this->buffer->fill(charToFill); // TODO: should `fill` be set as a state? Or just consisting of the various sub-states?
}

/**
 *
 */
void Curses::run() {
  on = false;
/**
   * Note that the first screen must be rendered.
   * The `ifContinue` function is only evaluated after it has beeen rendered once.
   */
  int c;
//  detach();
  auto logger = LoggerFactory::getSingletonLogger();

  std::thread resizeThread([&] {
    int t_width_, t_height;
    assert(true);
    logger->delayedLog("Resizing thread started!");
    buffer->fill('#'); // TODO: only for testing.
    do {
      if (this->autoResize_) {
        getmaxyx(stdscr, t_height, t_width_);
        if (t_height != height || t_width_ != width) {
          height = t_height;
          width = t_width_;
          logger->delayedMultiLog(LogLevel::VALUES::DEBUG,
                                  "Terminal resized! [",
                                  t_width_,
                                  ", ",
                                  t_height, "]");
          buffer->rescale({width, height});
          resize_term(t_height, t_width_);
        }
        buffer->fill('#'); // TODO: only for testing.
      }
      refresh();
      /**
       * For immediate response to the resizing,
       * we use `on` instead of evaluating `ifContinue_()`,
       * which may contain delays.
       */
    } while (on);
  });
  fitSize();
  buffer->fill('#');
//  std::thread keyThread([&] {
//    do {
//      c = wgetch(stdscr);
//      logger->delayedLog(c);
//      detach();
////      if (c == KEY_UP) {
////        continue;
////      }
////      if (c == KEY_DOWN) {
////        detach();
////        break;
////      }
//      /**
//       * For the same reason we use `on` here.
//       * But this loop would only proceed once a key is pressed.
//       */
//    } while (on);
//
//  });

  pRenderingThread_ = std::make_shared<std::thread>([&] {
    do {
      if (this->on) {
        refresh();
      }
//      std::cout << 1 << std::endl;
    } while (this->ifContinue_());
  });

//  keyThread.join();
  pRenderingThread_->join();
  resizeThread.join();

}

/**
 * @method
 * Try to put a character at the given position.
 * It will check if the position is valid.
 * If not, false will be returned. No exception. No interruption.
 * If the position is valid, true will be returned.
 */
bool Curses::putCharAt(size_t x, size_t y, char c) noexcept {
  return buffer->headlessSet(c, x, y);
}