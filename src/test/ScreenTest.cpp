//
// Created by 郑虎 on 2016-02-09.
//

#include <iostream>
#include <thread>
#include "Screen.h"
#include "utils/Timer.h"
using namespace std::literals;

int main() {
//  print();
  auto screen = Screen();
  int x, y;
  Delayer delayer(1.1);
  std::cout << "Before!" << std::endl;
  std::thread renderingThread([&] {
    screen.attach([&delayer] {
      delayer.delay();
      return true;
    });
  });
  std::thread settingThread([&] {
    screen.fill('1');
  });
  std::cout << "After!" << std::endl;
  renderingThread.join();
  settingThread.join();

  return 0;
}