//
// Created by 郑虎 on 2016-02-09.
//

#ifndef GAMEOFLIF_LOGGER_H
#define GAMEOFLIF_LOGGER_H
#include <string>
#include <iostream>
#include <cstdio>
#include <map>
#include <sstream>

struct LogLevel {

  typedef char* CharBuffer;
  CharBuffer charBuffer;

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
    charBuffer = new char[10]();
  }

  ~LogLevel() {
    delete[] charBuffer;
  }

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

  CharBuffer toChars() {
    switch (value) {
      case VALUES::FATAL:
        strcpy(charBuffer, "Fatal");
        break;
      case VALUES::CRITICAL:
        strcpy(charBuffer, "Critical");
        break;
      case VALUES::IMPORTANT:
        strcpy(charBuffer, "Important");
        break;
      case VALUES::INFO:
        strcpy(charBuffer, "Info");
        break;
      case VALUES::DEBUG:
        strcpy(charBuffer, "Debug");
        break;
    }
    return charBuffer;
  }

  bool operator==(VALUES logLevel) {
    return value == logLevel;
  }
};

/**
 * TODO: Add time.
 */
class Logger {
  typedef char* CharBuffer;
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

  template<class T>
  void delayedLog(T message, LogLevel level = LogLevel::VALUES::INFO) {
    logBufferStream_ << "[Logger id: " << id
        << ", " << level.toString() << "]: "
        << message << std::endl;
  }

  template<unsigned int length, class ...Types>
  void logf(LogLevel level,
            const char (&format)[length], Types &&...args) {
    printf("[Logger id: %d, %s]: ", id, level.toChars());
    printf(format, args...); // TODO: Concatenate these two sentences to ensure atomicity.
  };

  template<unsigned int length, class ...Types>
  void delayedLogf(LogLevel level,
            const char (&format)[length], Types &&...args) {
    printf("[Logger id: %d, %s]: ", id, level.toChars());
    printf(format, args...); // TODO: Concatenate these two sentences to ensure atomicity.
  };

  void flushInRange(LogLevel infimum, LogLevel supremum);
  void flushAbove(LogLevel infimum);
  void flushBelow(LogLevel supremum);
  void flush();

 private:
  LogLevel outputInfimum, outputSupremum;

  static int count;
  static std::map<int, Logger&> loggers;
  int id;
  std::stringstream logBufferStream_;
  CharBuffer logBuffer_;
  std::size_t logBufferSize;
  std::size_t usedLogBufferSize;
  static std::size_t defaultLogBufferSize;
};

#endif //GAMEOFLIF_LOGGER_H
