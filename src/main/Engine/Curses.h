//
// Created by 郑虎 on 2016-02-06.
//

#ifndef GAMEOFLIF_SCREEN_H
#define GAMEOFLIF_SCREEN_H

#define COMPUTED(name, function) \

#define AX_BIND_VARIABLE(var, bound) \
  decltype(var) get##bound() const noexcept {\
    return var;\
  }\
  void set##bound(decltype(var)& value) {\
    this->var = value;\
  }\

#include <type_traits>
#include <memory>
#include <tuple>
#include <functional>
#include <thread>

#include "../DataStructure/Buffer.tcc"

/**
 * TODO: rename it into "Runtime" or something else, which represents the interface of the conceptual world to the real world.
 */
class Curses {
 protected:
  bool autoResize_ = true;
 public:
  /**
   * Types.
   */
  using LengthType = std::size_t;
  using LengthTupleType = std::tuple<LengthType, LengthType>;
  using CoordinateType = LengthType;
  using CoordinateTupleType = LengthTupleType;
 private:
  LengthType offsetW, offsetH;
 public:
//  AX_BIND_VARIABLE(offsetW, offsetX);
//  AX_BIND_VARIABLE(offsetH, offsetY);
  /**
   * The constructor.
   */
  Curses();
  /**
   *
   */
  void attach(std::function<bool()> ifContinue);
  void run();
  void stop();
  void refresh();
  void detach();

  /**
   *
   */
  void fillWithChar(char charToFill);
  /**
   * The destructor
   */
  ~Curses();

  /**
   * Interface related with sizes of the screen itself (not the actual terminal).
   */
  void fitSize();
  LengthTupleType getSize() const noexcept;
  Curses& setSize(LengthTupleType sizes);
  LengthType getWidth() const noexcept;
  LengthType getHeight() const noexcept;
  Curses &setWidth(LengthType width);
  Curses &getHeight(LengthType height);


  /**
   * Generation one of the API.
   */
  template<unsigned int length, class ...Types>
  void print(const char (&format)[length], Types &&...args);

  template<unsigned int length, class ...Types>
  void printAt(size_t x, size_t y, const char (&format)[length], Types...content);

  /**
   * Write into the buffer, regardless of whether the position is valid.
   * If the position is not valid, false will be returned, no exception.
   */
  bool putCharAt(size_t x, size_t y, char c) noexcept;

  static std::tuple<int, int> getTerminalSize();
 protected:
//  std::shared_ptr<char> buffer;
  std::shared_ptr<Buffer<char, 2>> buffer;
  std::size_t width, height;
  std::size_t maxWidth = 50, maxHeight = 50;
  std::size_t oldWidth = 0, oldHeight = 0;
  std::function<bool()> ifContinue_;
  bool on;

  std::shared_ptr<std::thread> pRenderingThread_;
 public:
};

#endif //GAMEOFLIF_SCREEN_H
