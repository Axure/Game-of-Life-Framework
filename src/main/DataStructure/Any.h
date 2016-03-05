//
// Created by 郑虎 on 2016-02-12.
//

#ifndef GAMEOFLIF_ANY_H
#define GAMEOFLIF_ANY_H

class AnyBase {
  AnyBase();
  ~AnyBase();
};

template <class T>
class Any {
  T get() {

  }
};

template<class T>
T get(AnyBase anyBase) {

}

#endif //GAMEOFLIF_ANY_H
