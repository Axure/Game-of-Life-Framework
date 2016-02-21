//
// Created by 郑虎 on 2016-02-06.
//

#ifndef GAMEOFLIF_BUFFER_H
#define GAMEOFLIF_BUFFER_H

#include <array>
#include <cassert>
#include <memory>
#include <numeric>
#include <cstdlib>
#include <type_traits>
#include "utils/LoggerFactory.h"

using std::begin;
using std::end;
using size_t_ = std::size_t;

//constexpr size_t_ operator "" _sz(unsigned long long n) { return n; }

template<class T, size_t_ dimension>
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
//  static T zeroTemplate;
 public:
  virtual ~BufferBase() { };
};

//template<class T>
//T BufferBase<T>::zeroTemplate = T();

template<class T, size_t_ dimension>
class Buffer: public BufferBase<T> {
 public:

  using SizeArrayType = std::array<size_t_, dimension>;

  Buffer() = delete;

//  Buffer(const typename MultiInitializerList<T, dimension>::type &multiList) {
//    auto iterator = multiList.begin();
//    this->fromInitializerList(multiList);
//    initMemory();
//  }

  Buffer(Buffer &buffer) {
    this->selfOwned = buffer.selfOwned;
    this->sizes_ = buffer.sizes_;
    if (this->selfOwned) {
      auto totalSize = getTotalSize();
      this->memory_ = reinterpret_cast<T *>(malloc(totalSize * sizeof(T)));
      for (int i = 0; i < totalSize; ++i) {
        new(this->memory_ + i) T(*(buffer.memory_ + i));
      }
    } else {
      this->memory_ = buffer.memory_;
    }
  }

  Buffer(Buffer &&buffer) {
    this->selfOwned = buffer.selfOwned;
    this->memory_ = buffer.memory_;
    this->sizes_ = buffer.sizes_;
    buffer.memory_ = nullptr;
    buffer.selfOwned = false;
  }

  Buffer(SizeArrayType &&sizes) {
//    static_assert(sizeof...(Types) == dimension, "Buffer initialized with in compatible number of sizes!");
    this->sizes_ = std::forward<SizeArrayType>(sizes);
    initMemory();
  }

//  template<int ...sizes>
//  Buffer(const typename MultiInitializerList<T, sizeof...(sizes)>::type &multiList):
//      Buffer(std::array<size_t_, dimension>({sizes...})){
//    static_assert(sizeof...(sizes) == dimension, "Sizes should match!");
////    this->sizes_ = std::forward<std::array<size_t_, dimension>>(
////        sizes);
//    initMemory();
//    this->fromInitializerList(multiList);
//  };

  Buffer(Buffer<T, dimension + 1> &parentBuffer, size_t_ index) {
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
      /**
       * TODO: write a move and a copy constructor to make this work.
       */
      auto size = getTotalSize();
      auto pointer = this->memory_;
      for(auto i = 0; i < size; ++i) {
        (pointer + i)->~T();
      }
      delete[] this->memory_;
    }
  };

  Buffer<T, dimension - 1> operator[](size_t_ index) {
    /**
     * Reassign the size.
     */

    /**
     * Set the pointer.
     */
    return std::forward<Buffer<T, dimension - 1>>(Buffer<T, dimension - 1>(*this, index));
//    Buffer<T, 0> result;
//    result.selfOwned = false;
//    result.memory_ = this->getMemory() + index;
  }

  void initMemory() {
    this->memory_ = new T[getTotalSize()]();
    this->selfOwned = true;
  }

  size_t_ getTotalSize() {
    return Buffer::totalSize_(sizes_);
  }

  Buffer fromInitializerList(const typename MultiInitializerList<T, dimension>::type &multiList) {
    auto iterator = multiList.begin();
    for (size_t_ i = 0; i < sizes_[0]; ++i) {
      if (iterator != multiList.end()) {
        this->operator[](i).fromInitializerList(*iterator);
        iterator++;
      } else {
        this->operator[](i).fromDefaultConstructor();
      }

    }
    return *this;
  }

  void fromDefaultConstructor() {
    for (size_t_ i = 0; i < sizes_[0]; ++i) {
      this->operator[](i).fromDefaultConstructor();
    }
  }

  T *getMemory() const {
    return this->memory_;
  }

  SizeArrayType &getSizes() {
    return this->sizes_;
  };

//  template<size_t_ firstIndex, size_t_ ...restIndexes>
//  inline T get() {
//    static_assert(sizeof...(restIndexes) == dimension - 1, "?");
//    return this->operator[](firstIndex).get<restIndexes...>();
//  }

  template<typename FirstInt, typename ...RestInt>
  inline T get(FirstInt firstIndex, RestInt ...restIndex) {
    static_assert(sizeof...(RestInt) == dimension - 1,
                  "The number of indices provided is incorrect!");
    static_assert(std::is_convertible<FirstInt, size_t_>::value, "?");
    assert(this->sizes_[0] > firstIndex);
    return this->operator[](static_cast<size_t_ >(firstIndex))
//        .get<RestInt...>(restIndex...);
        .get(restIndex...);
  }

  /**
   * @method set
   *   Note that the value is set as the first argument, in compliance with variadic argument list.
   * @param value The value.
   * @param firstIndex The first index of the variable to be set.
   * @param restIndex The rest of the indices.
   *
   *
   */
  template<typename FirstInt, typename ...RestInt>
  inline Buffer &set(T value, FirstInt firstIndex, RestInt ...restIndex) {
    static_assert(sizeof...(RestInt) == dimension - 1,
                  "The number of indices provided is incorrect!");
    static_assert(std::is_convertible<FirstInt, size_t_>::value, "?");
    assert(this->sizes_[0] > firstIndex);
    this->operator[](static_cast<size_t_ >(firstIndex))
//        .get<RestInt...>(restIndex...);
        .set(value, restIndex...);
    return *this;
  }

//  template<size_t_ firstIndex, size_t_ ...restIndexes>
//  inline T set() {
//    static_assert(sizeof...(restIndexes) == dimension - 1, "?");
//    return this->operator[](firstIndex).get<restIndexes...>();
//  }
//
//  template<size_t_ firstIndex, size_t_ ...restIndexes>
//  inline void bindEventListener() {
//    static_assert(sizeof...(restIndexes) == dimension - 1, "?");
//  }

  void fill(T &&value) {
    auto totalSize = getTotalSize();
    for (int i = 0; i < totalSize; ++i) {
      std::copy(&value, (&value) + 1, this->memory_ + i);
    }
  }

  void fill(T &value) {
    auto totalSize = getTotalSize();
    for (int i = 0; i < totalSize; ++i) {
      std::copy(&value, (&value) + 1, this->memory_ + i);
    }
  }

  void rescale(SizeArrayType &&newSizes) {
    auto newTotalSize = Buffer::totalSize_(newSizes);
    // TODO: consider thread safety here...
    if (newTotalSize > this->getTotalSize()) {
      auto logger = LoggerFactory::getSingletonLogger();
      logger->delayedLog("Rescaled!");
      delete[] this->memory_;
      this->memory_ = new T[newTotalSize]();
    }
    this->sizes_ = newSizes;
  }

  void rescale(
      typename MultiInitializerList<size_t_, dimension>::type &&newSizes) {
    rescale(SizeArrayType(newSizes));
  }

 private:
  std::array<size_t_, dimension> sizes_;

  static size_t_ totalSize_(SizeArrayType sizeArray) {
    return std::accumulate(begin(sizeArray), end(sizeArray),
                           static_cast<size_t >(1),
                           [](size_t_ c, size_t_ p) -> size_t {
                             return c * p;
                           });
  };

};

template<class T>
class Buffer<T, 0>: public BufferBase<T> {
 public:
  Buffer() : T() {

  }

//  template<typename ...Types>
//  Buffer(Types &&...args) :
//      T(std::forward<Types>(args)...) {
//
//  }

  Buffer(const Buffer<T, 1> &parent, int index) {
    this->memory_ = parent.getMemory() + index;
    this->selfOwned = false;
  }

  Buffer(Buffer &buffer) {
    this->selfOwned = buffer.selfOwned;
    if (this->selfOwned) {
      this->memory_ = new T(*(buffer.memory_));
    } else {
      this->memory_ = buffer.memory_;
    }
  }

  Buffer(Buffer &&buffer) {
    this->selfOwned = buffer.selfOwned;
    this->memory_ = buffer.memory_;
    buffer.memory_ = nullptr;
    buffer.selfOwned = false;
  }

  T operator[](size_t_ index) = delete;

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

  T get() noexcept {
    return *(this->memory_);
  }

  void set(const T &value_) {
    *(this->memory_) = value_;
  }

  ~Buffer() noexcept {
    if (this->selfOwned) {
      this->memory_->~T();
      delete (this->memory_);
    }
  }

};

class BufferFactory {

 public:
  template<class T, size_t_ dimension>
  static Buffer<T, dimension> createBuffer(std::array<size_t_, dimension> &&sizes) {
    return Buffer<T, dimension>(std::forward<std::array<size_t_, dimension>>(sizes));
  }

  template<class T, size_t_ dimension>
  static Buffer<T, dimension> createBuffer(const size_t_ *sizeList) {
    return Buffer<T, dimension>(std::array<size_t_, dimension>({sizeList}));
  }

  template<class T, int ...sizes>
  static Buffer<T, sizeof...(sizes)> createBuffer() {
    constexpr auto length = sizeof...(sizes);
    return Buffer<T, length>(std::array<size_t_, length>({sizes...}));
  };

  template<class T, int ...sizes>
  static Buffer<T, sizeof...(sizes)> createBuffer(const typename MultiInitializerList<T,
                                                                                      sizeof...(sizes)>::type &multiList) {
    constexpr auto length = sizeof...(sizes);
    return Buffer<T, length>(std::array<size_t_, length>({sizes...}))
        .fromInitializerList(multiList);
  };

  template<class T, size_t_ dimension>
  static Buffer<T, dimension> createBuffer(
      std::array<size_t_, dimension> &&sizes,
      typename MultiInitializerList<T, dimension>::type multiList) {

  };
 private:

};

#endif //GAMEOFLIF_BUFFER_H
