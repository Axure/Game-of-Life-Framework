//
// Created by 郑虎 on 2016-02-16.
//

#include<utils/Timer.h>
#include <iostream>
int main() {
  Timer myTimer(1.1);
  myTimer.setToFire([]() {
    std::cout << "1" << std::endl;
  });
  myTimer.run();
  return 0;
}