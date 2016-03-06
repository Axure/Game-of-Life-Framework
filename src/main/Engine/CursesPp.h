//
// Created by 郑虎 on 2016-03-06.
//

#ifndef GAMEOFLIF_CURSESPP_H
#define GAMEOFLIF_CURSESPP_H
#include "../Macros.h"

#include <cstddef>
#include <tuple>

class CursesPp {
 AX_GROUP_ANNOTATION("Type definition", public)
  using LengthType = std::size_t;
  using LengthTupleType = std::tuple<LengthType, LengthType>;

 AX_GROUP_ANNOTATION("Constructor", public)
  CursesPp();

 AX_GROUP_ANNOTATION("States", protected)
  bool on_;

 AX_GROUP_ANNOTATION("Output", public)
  /**
   * Attach the curses from the terminal.
   */
  void attach();
  /**
   * Detach the curses from the terminal.
   */
  void detach();
  /**
   * Detach the curses from the terminal.
   * TODO: this should not be in this driver layer.
   */
  void resize(LengthType height, LengthType width);
  /**
   * Return the tuple of the height and width of the current screen.
   * @throws If the driver is not running, an exception will be thrown.
   */
  LengthTupleType getSize() noexcept(false);
  /**
   * Fit the curses into the current screen.
   */
  void fit() noexcept(false);
  /**
   * Refresh the screen.
   */
  void refresh();
  /**
   *
   */

 AX_GROUP_ANNOTATION("Input", public)
  /**
   * @method putCharAt
   * Put a character on the given position.
   * @param row The row coordinate.
   * @param column The column coordinate.
   */
  void putCharAt(LengthType row, LengthType column, char ch);
};

#endif //GAMEOFLIF_CURSESPP_H
