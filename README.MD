# (SUPER) FRED

SUPER FRED is a system based on the RCA CDP 1802 processor designed by Joseph Weisbecker.

The purpose of this project is to provide the tools needed to run a "modernized" and "semplified" version of a system based on this processor.

This repository contain an assembler and an emulator to develop and run programs for such system.

Both tools have been build from scratch in C language.

## SYSTEM DESCRIPTION

The system emulate the following components :
```
- CPU      : RCA CDP1802
- MEMORY   : 64 KB
- VIDEO    : 40x25 characters, 16 colors
- KEYBOARD : standard keyboard
- SOUND    : 48 tones speaker
```

For details of specifications refer to the [SPECS](DOCS/SPECS.MD) section.

## ASSEMBLER

To invoke the assembler use :
```
FRED_ASM.EXE INPUT_FILE OUTPUT_FILE
```
where :

```
INPUT_FILE  : your assembly source code
OUTPUT_FILE : binary file output
```

## EMULATOR

In order to run the emulator, in the same folder of the executable must be present the following files :
```
- glfw3.dll
- OpenAL32.dll
- soft_oal.dll
- frag.glsl
- vert.glsl
- charset.rom (required if using default values)
- program.bin (required if using default values)
```

To invoke the emulator with defaults values and speed (1 Mhz) use :
```
FRED_EMU.EXE
```
To invoke the emulator with custom parameters use :
```
FRED_EMU.EXE CPU_MHZ CHARSET_FILE PROGRAM_FILE
```
where :

```
CPU_MHZ      : the speed of the cpu in Mhz (integers only)
CHARSET_FILE : your custom charset file
PROGRAM_FILE : your binary program
```

## BUILDING

To build the assembler and/or the emulator from source open a console like : 

```
Native Tools Command Prompt for VS 2022
```

and run one of the following commands in the according folder :

```
DEBUG.BAT   : debug mode with static analyzer and checks for memory leaks
RELEASE.BAT : release mode
CLEAN.BAT   : to clean intermediate files
```

## PROGRAMMING

The assembler use a two steps approach without intermediate files.

The full reference of the CDP1802 instruction set is documented in the [ISA](DOCS/ISA.MD) section.

Some simple macros are available in the [MACRO](DOCS/MACRO.MD) section.

Basic informations and several code snippets are provided in the [ASM](DOCS/ASM.MD) section.

## DEVELOPMENT

Currently working on the following :
- detection of non-printable keys of the keyboard

The future improvements of the project will include the implementation of the following functionalities :
- an (optional) external timer 
- an (optional) block device 
- an (optional) graphic mode