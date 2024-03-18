# Chip 8 Emulator

## Resources
- [Chip 8 specs](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3xkk)
- [Chip 8 roms](https://github.com/kripod/chip8-roms)

## Install

### Requirements
**Works on linux with X11 protocol, no Wayland**

Run:

```bash
mkdir build

make
```

## How to use

```bash
./build/chip8_emulator path_to_rom
```

Once that the rom is running, the following mapping is used for the keyboard

```
1 2 3 4   ->   1 2 3 a
q w e r        4 5 6 b
a s d f        7 8 9 c
z x c v        d 0 e f
```

When the emulator is running, press `p` to close it.

## Pictures

![](assets/brick.png)
![](assets/maze.png)
