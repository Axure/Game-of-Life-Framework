//
// Created by 郑虎 on 2016-02-06.
//

#ifndef GAMEOFLIF_SCREEN_H
#define GAMEOFLIF_SCREEN_H

#include <type_traits>
#include <memory>
#include <tuple>
#include <functional>

#include "Buffer.h"

class Screen {
 public:
  Screen();
  void attach(std::function<bool()> ifContinue);
  void reRender();
  void detach();
  void fitSize();
  ~Screen();

  template<unsigned int length, class ...Types>
  void print(const char (&format)[length], Types &&...args);

  template<unsigned int length, class ...Types>
  void printAt(size_t x, size_t y, const char (&format)[length], Types...content);

  static std::tuple<int, int> getTerminalSize();
 protected:
//  std::shared_ptr<char> buffer;
  std::shared_ptr<Buffer<char, 2>> buffer;
  std::size_t width, height;
  std::size_t maxWidth = 50, maxHeight = 50;
  std::size_t oldWidth = 0, oldHeight = 0;
 public:
  std::size_t getWidth() const {
    return width;
  }

  std::size_t getHeight() const {
    return height;
  }
};

#endif //GAMEOFLIF_SCREEN_H
