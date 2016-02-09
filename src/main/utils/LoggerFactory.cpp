//
// Created by 郑虎 on 2016-02-09.
//

#include "LoggerFactory.h"

std::shared_ptr<Logger> LoggerFactory::pLogger_ = nullptr;

std::shared_ptr<Logger>LoggerFactory::getSingletonLogger() {
  if (LoggerFactory::pLogger_ == nullptr) {
    LoggerFactory::pLogger_ = std::make_shared<Logger>();
  }
  return LoggerFactory::pLogger_;
}
