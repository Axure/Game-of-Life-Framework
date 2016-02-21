//
// Created by 郑虎 on 2016-02-06.
//

#include "Screen.h"
#ifdef WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <ncurses.h>
#endif
#include <iostream>
#include <thread>
#include "utils/LoggerFactory.h"

using std::size_t;

Screen::Screen() : on(false) {
  std::array<size_t, 2> sizes({{maxWidth, maxHeight}});
  oldWidth = maxWidth;
  oldHeight = maxHeight;
  buffer = std::make_shared<Buffer<char, 2>>(
      std::move(sizes));
}

void Screen::fitSize() {
  auto logger = LoggerFactory::getSingletonLogger();
  std::tie(width, height) = Screen::getTerminalSize();
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

void Screen::attach(std::function<bool()> ifContinue) {
  initscr();
  this->on = true;
  this->ifContinue_ = ifContinue;
  this->fitSize();
//  printw("Haha");

}

void Screen::reRender() {
//  for
  clear();
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      printw("%c", (*buffer).get(i, j));
    }
  }
  refresh();
}

void Screen::detach() {
  stop();
  endwin();
}

void Screen::stop() {
  this->ifContinue_ = [] { return false; };
  this->on = false;

//  pRenderingThread_->stop;
//  delete pRenderingThread_;
}

template<unsigned int length, class ...Types>
void Screen::printAt(size_t x, size_t y, const char (&format)[length], Types...content) {
  move(x, y);
  printw(format, content...);
}

std::tuple<int, int> Screen::getTerminalSize() {

//#ifdef WIN32
//  CONSOLE_SCREEN_BUFFER_INFO csbi;
//  int ret;
//  ret = GetConsoleScreenBufferInfo(GetStdHandle( STD_OUTPUT_HANDLE ),&csbi);
//  if(ret)
//  {
//      printf("Console Buffer Width: %d\n", csbi.dwSize.X);
//      printf("Console Buffer Height: %d\n", csbi.dwSize.Y);
//  }
//#else
//  static struct winsize w;
//  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
//  return std::make_tuple(std::move(w.ws_col), std::move(w.ws_row));
//#endif
  int x, y;
  getmaxyx(stdscr, y, x);
  if (y < 0 || x < 0) {
    initscr();
    getmaxyx(stdscr, y, x);
    endwin();
  }

  return std::make_tuple(std::move(x), std::move(y));
}

template<unsigned int length, class ...Types>
void Screen::print(const char (&format)[length], Types &&...args) {
  printw(format, std::forward<Types>(args)...);
  refresh();
};

Screen::~Screen() {

}

void Screen::fill(char charToFill) {
//  std::cout << ("Filling with ") << charToFill << std::endl;
  this->buffer->fill(charToFill); // TODO: should `fill` be set as a state? Or just consisting of the various sub-states?
}

void Screen::run() {
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
    AXUREZ_LOGGER_DEBUG(logger, "Resizing thread started!");
    do {
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
        buffer->fill('#');
        resize_term(t_height, t_width_);
        reRender();
      }
      /**
       * For immediate response to the resizing,
       * we use `on` instead of evaluating `ifContinue_()`,
       * which may contain delays.
       */
    } while (on);
  });

  std::thread keyThread([&] {
    do {
      c = wgetch(stdscr);
      logger->delayedLog(c);
      detach();
//      if (c == KEY_UP) {
//        continue;
//      }
//      if (c == KEY_DOWN) {
//        detach();
//        break;
//      }
      /**
       * For the same reason we use `on` here.
       * But this loop would only proceed once a key is pressed.
       */
    } while (on);

  });

  pRenderingThread_ = std::make_shared<std::thread>([&] {
    do {
      if (this->on) {
        reRender();
      }
//      std::cout << 1 << std::endl;
    } while (this->ifContinue_());
  });

  keyThread.join();
  pRenderingThread_->join();
  resizeThread.join();

}
