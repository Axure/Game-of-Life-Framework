//
// Created by 郑虎 on 2016-02-09.
//

#ifndef GAMEOFLIF_LOGGER_H
#define GAMEOFLIF_LOGGER_H
#include <string>
#include <iostream>
#include <map>

struct LogLevel {
  enum class VALUES {
    FATAL,
    CRITICAL,
    IMPORTANT,
    INFO,
    DEBUG
  };
  VALUES value = VALUES::INFO;

  LogLevel(VALUES value_ = VALUES::INFO) {
    value = value_;
  }

  ~LogLevel() { }

  std::string toString() {
    switch (value) {
      case VALUES::FATAL:
        return "Fatal";
      case VALUES::CRITICAL:
        return "Critical";
      case VALUES::IMPORTANT:
        return "Important";
      case VALUES::INFO:
        return "Info";
      case VALUES::DEBUG:
        return "Debug";
    }
  }

  bool operator==(VALUES logLevel) {
    return value == logLevel;
  }
};

class Logger {
 public:
  Logger();
  virtual ~Logger();
//  void log(std::string message, LogLevel level = LogLevel::VALUES::INFO);
  void bufferedLog(std::string message, LogLevel level = LogLevel::VALUES::INFO);

  template<class T>
  void log(T message, LogLevel level = LogLevel::VALUES::INFO) {
    std::cout << "[Logger id: " << id
        << ", " << level.toString() << "]: "
        << message << std::endl;
  }

 private:
  LogLevel outputInfimum, outputSupremum;
  void flushInRange(LogLevel infimum, LogLevel supremum);
  void flushAbove(LogLevel infimum);
  void flushBelow(LogLevel supremum);
  static int count;
  static std::map<int, Logger&> loggers;
  int id;
};

#endif //GAMEOFLIF_LOGGER_H
