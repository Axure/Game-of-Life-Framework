//
// Created by 郑虎 on 2016-02-09.
//

#include "Screen.h"

int main() {
//  print();
  auto screen = Screen();
  screen.attach([] { return true; });

  return 0;
}