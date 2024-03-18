#include "keyboard.h"

/** key_is_pressed
    Check whether a key is pressed.
    Uses the X11 protocol

    @param ks KeySym identifier of a key
    @return bool whether the key is pressed or not
*/
bool key_is_pressed(KeySym ks) {
    Display *dpy = XOpenDisplay(":0");
    char keys_return[32];
    XQueryKeymap(dpy, keys_return);
    KeyCode kc2 = XKeysymToKeycode(dpy, ks);
    bool isPressed = !!(keys_return[kc2 >> 3] & (1 << (kc2 & 7)));
    XCloseDisplay(dpy);
    return isPressed;
}

/** Keyboard::read_key
    Return which key is pressed using the following matching

    1 2 3 4 -> 1 2 3 a
    q w e r    4 5 6 b
    a s d f    7 8 9 c
    z x c v    d 0 e f

    If the key x is pressed, then the xth bit of the result is set.
    If the key p is pressed, the result is 0xffff

    @return uint16_t mask with the pressed keys
*/
uint16_t Keyboard::read_key(){

  // Set bit nth of res if nth key is pressed
  // (in this way more than one key can be pressed at a time)
  uint16_t res = 0;

  if(key_is_pressed(XK_1)) res |= (1 << 0x01); //1
  if(key_is_pressed(XK_2)) res |= (1 << 0x02); //2
  if(key_is_pressed(XK_3)) res |= (1 << 0x03); //3
  if(key_is_pressed(XK_4)) res |= (1 << 0x0c); //c
  if(key_is_pressed(XK_Q)) res |= (1 << 0x04); //4
  if(key_is_pressed(XK_W)) res |= (1 << 0x05); //5
  if(key_is_pressed(XK_E)) res |= (1 << 0x06); //6
  if(key_is_pressed(XK_R)) res |= (1 << 0x0d); //d
  if(key_is_pressed(XK_A)) res |= (1 << 0x07); //7
  if(key_is_pressed(XK_S)) res |= (1 << 0x08); //8
  if(key_is_pressed(XK_D)) res |= (1 << 0x09); //9
  if(key_is_pressed(XK_F)) res |= (1 << 0x0e); //e
  if(key_is_pressed(XK_Z)) res |= (1 << 0x0a); //a
  if(key_is_pressed(XK_X)) res |= (1 << 0x00); //0
  if(key_is_pressed(XK_C)) res |= (1 << 0x0b); //b
  if(key_is_pressed(XK_V)) res |= (1 << 0x0f); //f
  if(key_is_pressed(XK_P)) res = 0xffff;

  return res;

}
