# SYSTEM DETAILS

## CPU
The emulated CPU is a RCA CDP1802 full isa, executing instructions in **TIME BURSTING** mode.

The speed of the cpu can be set as integer parameter in Mhz (default is 1 Mhz), for reference consider that the Intersil BC at 5v run at 5.0MHz.

For further details it is available the [1802 MANUAL](1802_MANUAL.PDF) from 1976.

## MEMORY
64 Kb of memory organized as follow :
```
 __________
| video    | 0000
|          |  
|__________| 07CF
2000 bytes
 __________
| charset  | 07D0
|          |
|__________| 0FCF
2048 bytes
 __________
| reserved | 0FD0
|          |
|__________| 0FFF
48 bytes
 __________
| program  | 1000
|          |
|__________| FFFF
remaining bytes

```

## VIDEO
Every 16.6 msec the video device reads the 2000 byte of the memory from 0x000 to 0x07CF updating the display.

The video device can be set in TEXT or GRAPH mode :

- __TEXT__  : 40x25 characters, 16 colors
- __GRAPH__ :160x100 pixels, monochrome

To handle the video device write a value on the data bus using : **OUT 1** 

To select the __TEXT__ mode (_default_) output the value **0x00** 

To select the __GRAPH__ mode output the value **0x01**

To get the current video mode read the data bus with : **INP 1**

## TEXT MODE

The video memory in text have the following layout:

```
        0   1 ║ 2   3 ║ 4   5 ║ ... ║78  79
      ┌───┼───╫───┼───╫───┼───╫─────╫───┼───╖
     0│chr│atr║chr│atr║chr│atr║ ... ║chr│atr║ line 0
      ├───┼───╫───┼───╫───┼───╫─────╫───┼───╢
    80│chr│atr║chr│atr║chr│atr║ ... ║chr│atr║ line 1
      ├───┼───╫───┼───╫───┼───╫─────╫───┼───╢
   160│chr│atr║chr│atr║chr│atr║ ... ║chr│atr║ line 2
      ├───┼───╫───┼───╫───┼───╫─────╫───┼───╢
   240│chr│atr║chr│atr║chr│atr║ ... ║chr│atr║
      ├───┼───╫───┼───╫───┼───╫─────╫───┼───╢
      \                                     \
      ├───┼───╫───┼───╫───┼───╫─────╫───┼───╢
  1920│chr│atr║chr│atr║chr│atr║ ... ║chr│atr║ line 24
```

All even-numbered addresses contain characters and all odd-numbered addresses contain video attributes.

The width of a line is 40 characters (80 bytes) and there are 25 lines (from 0 to 24)

Each character is followed by an attribute byte with foreground/background color.

```
  | CHR  | ATR  |
  | 0x02 | 0x90 |
  | ☻    |   || |
             ||
             ||
foreground --||-- background
```
this will produce : ☻

The 16 available colors for the byte attribute nibble are:

```
0 - Black
1 - Maroon
2 - Green
3 - Olive
4 - Navy
5 - Purple
6 - Teal
7 - Silver
8 - Gray
9 - Red
A - Lime
B - Yellow
C - Blue
D - Magenta
E - Cyan
F - White
```

_(By default the video memory attributes are initialized with : 0xF0 white on black)_

## CHARSET

The character set is the classic Codepage 437, with 256 available symbols.

Each symbols is 8x8 pixels (8 bytes) of consequence a complete set is 2048 bytes.

The emulator will load the binary file containing the set starting at memory location 0x07D0.

**NOTE** : a great resource for fonts is [int10h.org](https://int10h.org/oldschool-pc-fonts/fontlist/), just be sure to select a 8x8 format.


## GRAPH MODE

In graph mode each line is 20 bytes long and there are 100 lines.

Each byte contains 8 pixels (16,000 total pixels):

```
    ╓7┬6┬5┬4┬3┬2┬1┬0╖
    ║ │ │ │ │ │ │ │ ║
    ╙╥┴╥┴╥┴╥┴╥┴╥┴╥┴╥╜  bits mask
     ║ ║ ║ ║ ║ ║ ║ ╚══► 0:  01H  eighth pixel in byte
     ║ ║ ║ ║ ║ ║ ╚════► 1:  02H  seventh pixel in byte
     ║ ║ ║ ║ ║ ╚══════► 2:  04H  sixth pixel in byte
     ║ ║ ║ ║ ╚════════► 3:  08H  fifth pixel in byte
     ║ ║ ║ ╚══════════► 4:  10H  fourth pixel in byte
     ║ ║ ╚════════════► 5:  20H  third pixel in byte
     ║ ╚══════════════► 6:  40H  second pixel in byte
     ╚════════════════► 7:  80H  first pixel in byte
```

Each 1-bit field selects _black_ (0) or _white_ (1).

## KEYBOARD

The keyboard has a 64 byte buffer that stores the ASCII values of keystrokes. 

If the keyboard buffer contains data, **EF2** is set to **2**, otherwise to **0**.

To pop data from the buffer read the data bus with : **INP 2**

## TIMER

It is available a clock that is activated **16.6** times per second.

At every activation the clock is increased by **1**, upon reaching **60** the clock resets to **0**.

To get the current clock value read the data bus with : **INP 3**

## AUDIO

There are availables 48 different notes to play.

To handle the audio device write a value on the data bus using : **OUT 4** 

To start the playback of a note output a value between **0x01** and **0x30** (both included).

To stop the playback output the value : **0x00**. 



