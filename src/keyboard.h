#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <X11/Xlib.h>
#include <iostream>
#include "X11/keysym.h"

class Keyboard{

public:
  uint16_t read_key();
};

#endif // ! __KEYBOARD_H
