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

//constexpr std::size_t operator "" _sz(unsigned long long n) { return n; }

template<class T, std::size_t dimension>
struct MultiInitializerList {
  using type = std::initializer_list<typename MultiInitializerList<T, dimension - 1>::type>;
};

template<class T>
struct MultiInitializerList<T, 0> {
  using type = T;
};

template<class T, std::size_t dimension>
class Buffer {
 public:
  Buffer() = delete;

  Buffer(const typename MultiInitializerList<T, dimension>::type &multiList) {
    auto iterator = multiList.begin();
    for (std::size_t i = 0; i < sizes_[0]; ++i) {
      if (iterator != multiList.end()) {
        this->operator[](i).fromInitializerList(*iterator);
        iterator++;
      } else {

      }

    }
    selfOwnend = true;
  }

  Buffer(std::array<std::size_t, dimension> &&sizes) {
//    static_assert(sizeof...(Types) == dimension, "Buffer initialized with in compatible number of sizes!");
    sizes_ = std::forward<std::array<std::size_t, dimension>>(sizes);
    memory_ = new T[getTotalSize()];
    selfOwnend = true;
  }

  Buffer(Buffer<T, dimension + 1> &parentBuffer, std::size_t index) {
    /**
     * Reassign the size.
     */
    auto sizeIterator = sizes_.begin();
    sizeIterator++;
    std::copy(
        begin(parentBuffer.getSizes()) + 1,
        end(parentBuffer.getSizes()),
        begin(sizes_));
    /**
     * Set the pointer.
     */
    memory_ = parentBuffer.getMemory() + index * getTotalSize();
    /**
     * Mark as not self-owned;
     */
    selfOwnend = false;
  }

  ~Buffer() {
    if (selfOwnend) {
//      delete memory_;
    }
  };

  Buffer<T, dimension - 1> operator[](std::size_t index) {
    /**
     * Reassign the size.
     */

    /**
     * Set the pointer.
     */
    return std::forward<Buffer<T, dimension - 1>>(Buffer<T, dimension - 1>(*this, index));
  }

  void initMemory() {
    this->memory_ = reinterpret_cast<T *>(malloc(sizeof(T) * getTotalSize()));
  }

  std::size_t getTotalSize() {
    return std::accumulate(begin(sizes_), end(sizes_), 1, [](std::size_t c, std::size_t p) {
      return c * p;
    });
  }

  void fromInitializerList(const typename MultiInitializerList<T, dimension>::type &multiList) {
    auto iterator = multiList.begin();
    for (std::size_t i = 0; i < sizes_[0]; ++i) {
      if (iterator != multiList.end()) {
        this->operator[](i).fromInitializerList(*iterator);
        iterator++;
      } else {

      }

    }
  }

  T *getMemory() const {
    return memory_;
  }

  std::array<std::size_t, dimension> &getSizes() {
    return sizes_;
  };

 private:
  std::array<std::size_t, dimension> sizes_;
  T *memory_;
  bool selfOwnend;

  void reScale() {

  }

};

template<class T>
class Buffer<T, 0> {
 public:
  Buffer() : T() {

  }

  Buffer(const Buffer<T, 1> &parent, int index) {
    value = parent.getMemory() + index;
    selfOwned = false;
  }

  T operator[](std::size_t index) = delete;

  void fromInitializerList(const typename MultiInitializerList<T, 0>::type &multiList) {
    *value = multiList;
  }

  T operator()() {
    return *value;
  }

  T get() {
    return *value;
  }

  void set(const T &value_) {
    *value = value_;
  }

  ~Buffer() {
    if (selfOwned) {
      delete value;
    }
  }

 private:
  T *value;
  bool selfOwned;
};

class BufferFactory {

 public:
  template<class T, std::size_t dimension>
  Buffer<T, dimension> createBuffer(std::array<std::size_t, dimension> &&sizes) {
    return Buffer<T, dimension>(std::forward<std::array<std::size_t, dimension>>(sizes));
  }

  template<class T, std::size_t dimension>
  Buffer<T, dimension> createBuffer(const std::size_t *sizeList) {
    return Buffer<T, dimension>(std::array<std::size_t, dimension>({sizeList}));
  }

//  template<class T, std::size_t dimension>
//  Buffer<T, dimension> createBuffer(typename MultiInitializerList<T, dimension>::type multiList) {
//
//  };
 private:

};

#endif //GAMEOFLIF_BUFFER_H
