//
// Created by 郑虎 on 2016-02-06.
//

#include "Screen.h"
#ifdef WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

Screen::Screen() {
}

void Screen::fitSize(){
  std::tie(width, height) = Screen::getTerminalSize();
  if (width > oldHeight * 2 || height > oldHeight * 2) {

  }
}

void Screen::attach() {
  this->fitSize();
}

void Screen::reRender() {
  refresh();
}

void Screen::detach() {
  endwin();
}

std::tuple<int, int> Screen::getTerminalSize() {

#ifdef WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  int ret;
  ret = GetConsoleScreenBufferInfo(GetStdHandle( STD_OUTPUT_HANDLE ),&csbi);
  if(ret)
  {
      printf("Console Buffer Width: %d\n", csbi.dwSize.X);
      printf("Console Buffer Height: %d\n", csbi.dwSize.Y);
  }
#else
  static struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return std::make_tuple(std::move(w.ws_col), std::move(w.ws_row));
#endif
}


