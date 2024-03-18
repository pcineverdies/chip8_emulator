#include "keyboard.h"

bool key_is_pressed(KeySym ks) {
    Display *dpy = XOpenDisplay(":0");
    char keys_return[32];
    XQueryKeymap(dpy, keys_return);
    KeyCode kc2 = XKeysymToKeycode(dpy, ks);
    bool isPressed = !!(keys_return[kc2 >> 3] & (1 << (kc2 & 7)));
    XCloseDisplay(dpy);
    return isPressed;
}

uint16_t Keyboard::read_key(){

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

  return res;

}
