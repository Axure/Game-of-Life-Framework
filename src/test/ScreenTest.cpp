//
// Created by 郑虎 on 2016-02-09.
//

#include <iostream>
#include <thread>
#include <future>
#include <utils/LoggerFactory.h>
#include "Screen.h"
#include "utils/Timer.h"
using namespace std::literals;

int main() {
//  print();
  auto screen = Screen();
  int x, y;
  Delayer delayer(1.1);
  auto logger = LoggerFactory::getSingletonLogger();
  logger->delayedLog("Before!");
  screen.attach([&delayer] {
    delayer.delay();
    return true;
  });

  std::thread renderingThread([&] {
    screen.run();
  });
  std::thread settingThread([&] {
    screen.fill('1');
  });

  std::cout << "After!" << std::endl;
  logger->delayedLog("After!");
  renderingThread.join();
  settingThread.join();
  logger->flush();
  return 0;
}