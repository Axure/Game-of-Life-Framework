//
// Created by 郑虎 on 2016-02-06.
//

#ifndef GAMEOFLIF_SCREEN_H
#define GAMEOFLIF_SCREEN_H

#include <ncurses.h>
#include <type_traits>

class Screen {

 public:
  Screen();
  void attach();
  void reRender();
  void detach();

  template<unsigned int length, class ...Types>
  void print(const char (&format)[length], Types&& ...args) {
    printw(format, std::forward<Types>(args)...);
    refresh();
  };
 private:

};

#endif //GAMEOFLIF_SCREEN_H
