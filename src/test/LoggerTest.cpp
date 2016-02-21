//
// Created by 郑虎 on 2016-02-21.
//

#include <utils/LoggerFactory.h>

int main() {
  using LOG_LEVEL = LogLevel::VALUES;
  auto logger = LoggerFactory::getSingletonLogger();
  logger->multiLog(LOG_LEVEL::INFO, "haha", 123, ' ');
  logger->logf(LOG_LEVEL::INFO, "%s", "haha");
  logger->logf(LOG_LEVEL::DEBUG, "%s, %d + %d", "haha", 5, 6);
  return 0;
}