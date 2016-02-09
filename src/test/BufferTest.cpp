//
// Created by 郑虎 on 2016-02-09.
//
#include <iostream>
#include "Buffer.h"
#include "utils/LoggerFactory.h"

int main() {
  auto buffer = Buffer<char, 3>(std::array<std::size_t, 3>({{1, 2, 3}}));
  auto bufferFactory = BufferFactory();
  auto buffer2 = bufferFactory.createBuffer<char>(std::array<std::size_t, 3>({{1, 2, 3}}));
  Buffer<char, 3> buffer3 = bufferFactory.createBuffer<char, 3>({1, 2, 3});

  std::cout << buffer3.getTotalSize() << std::endl;

  auto loggerFactory = LoggerFactory();
  auto logger = loggerFactory.getSingletonLogger();
  logger->log("Hi!");

  auto buffer0 = bufferFactory.createBuffer<char, 1>({2});
  buffer0.fromInitializerList({'1', '2'});
  logger->log(buffer0[0]());
  return 0;
}