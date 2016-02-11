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
  logger->log(buffer0[0].get());

  auto bufferm1 = bufferFactory.createBuffer<int, 2>({1, 1});
  bufferm1.fromInitializerList({{1}});
  logger->log(bufferm1[0][0].get());

  auto bufferm2 = bufferFactory.createBuffer<int, 2>({2, 3});
  bufferm2.fromInitializerList({{1, 2, 3}, {4, 5, 6}});
  logger->log(bufferm2[0][0].get());
  logger->log(bufferm2[0][1].get());
  logger->log(bufferm2[0][2].get());
  logger->log(bufferm2[1][0].get());
  logger->log(bufferm2[1][1].get());
  logger->log(bufferm2[1][2].get());
  (bufferm2[0][0]).set(7);
  logger->log(bufferm2[0][0].get());

  auto bufferm3 = bufferFactory.createBuffer<int, 2>({2, 3});
  bufferm3.fromInitializerList({{1, 2 }});
  logger->log(bufferm3[0][0].get());
  logger->log(bufferm3[0][1].get());
  logger->log(bufferm3[0][2].get());
  logger->log(bufferm3[1][0].get());
  logger->log(bufferm3[1][1].get());
  logger->log(bufferm3[1][2].get());

  auto bufferm4 = bufferFactory.createBuffer<int, 2, 3, 2>();
  bufferm4.fromInitializerList({{{1}}});
  logger->log(bufferm4.getTotalSize());

  return 0;
}