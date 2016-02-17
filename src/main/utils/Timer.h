//
// Created by 郑虎 on 2016-02-16.
//

#ifndef GAMEOFLIF_TIMER_H
#define GAMEOFLIF_TIMER_H

#if __cplusplus < 201103L && (!defined(_MSC_VER) || _MSC_VER < 1700)
#error jrd::Timer class requires C++11 support
#else

#include <functional>
#include <ostream>

class NonCopyable {
 public:
  NonCopyable() { }

  ~NonCopyable() { }

  NonCopyable(const NonCopyable &) = delete;
  NonCopyable &operator=(const NonCopyable &) = delete;
};

class Timer: NonCopyable {
 public:
  Timer(double interval = 1);
  void setToFire(std::function<void()> function);
  void run();
  ~Timer();
 private:
  std::function<void()> thing_;
  double interval;
};

class Delayer {
 public:
  Delayer(double interval = 1);
  void delay();
  ~Delayer();
 private:
  int count;
  double interval;
};

#endif
#endif //GAMEOFLIF_TIMER_H
