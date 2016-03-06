//
// Created by 郑虎 on 2016-03-06.
//

#include "CursesPp.h"
#ifdef WINCURSES
#else
#include <ncurses.h>
#endif
#include <string.h>

CursesPp::CursesPp() {

}

void CursesPp::attach() {
  ::initscr();
  this->on_ = true;
};

void CursesPp::detach() {
  this->on_ = false;
  ::endwin();
}

void CursesPp::refresh() {
  ::refresh();
}

void CursesPp::putCharAt(LengthType row, LengthType column, char ch) {
  mvaddch(row, column, ch);
}