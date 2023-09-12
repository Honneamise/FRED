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

The video memory have the following layout:
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
this will produce : <span style="color:#FF0000;background-color:#000000">☻</span>

The 16 available colors for the byte attribute nibble are:

<span style="color:#000000">█0</span>
<span style="color:#800000">█1</span>
<span style="color:#008000">█2</span>
<span style="color:#808000">█3</span>
<span style="color:#000080">█4</span>
<span style="color:#800080">█5</span>
<span style="color:#008080">█6</span>
<span style="color:#C0C0C0">█7</span>
<span style="color:#808080">█8</span>
<span style="color:#FF0000">█9</span>
<span style="color:#00FF00">█A</span>
<span style="color:#FFFF00">█B</span>
<span style="color:#0000FF">█C</span>
<span style="color:#FF00FF">█D</span>
<span style="color:#00FFFF">█E</span>
<span style="color:#FFFFFF">█F</span>

(By default the video memory attributes are initialized with : 0xF0 white on black)

## CHARSET

The character set is the classic Codepage 437, with 256 available symbols.

Each symbols is 8x8 pixels (8 bytes) of consequence a complete set is 2048 bytes.

The emulator will load the binary file containing the set starting at memory location 0x07D0.

**NOTE** : a great resource for fonts is [int10h.org](https://int10h.org/oldschool-pc-fonts/fontlist/), just be sure to select a 8x8 format.

## KEYBOARD

The keyboard has a 64 byte buffer that stores the ASCII values of keystrokes. 

If the keyboard buffer contains data, **EF1** is set to **1**, otherwise to **0**.

To pop data from the buffer read the data bus with : **INP 1**

## AUDIO

There are availables 48 different notes to play.

To handle the audio device write a value on the data bus using : **OUT 2** 

To start the playback of a note output a value between **0x01** and **0x30** (both included).

To stop the playback output the value : **0x00**. 



