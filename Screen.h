//
// Created by 郑虎 on 2016-02-06.
//

#ifndef GAMEOFLIF_SCREEN_H
#define GAMEOFLIF_SCREEN_H

#include <ncurses.h>
#include <type_traits>
#include <memory>
#include <tuple>

#include "Buffer.h"

class Screen {
 public:
  Screen();
  void attach();
  void reRender();
  void detach();
  void fitSize();

  template<unsigned int length, class ...Types>
  void print(const char (&format)[length], Types&& ...args) {
    printw(format, std::forward<Types>(args)...);
    refresh();
  };

  static std::tuple<int, int> getTerminalSize();
 private:
  std::shared_ptr<char> buffer;
  int width, height;
  int oldWidth = 0, oldHeight = 0;
 public:
  int getWidth() const {
    return width;
  }

  int getHeight() const {
    return height;
  }
};

#endif //GAMEOFLIF_SCREEN_H
