//
// Created by 郑虎 on 2016-02-09.
//

#include <iostream>
#include <thread>
#include <utils/LoggerFactory.h>
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
    screen.run();
  });
  std::thread settingThread([&] {
    screen.fill('1');
  });
  std::cout << "After!" << std::endl;
  renderingThread.join();
  settingThread.join();
  auto logger = LoggerFactory::getSingletonLogger();
  logger->flush();
  return 0;
}