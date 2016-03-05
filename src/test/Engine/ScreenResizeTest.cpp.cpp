//
// Created by 郑虎 on 2016-03-05.
//

//
// Created by 郑虎 on 2016-02-09.
//

#include <iostream>
#include <thread>
#include <future>
#include <utils/LoggerFactory.h>
#include "Engine/Curses.h"
#include "utils/Timer.h"
using namespace std::literals;

int main() {
//  print();
  auto screen = Curses();
  int x, y;
  Delayer delayer(1.1);
  auto logger = LoggerFactory::getSingletonLogger();
  logger->delayedLog("Before!");
  screen.attach([&delayer] {
    delayer.delay();
    return false;
  });

  std::thread renderingThread([&] {
    screen.run();
  });
//  std::thread settingThread([&] {
//    screen.fillWithChar('1');
//  });
  renderingThread.join();

  std::cout << "After!" << std::endl;
  logger->delayedLog("After!");
//  settingThread.join();
  logger->flush();
  return 0;
}