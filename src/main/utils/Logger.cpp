//
// Created by 郑虎 on 2016-02-09.
//

#include <iostream>
#include <cstdlib>
#include <cassert>
#include "Logger.h"

typedef char* CharBuffer;

LogLevel::CharBufferEntity *LogLevel::charBufferEntity = new LogLevel::CharBufferEntity();
int LogLevel::MAX_LENGTH = 10;
CharBuffer LogLevel::charBuffer = LogLevel::charBufferEntity->get();

int Logger::count = 0;
std::map<int, Logger &> Logger::loggers = {};
std::size_t Logger::defaultLogBufferSize = 50;
std::time_t Logger::currentTime_ = std::time(nullptr);
//void Logger::log(std::string message, LogLevel level) {
//  std::cout << "[Logger id: " << id
//      << ", " << level.toString() << "]: "
//      << message << std::endl;
//}

void Logger::bufferedLog(LogLevel::VALUES level, std::string &message) {

}

void Logger::flushInRange(LogLevel::VALUES infimum, LogLevel::VALUES supremum) {

}

void Logger::flushAbove(LogLevel::VALUES infimum) {

}

void Logger::flushBelow(LogLevel::VALUES supremum) {

}

Logger::Logger() : logBufferSize(defaultLogBufferSize) {
  id = Logger::count;
  while (Logger::loggers.find(id) != Logger::loggers.end()) {
    id += 1;
  }
  std::cout << "Logger with id " <<id << " created!" << std::endl;
  Logger::loggers.insert(
      std::pair<int, Logger &>(id, *this));
//  Logger::loggers[id] = *this;
  Logger::count += 1;
  logBuffer_ = reinterpret_cast<CharBuffer>
  (sizeof(char) * logBufferSize); // TODO: what does this mean?
}

Logger::~Logger() {
  this->flush();
  Logger::loggers.erase(id);
}

void Logger::flush() {
  std::cout << logBufferStream_.str();
  logBufferStream_.str(std::string());
  logBufferStream_.clear();
}

std::time_t Logger::getCurrentTime() {
  currentTime_ = std::time(nullptr);
  return currentTime_;
}

std::string Logger::additionalInfo(LogLevel::VALUES level) {
  /**
   * The opening part.
   */
  std::stringstream sstream;
  sstream << "[Logger id: " << id
      << ", " << LogLevel::toString(level);
  /**
   * Insert time.
   */
  getCurrentTime();
  std::string str(std::asctime(std::localtime(&currentTime_)));
  assert(str.size() > 0);
  str.pop_back();
  sstream << ", " << str;
  /**
   * The closing part.
   */
  sstream << "]: ";
  return sstream.str();
}
