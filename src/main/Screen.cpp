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

using std::size_t;

Screen::Screen() {
  std::array<size_t, 2> sizes({{maxWidth, maxHeight}});
  buffer = std::make_shared<Buffer<char, 2>>(
      std::move(sizes));
}

void Screen::fitSize() {
  std::tie(width, height) = Screen::getTerminalSize();
  if (width > oldHeight * 2 || height > oldHeight * 2) {
//    getmaxyx();
  }
}

void Screen::attach(std::function<bool()> ifContinue) {
  this->fitSize();
  initscr();
  printw("Haha");
  while (ifContinue()) {
    reRender();
  }
}

void Screen::reRender() {
//  for
  clear();
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      printw("%c", (*buffer)[i][j].get());
    }
  }
  refresh();
}

void Screen::detach() {
  endwin();
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
  static struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return std::make_tuple(std::move(w.ws_col), std::move(w.ws_row));
//#endif
  int x, y;
  getmaxyx(stdscr, y, x);
  return std::make_tuple(std::move(x), std::move(y));
}

template<unsigned int length, class ...Types>
void Screen::print(const char (&format)[length], Types &&...args) {
  printw(format, std::forward<Types>(args)...);
  refresh();
};

Screen::~Screen() {

}
