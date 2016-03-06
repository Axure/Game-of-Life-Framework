//
// Created by 郑虎 on 2016-03-06.
//

#ifndef GAMEOFLIF_MACROS_H
#define GAMEOFLIF_MACROS_H

#define AX_GROUP_ANNOTATION(message, scope) \
 scope:

#define COMPUTED(name, function) \

#define AX_BIND_VARIABLE(var, bound) \
  decltype(var) get##bound() const noexcept {\
    return var;\
  }\
  void set##bound(decltype(var)& value) {\
    this->var = value;\
  }\

#endif //GAMEOFLIF_MACROS_H
