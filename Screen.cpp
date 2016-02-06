//
// Created by 郑虎 on 2016-02-06.
//

#include "Screen.h"

Screen::Screen() {

}

void Screen::attach() {
  initscr();
}

void Screen::reRender() {
  refresh();
}

void Screen::detach() {
  endwin();
}