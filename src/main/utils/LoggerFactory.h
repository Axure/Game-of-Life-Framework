//
// Created by 郑虎 on 2016-02-09.
//

#ifndef GAMEOFLIF_LOGGERFACTORY_H
#define GAMEOFLIF_LOGGERFACTORY_H
#include "Logger.h"
#include <memory>

class LoggerFactory {
 public:
  static std::shared_ptr<Logger>  getSingletonLogger();
 private:
  static std::shared_ptr<Logger> pLogger_;
};

#endif //GAMEOFLIF_LOGGERFACTORY_H
