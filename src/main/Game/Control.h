//
// Created by 郑虎 on 2016-02-24.
//

#ifndef GAMEOFLIF_CONTROL_H
#define GAMEOFLIF_CONTROL_H

#include "../Engine/Curses.h"

class Control {
 public:
  virtual void render(Curses& screen);
  struct Compare {
    bool operator()(const Control &lhs, const Control &rhs) const {
      return lhs.zIndex < rhs.zIndex;
    }
  };
 protected:
  int zIndex;
};

#endif //GAMEOFLIF_CONTROL_H
