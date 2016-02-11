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

template<class T>
class BufferBase {
 protected:
  T *memory_;
  bool selfOwned;
  static T zeroTemplate;
};

template<class T>
T BufferBase<T>::zeroTemplate = T();

template<class T, std::size_t dimension>
class Buffer: public BufferBase<T> {
 public:
  Buffer() = delete;

  Buffer(const typename MultiInitializerList<T, dimension>::type &multiList) {
    auto iterator = multiList.begin();
    this->fromInitializerList(multiList);
    initMemory();
  }

  Buffer(std::array<std::size_t, dimension> &&sizes) {
//    static_assert(sizeof...(Types) == dimension, "Buffer initialized with in compatible number of sizes!");
    this->sizes_ = std::forward<std::array<std::size_t, dimension>>(sizes);
    initMemory();
  }

  Buffer(Buffer<T, dimension + 1> &parentBuffer, std::size_t index) {
    /**
     * Reassign the size.
     */
    auto sizeIterator = this->sizes_.begin();
    sizeIterator++;
    std::copy(
        begin(parentBuffer.getSizes()) + 1,
        end(parentBuffer.getSizes()),
        begin(this->sizes_));
    /**
     * Set the pointer.
     */
    this->memory_ = parentBuffer.getMemory() + index * getTotalSize();
    /**
     * Mark as not self-owned;
     */
    this->selfOwned = false;
  }

  ~Buffer() {
    if (this->selfOwned) {
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
    this->memory_ = new T[getTotalSize()];
    this->selfOwned = true;
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
        this->operator[](i).fromDefaultConstructor();
      }

    }
  }

  void fromDefaultConstructor() {
    for (std::size_t i = 0; i < sizes_[0]; ++i) {
      this->operator[](i).fromDefaultConstructor();
    }
  }

  T *getMemory() const {
    return this->memory_;
  }

  std::array<std::size_t, dimension> &getSizes() {
    return this->sizes_;
  };

 private:
  std::array<std::size_t, dimension> sizes_;
  void reScale() {

  }

};

template<class T>
class Buffer<T, 0>: public BufferBase<T> {
 public:
  Buffer() : T() {

  }

  Buffer(const Buffer<T, 1> &parent, int index) {
    this->memory_ = parent.getMemory() + index;
    this->selfOwned = false;
  }

  T operator[](std::size_t index) = delete;

  void fromInitializerList(const typename MultiInitializerList<T, 0>::type &multiList) {
    *(this->memory_) = multiList;
  }

  void fromDefaultConstructor() {
    new(this->memory_) T();
//    std::copy(&BufferBase<T>::zeroTemplate,
//              &BufferBase<T>::zeroTemplate + 1,
//              this->memory_);
  }

  T operator()() {
    return *(this->memory_);
  }

  T get() {
    return *(this->memory_);
  }

  void set(const T &value_) {
    *(this->memory_) = value_;
  }

  ~Buffer() {
    if (this->selfOwned) {
      delete (this->memory_);
    }
  }

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

  template<class T, int ...sizes>
  Buffer<T, sizeof...(sizes)> createBuffer() {
    constexpr auto length = sizeof...(sizes);
    return Buffer<T, length>(std::array<std::size_t, length>({sizes...}));
  };

  template<class T, std::size_t dimension>
  Buffer<T, dimension> createBuffer(
      std::array<std::size_t, dimension> &&sizes,
      typename MultiInitializerList<T, dimension>::type multiList) {

  };
 private:

};

#endif //GAMEOFLIF_BUFFER_H
