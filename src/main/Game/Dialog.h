//
// Created by 郑虎 on 2016-02-24.
//

#ifndef GAMEOFLIF_DIALOG_H
#define GAMEOFLIF_DIALOG_H
#include <set>
#include "Control.h"
#include "Canvas.h"

class Dialog {
 public:
  struct Compare {
    bool operator()(const Dialog &lhs, const Dialog &rhs) const {
      return lhs.zIndex < rhs.zIndex;
    }
  };

  Dialog();
  virtual ~Dialog();

//  virtual void render(Curses& screen); // We directly use the screen's buffer.

 protected:
  int zIndex;
  std::set<Control, Control::Compare> controls_;
};

#endif //GAMEOFLIF_DIALOG_H
