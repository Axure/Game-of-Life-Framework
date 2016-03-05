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
#include "../utils/LoggerFactory.h"

using std::begin;
using std::end;
using SizeType = std::size_t;

constexpr SizeType operator
""

_sz(unsigned long long n) { return n; }

template<class T, SizeType dimension>
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

//template<class T>
//class DataStructure {
// public:
//  template<class U>
//  virtual DataStructure<U> map(const DataStructure &dataStructure,
//                               std::function<U(T)> function) = 0;
//};

/**
 * TODO: how to iterate through the buffer?
 * TODO: indefinitely nested loop in C++.
 */
template<class T, SizeType dimension>
class Buffer: public BufferBase<T> {
 public:

  using SizeArrayType = std::array<SizeType, dimension>;

  Buffer() = delete;

//  Buffer(const typename MultiInitializerList<T, dimension>::type &multiList) {
//    auto iterator = multiList.begin();
//    this->fromInitializerList(multiList);
//    initMemory();
//  }

  /**
   * The copy constructor
   */
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

  /**
   * The move constructor
   */
  Buffer(Buffer &&buffer) {
    this->selfOwned = buffer.selfOwned;
    this->memory_ = buffer.memory_;
    this->sizes_ = buffer.sizes_;
    buffer.memory_ = nullptr;
    buffer.selfOwned = false;
  }

  /**
   *
   */
  Buffer(SizeArrayType &&sizes) :
      sizes_(sizes) {
    this->sizes_ = std::forward<SizeArrayType>(sizes);
    initMemory();
  }

  /**
   *
   */
  Buffer(const SizeType *sizes) :
      sizes_({{sizes}}) {
    initMemory();
  }

  /**
   * TODO: figure out why this constructor does not work.
   */
//  Buffer(const size_t_ *sizes, T value):
//      sizes_({{sizes}}){
//    initMemory();
//    this->fill(value);
//  }

//  template<int ...sizes>
//  Buffer(const typename MultiInitializerList<T, sizeof...(sizes)>::type &multiList):
//      Buffer(std::array<size_t_, dimension>({sizes...})){
//    static_assert(sizeof...(sizes) == dimension, "Sizes should match!");
////    this->sizes_ = std::forward<std::array<size_t_, dimension>>(
////        sizes);
//    initMemory();
//    this->fromInitializerList(multiList);
//  };

  /**
   *
   */
  Buffer(Buffer<T, dimension + 1> &parentBuffer, SizeType index) {
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

  /**
   * The destructor of the multi-dimensional buffer class.
   * If the buffer owns itself, we iterate over it and destroy all objects it points to.
   * If the buffer does not own itself, we simply do nothing.
   */
  ~Buffer() {
    if (this->selfOwned) {
      auto size = getTotalSize();
      auto pointer = this->memory_;
      for (auto i = 0; i < size; ++i) {
        (pointer + i)->~T();
      }
      delete[] this->memory_;
    }
  };

  /**
   * The subscript operator for the multi-dimensional buffer class.
   */
  Buffer<T, dimension - 1> operator[](SizeType index) {
    /**
     * Simply call the constructor from a higher dimensional buffer of the buffer of lower dimension.
     */
    return std::forward<Buffer<T, dimension - 1>>(Buffer<T, dimension - 1>(*this, index));
  }

  /**
   * Initialize the memory with the default constructor of the class T.
   */
  void initMemory() {
    this->memory_ = new T[getTotalSize()]();
    this->selfOwned = true;
  }

  /**
   * Get the total size of the buffer.
   */
  SizeType getTotalSize() {
    return Buffer::totalSize_(sizes_);
  }

  /**
   * Initialize the buffer from an initializer list.
   */
  Buffer &fromInitializerList(const typename MultiInitializerList<T, dimension>::type &multiList) {
    auto iterator = multiList.begin();
    for (SizeType i = 0; i < sizes_[0]; ++i) {
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
    for (SizeType i = 0; i < sizes_[0]; ++i) {
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

  /**
   *
   */
  template<typename FirstInt, typename ...RestInt>
  inline T &get(FirstInt firstIndex, RestInt ...restIndex) {
    static_assert(sizeof...(RestInt) == dimension - 1,
                  "The number of indices provided is incorrect!");
    static_assert(std::is_convertible<FirstInt, SizeType>::value, "?");
    assert(this->sizes_[0] > firstIndex);
    assert(firstIndex >= 0);
    return this->operator[](static_cast<SizeType >(firstIndex))
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
  inline Buffer &set(T &value, FirstInt firstIndex, RestInt ...restIndex) {
    static_assert(sizeof...(RestInt) == dimension - 1,
                  "The number of indices provided is incorrect!");
    static_assert(std::is_convertible<FirstInt, SizeType>::value, "?");
    assert(this->sizes_[0] > firstIndex);
    assert(firstIndex >= 0);
    this->operator[](static_cast<SizeType >(firstIndex))
//        .get<RestInt...>(restIndex...);
        .set(value, restIndex...);
    return *this;
  }

    template<typename FirstInt, typename ...RestInt>
  inline Buffer &set(T &&value, FirstInt firstIndex, RestInt ...restIndex) {
    static_assert(sizeof...(RestInt) == dimension - 1,
                  "The number of indices provided is incorrect!");
    static_assert(std::is_convertible<FirstInt, SizeType>::value, "?");
    assert(this->sizes_[0] > firstIndex);
    assert(firstIndex >= 0);
    this->operator[](static_cast<SizeType >(firstIndex))
//        .get<RestInt...>(restIndex...);
        .set(value, restIndex...);
    return *this;
  }

  /**
   * Headless set the value, whether the position is valid or not.
   * Returns true if the position is valid, false otherwise.
   */
  template<typename FirstInt, typename ...RestInt>
  inline bool headlessSet(T &value, FirstInt firstIndex, RestInt ...restIndex) {
    static_assert(sizeof...(RestInt) == dimension - 1,
                  "The number of indices provided is incorrect!");
    static_assert(std::is_convertible<FirstInt, SizeType>::value, "?");
    if (this->sizes_[0] > firstIndex || firstIndex < 0) {
      return 0;
    } else {
      return this->operator[](static_cast<SizeType >(firstIndex))
          .headlessSet((value), restIndex...);
    }
  }

  /**
   */
  template<typename FirstInt, typename ...RestInt>
  inline bool headlessSet(T &&value, FirstInt firstIndex, RestInt ...restIndex) {
    static_assert(sizeof...(RestInt) == dimension - 1,
                  "The number of indices provided is incorrect!");
    static_assert(std::is_convertible<FirstInt, SizeType>::value, "?");
    if (this->sizes_[0] > firstIndex || firstIndex < 0) {
      return 0;
    } else {
      return this->operator[](static_cast<SizeType >(firstIndex))
          .headlessSet(std::move(value), restIndex...);
    }
  }

  /**
   * Headless transform the value, whether the position is vlaid or not.
   * Returns true if the position is valid, false otherwise.
   */
  template<typename FirstInt, typename ...RestInt>
  inline bool headlessTransform(std::function<T(T)> &&function, FirstInt firstIndex, RestInt ...restIndex) {
    static_assert(sizeof...(RestInt) == dimension - 1,
                  "The number of indices provided is incorrect!");
    static_assert(std::is_convertible<FirstInt, SizeType>::value, "?");
    if (this->sizes_[0] > firstIndex || firstIndex < 0) {
      return false;
    } else {
      return this->operator[](static_cast<SizeType >(firstIndex))
          .headlessTransform(std::forward<std::function<T(T)>>(function), restIndex...);
    }
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

  /**
   *
   */
  void fill(T &&value) {
    auto totalSize = getTotalSize();
    for (int i = 0; i < totalSize; ++i) {
      std::copy(&value, (&value) + 1, this->memory_ + i);
    }
  }

  /**
   *
   */
  void fill(T &value) {
    auto totalSize = getTotalSize();
    for (int i = 0; i < totalSize; ++i) {
      std::copy(&value, (&value) + 1, this->memory_ + i);
    }
  }

  /**
   *
   */
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

  /**
   *
   */
  void rescale(
      typename MultiInitializerList<SizeType, dimension>::type &&newSizes) {
    rescale(SizeArrayType(newSizes));
  }

  /**
   *
   */
  template<class U>
  Buffer<U, dimension> map(std::function<U(T)> function) {

  };

 private:
  std::array<SizeType, dimension> sizes_;

  /**
   *
   */
  static SizeType totalSize_(SizeArrayType sizeArray) {
    return std::accumulate(begin(sizeArray), end(sizeArray),
                           static_cast<size_t >(1),
                           [](SizeType c, SizeType p) -> size_t {
                             return c * p;
                           });
  };

};

template<class T>
class Buffer<T, 0>: public BufferBase<T> {
 public:
  Buffer() = delete;

//  template<typename ...Types>
//  Buffer(Types &&...args) :
//      T(std::forward<Types>(args)...) {
//
//  }

  Buffer(T &value) {
    this->selfOwned = true;
    this->memory_ = new T(value);
  }

  Buffer(T &&value) {
    this->selfOwned = true;
    this->memory_ = new T(value);
  }

  Buffer(const Buffer<T, 1> &parent, int index) {
    this->memory_ = parent.getMemory() + index;
    this->selfOwned = false;
  }

  /**
   * The copy constructor.
   */
  Buffer(Buffer &buffer) {
    this->selfOwned = buffer.selfOwned;
    if (this->selfOwned) {
      this->memory_ = new T(*(buffer.memory_));
    } else {
      this->memory_ = buffer.memory_;
    }
  }

  /**
   * The move constrtuctor.
   */
  Buffer(Buffer &&buffer) {
    this->selfOwned = buffer.selfOwned;
    this->memory_ = buffer.memory_;
    buffer.memory_ = nullptr;
    buffer.selfOwned = false;
  }

  /**
   * Delete the subscript operator.
   */
  T operator[](SizeType index) = delete;

  /**
   * Initialize the element from a 0-dimensional multi-initializer list, i.e. a single variable of type T.
   */
  void fromInitializerList(const typename MultiInitializerList<T, 0>::type &multiList) {
    *(this->memory_) = multiList;
  }

  /**
   * Initialize the element from the default constructor of T.
   */
  void fromDefaultConstructor() {
    new(this->memory_) T();
//    std::copy(&BufferBase<T>::zeroTemplate,
//              &BufferBase<T>::zeroTemplate + 1,
//              this->memory_);
  }

  /**
   * Overload the bracket operator for getter of the value.
   */
  T &operator()() {
    return *(this->memory_);
  }

  /**
   * @return The value stored in the memory.
   */
  T &get() noexcept {
    return *(this->memory_);
  }

  void set(const T &value_) {
    new(this->memory_) T(value_);
  }

  void set(const T &&value_) {
    new(this->memory_) T(value_);
  }

  bool headlessSet(T &value) {
    this->set(value);
    return true;
  }

  bool headlessSet(T &&value) {
    this->set(value);
    return true;
  }

  bool headlessTransform(std::function<T(T)> &&function) {
    auto value = *this->memory_;
    *this->memory_ = function(value);
    return true;
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
  /**
   *
   */
  template<class T, SizeType dimension>
  static Buffer<T, dimension> createBufferDynamic(std::array<SizeType, dimension> &&sizes) {
    return Buffer<T, dimension>(std::forward<std::array<SizeType, dimension>>(sizes));
  }

  /**
   * Use a dynamic list of sizes to create the buffer.
   * @tparam T The type.
   * @tparam dimension The dimension the buffer.
   * @arg sizeList The list of sizes.
   *
   */
  template<class T, SizeType dimension>
  static Buffer<T, dimension> createBufferDynamic(const SizeType *sizeList) {
    return Buffer<T, dimension>(std::array<SizeType, dimension>({sizeList}));
  }

  /**
   * @tparam T The type.
   * @tparam sizes The size list of the buffer.
   * @return The buffer created.
   */
  template<class T, int ...sizes>
  static Buffer<T, sizeof...(sizes)> createBuffer() {
    constexpr auto length = sizeof...(sizes);
    return Buffer<T, length>(std::array<SizeType, length>({sizes...}));
  };

  /**
   * Use a static list of sizes to create the buffer.
   */
  template<class T, int ...sizes>
  static Buffer<T, sizeof...(sizes)> createBuffer(const typename MultiInitializerList<T,
                                                                                      sizeof...(sizes)>::type &multiList) {
    constexpr auto length = sizeof...(sizes);
    return Buffer<T, length>(std::array<SizeType, length>({sizes...}))
        .fromInitializerList(multiList);
  };

  /**
   * @tparam T
   * @tparam dimension
   * @arg sizes
   * @arg multiList
   */
  template<class T, SizeType dimension>
  static Buffer<T, dimension> createBuffer(
      std::array<SizeType, dimension> &&sizes,
      typename MultiInitializerList<T, dimension>::type multiList) {

  };
 private:

};

#endif //GAMEOFLIF_BUFFER_H
