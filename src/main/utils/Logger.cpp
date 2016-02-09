//
// Created by 郑虎 on 2016-02-09.
//

#include <iostream>
#include "Logger.h"

int Logger::count = 0;

//void Logger::log(std::string message, LogLevel level) {
//  std::cout << "[Logger id: " << id
//      << ", " << level.toString() << "]: "
//      << message << std::endl;
//}

void Logger::bufferedLog(std::string message, LogLevel level) {

}

void Logger::flushInRange(LogLevel infimum, LogLevel supremum) {

}

void Logger::flushAbove(LogLevel infimum) {

}

void Logger::flushBelow(LogLevel supremum) {

}

Logger::Logger() {
  id = Logger::count;
  Logger::count += 1;
}

Logger::~Logger() {

}
