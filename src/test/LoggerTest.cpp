//
// Created by 郑虎 on 2016-02-21.
//

#include <utils/LoggerFactory.h>

int main() {
  using LOG_LEVEL = LogLevel::VALUES;
  auto logger = LoggerFactory::getSingletonLogger();
  logger->multiLog(LOG_LEVEL::INFO, "haha", 123, ' ');

  return 0;
}