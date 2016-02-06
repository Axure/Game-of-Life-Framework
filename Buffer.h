//
// Created by 郑虎 on 2016-02-06.
//

#ifndef GAMEOFLIF_BUFFER_H
#define GAMEOFLIF_BUFFER_H

#include <array>
#include <memory>
#include <numeric>
#include <cstdlib>

using std::begin;
using std::end;

constexpr std::size_t operator
""

_sz(unsigned long long n) { return n; }

template<class T, std::size_t dimension>
class Buffer {
 public:
  Buffer() = delete;

  Buffer(std::array<std::size_t, dimension> &&sizes) {
//    static_assert(sizeof...(Types) == dimension, "Buffer initialized with in compatible number of sizes!");
    sizes_ = std::forward<std::array<std::size_t, dimension>>(sizes);
  }

  Buffer(Buffer<T, dimension + 1> parentBuffer) {

  }

  Buffer<T, dimension - 1> operator[](std::size_t index) {

  }

  void initMemory() {
    this->memory_ = reinterpret_cast<T *>(malloc(sizeof(T) * getTotalSize()));
  }

  std::size_t getTotalSize() {
    return std::accumulate(begin(sizes_), end(sizes_), 1, [](std::size_t c, std::size_t p) {
      return c * p;
    });
  }

 private:
  std::array<std::size_t, dimension> sizes_;
  T *memory_;

  void reScale() {

  }
};

template<class T>
class Buffer<T, 0> {
 public:
  Buffer() {

  }

  T operator[](std::size_t index) {

  }

};

class BufferFactory {
 private:

 public:
  template<class T, std::size_t dimension>
  Buffer<T, dimension> createBuffer(std::array<std::size_t, dimension> &&sizes) {
    return Buffer<T, dimension>(std::forward<std::array<std::size_t, dimension>>(sizes));
  }

  template<class T, std::size_t dimension>
  Buffer<T, dimension> createBuffer(const std::size_t *sizeList) {
    return Buffer<T, dimension>(std::array<std::size_t, dimension>({sizeList}));
  }

};

#endif //GAMEOFLIF_BUFFER_H
