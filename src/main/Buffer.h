//
// Created by 郑虎 on 2016-02-06.
//

#ifndef GAMEOFLIF_BUFFER_H
#define GAMEOFLIF_BUFFER_H

#include <array>
#include <memory>
#include <numeric>
#include <cstdlib>
#include <type_traits>

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
  static T zeroTemplate;
};

template<class T>
T BufferBase<T>::zeroTemplate = T();

template<class T, size_t_ dimension>
class Buffer: public BufferBase<T> {
 public:
  Buffer() = delete;

//  Buffer(const typename MultiInitializerList<T, dimension>::type &multiList) {
//    auto iterator = multiList.begin();
//    this->fromInitializerList(multiList);
//    initMemory();
//  }

  Buffer(std::array<size_t_, dimension> &&sizes) {
//    static_assert(sizeof...(Types) == dimension, "Buffer initialized with in compatible number of sizes!");
    this->sizes_ = std::forward<std::array<size_t_, dimension>>(sizes);
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
//      delete memory_;
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
  }

  void initMemory() {
    this->memory_ = new T[getTotalSize()];
    this->selfOwned = true;
  }

  size_t_ getTotalSize() {
    return std::accumulate(begin(sizes_), end(sizes_), 1, [](size_t_ c, size_t_ p) {
      return c * p;
    });
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

  std::array<size_t_, dimension> &getSizes() {
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

 private:
  std::array<size_t_, dimension> sizes_;

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

  T get() {
    return *(this->memory_);
  }

  void set(const T &value_) {
    *(this->memory_) = value_;
  }

  ~Buffer() {
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
