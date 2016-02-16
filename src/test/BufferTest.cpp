//
// Created by 郑虎 on 2016-02-09.
//
#include <iostream>
#include "Buffer.h"
#include "utils/LoggerFactory.h"

int main() {
  auto buffer = Buffer<char, 3>(std::array<std::size_t, 3>({{1, 2, 3}}));
//  auto buffer2 = BufferFactory::createBuffer<char>(std::array<std::size_t, 3>({{1, 2, 3}}));
  auto buffer3 = BufferFactory::createBuffer<char, 1, 2, 3>();

  std::cout << buffer3.getTotalSize() << std::endl;

  auto loggerFactory = LoggerFactory();
  auto logger = loggerFactory.getSingletonLogger();
  logger->log("Hi!");

  auto buffer0 = BufferFactory::createBuffer<char, 2>();
  buffer0.fromInitializerList({'1', '2'});
  logger->log(buffer0[0].get());
  logger->log(buffer0[1].get());

  auto bufferm1 = BufferFactory::createBuffer<int, 1, 1>();
  bufferm1.fromInitializerList({{1}});
  logger->log(bufferm1[0][0].get());

  auto bufferm2 = BufferFactory::createBuffer<int, 2, 3>();
  bufferm2.fromInitializerList({{1, 2, 3}, {4, 5, 6}});
  logger->log(bufferm2[0][0].get());
  logger->log(bufferm2[0][1].get());
  logger->log(bufferm2[0][2].get());
  logger->log(bufferm2[1][0].get());
  logger->log(bufferm2[1][1].get());
  logger->log(bufferm2[1][2].get());
  (bufferm2[0][0]).set(7);
  logger->log(bufferm2[0][0].get());

  auto bufferm3 = BufferFactory::createBuffer<int, 2, 3>();
  bufferm3.fromInitializerList({{1, 2}});
  logger->log(bufferm3[0][0].get());
  logger->log(bufferm3[0][1].get());
  logger->log(bufferm3[0][2].get());
  logger->log(bufferm3[1][0].get());
  logger->log(bufferm3[1][1].get());
  logger->log(bufferm3[1][2].get());

  auto bufferm4 = BufferFactory::createBuffer<int, 2, 3, 2>();
  bufferm4.fromInitializerList({{{1}}});
  logger->log(bufferm4.getTotalSize());

  auto bufferm5 = BufferFactory::createBuffer<int, 3, 3>({
                                                             {1, 2, 3},
                                                             {4, 5, 6},
                                                             {7, 8, 9}
                                                         });
  logger->log(bufferm5[0][0].get());
  logger->log(bufferm5[0][1].get());
  logger->log(bufferm5[0][2].get());
  logger->log(bufferm5[1][0].get());
  logger->log(bufferm5[1][1].get());
  logger->log(bufferm5[1][2].get());
  logger->log(bufferm5[2][0].get());
  logger->log(bufferm5[2][1].get());
  logger->log(bufferm5[2][2].get());
  logger->log(bufferm5.get(2, 2));
  bufferm5.set(178, 2, 2);
  logger->log(bufferm5.get(2, 2));
//  logger->log(bufferm5.get(2, 3, 4));
//  logger->log(bufferm5.get(2, buffer));

//  Buffer<int, 2> bufferm6<3, 3>({{}});
  auto bufferm7 = BufferFactory::createBuffer<char, 2, 2>({{'2', '3'}, {'4', '5'}});
  logger->log(bufferm7[0][0].get());
  logger->log(bufferm7[0][1].get());
  logger->log(bufferm7[1][0].get());
  logger->log(bufferm7[1][1].get());
  bufferm7.fill('1');
  logger->log(bufferm7[0][0].get());
  logger->log(bufferm7[0][1].get());
  logger->log(bufferm7[1][0].get());
  logger->log(bufferm7[1][1].get());

  return 0;
}