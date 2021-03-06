//
// Created by 郑虎 on 2016-02-16.
//

#include <chrono>
#include <thread>
#include "LoggerFactory.h"
#include "Timer.h"

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::this_thread::sleep_for;

Timer::Timer(double interval): interval(interval) {

}

void Timer::setToFire(std::function<void()> function) {
  this->thing_ = function;
}

/**
 *
 */
void Timer::run() {
  auto logger = LoggerFactory::getSingletonLogger();
  logger->log("aha");
  const milliseconds realInterval(
      static_cast<int>(interval * 1000));
  std::thread workingThread([this, realInterval, &logger] {
    while(true) {
      this->thing_();
      sleep_for(realInterval);
    }
  });
  workingThread.join();
}

Timer::~Timer() {

}

Delayer::Delayer(double interval): interval(interval), count(0) {

}

void Delayer::delay() {
  const milliseconds realInterval(
      static_cast<int>(interval * 1000));
  count += 1;
  sleep_for(realInterval);
}

Delayer::~Delayer() {

}

//Delayer::Delayer(Delayer &delayer) {
//
//}
//
//Delayer::Delayer(Delayer &&delayer) {
//
//}
