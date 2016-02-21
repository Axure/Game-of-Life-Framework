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
#include <ctime>

#ifndef AXUREZ_LOGGER_RELEASE
#define AXUREZ_LOGGER_DEBUG(logger, message) \
logger->delayedLog(message, LogLevel::VALUES::DEBUG);
#else
#define AXUREZ_LOGGER_DEBUG(logger, message)
#endif


struct LogLevel {
  enum class VALUES {
    FATAL,
    CRITICAL,
    IMPORTANT,
    INFO,
    DEBUG,
    COUNT_
  };

  VALUES value;
  typedef char* CharBuffer;

  class CharBufferEntity {
   protected:
    CharBuffer charBuffer;
   public:
    CharBufferEntity () {
      charBuffer = reinterpret_cast<CharBuffer>(
          malloc(sizeof(char) * LogLevel::MAX_LENGTH * (int)(VALUES::COUNT_)));
      for (int i = (int)(VALUES::FATAL);
           i < (int)(VALUES::COUNT_);
           ++i) {
        auto string = LogLevel::toString((VALUES)(i));
        std::copy(string.begin(),
                  string.end(),
                  charBuffer + i * LogLevel::MAX_LENGTH);
      }
    }

    ~CharBufferEntity () {
      delete charBuffer;
    }

    CharBuffer get() {
      return charBuffer;
    }
  };

  static CharBufferEntity* charBufferEntity;
  static int MAX_LENGTH;
  static CharBuffer charBuffer;

  static std::string toString(VALUES value) {
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
      default:
        return "";
    }
  }

  CharBuffer toChars(VALUES value) {
    return LogLevel::charBuffer
        + int(value) * MAX_LENGTH;
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
  void bufferedLog(LogLevel::VALUES level, std::string &message);

  template<class T>
  void log(T &&message, LogLevel::VALUES level = LogLevel::VALUES::INFO) {
    std::cout
        << additionalInfo(level)
        << message
        << std::endl;
  }

  template<class ...Types>
  void multiLog(LogLevel::VALUES level,Types&& ...messages) {
    std::cout
        << additionalInfo(level)
        << buildLogMessage(messages...)
        << std::endl;
  }


  template<class T>
  void delayedLog(T &&message, LogLevel::VALUES level = LogLevel::VALUES::INFO) {
    logBufferStream_
        << additionalInfo(level)
        << message
        << std::endl;
  }

  template<unsigned int length, class ...Types>
  void logf(LogLevel::VALUES level,
            const char (&format)[length], Types &&...args) {
    printf("%s ", additionalInfo(level));
    printf(format, args...); // TODO: Concatenate these two sentences to ensure atomicity.
  };

  template<unsigned int length, class ...Types>
  void delayedLogf(LogLevel::VALUES level,
            const char (&format)[length], Types &&...args) {
    printf("%s ", additionalInfo(level));
    printf(format, args...); // TODO: Concatenate these two sentences to ensure atomicity.
  };

  void flushInRange(LogLevel::VALUES infimum, LogLevel::VALUES supremum);
  void flushAbove(LogLevel::VALUES infimum);
  void flushBelow(LogLevel::VALUES supremum);
  void flush();

 protected:
  template<class ...Types>
  std::string buildLogMessage(Types&&...messages) {
    static_assert(sizeof...(messages) > 0, "Message list cannot be empty!");
    std::stringstream sstream;
    buildLogMessageStream(sstream, messages...);
    return sstream.str();
  }

  template<class FirstType, class ...Types>
  std::stringstream &buildLogMessageStream(std::stringstream &sstream,
                                           FirstType&& firstMessage,
                                           Types&& ...messages) {
    sstream << firstMessage;
    return buildLogMessageStream(sstream, std::forward<Types>(messages)...);
  }

  template<class T>
  std::stringstream &buildLogMessageStream(std::stringstream &sstream, T&message) {
    sstream << message;
    return sstream;
  }

//  template<class T>
//  std::string buildLogMessage(T message) {
//    std::stringstream sstream;
//    sstream << message;
//    return sstream.str();
//  }
//
//  template<>
//  std::string buildLogMessage(std::stringstream sstream) {
//    return sstream.str();
//  }

 private:
  LogLevel outputInfimum, outputSupremum;
  static std::time_t getCurrentTime();
  static std::time_t currentTime_;
  std::string additionalInfo(LogLevel::VALUES);
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
