# 6809 Simulator/Emulator
=======================

Sbc09 stands for Lennart Benschop's 6809 Single Board Computer.
It contains an assembler and simulator for the Motorola M6809 processor.

Copyleft (c) 1994-2018 by the sbc09 team, see AUTHORS for more details.
license: GNU General Public License version 2, see LICENSE for more details.

Forum thread: http://archive.worldofdragon.org/phpBB3/viewtopic.php?f=8&t=4880

Project: https://github.com/6809/sbc09

Grant's Simple 6809 SBC: http://searle.hostei.com/grant/6809/Simple6809.html

For the usage of the assembler a09 and 6809 single board system v09 
read doc/sbc09.creole!

I've added sbc09.html and monitor.html to the doc directory.

This distribution includes two different kinds of simulators:
 1. The old sim6809 based "simple" simulator built as v09s, v09st
 2. The 6809 single board system as a stand alone environment built as v09

## License
GNU General Public License version 2

## Notes
--------

*Updated:* 20180531 - ncherry@linuxha.com

I have found a couple of different versions of this simulator and while all are similar
there are usually differences and the documentation seems to be out of sync. I intend
to merge what I can, fix the documentation and modernize the code a bit. I'm not sure
this is a good or bad idea but if you don't like it you can fork the code. :smile:

I've added a -f and -a option (see below) to allow you to use a different rom file and
to move the ACIA.

This simulator contain 3 program, a09, makerom and v09.

### a09
A simple assembler, I find it a bit wonky so I won't put too much work into it. I prefer asl.

### makerom
I'm not exactly sure how this works but it reads standard input as S-records and build ROM image file v09.rom

### v09
This is the actual simulator. There are 3 different versions of it. v09, the old version, v09s - not sure of it's differences yet and v09st which is like v09s but conpiled with Turbo C in mind. Still need to do a bit more digging on this part

## Structure
---------

src/

  Source for the developement tools and virtual machines ...

  a09.c

      The 6809 assembler. It's fairly portable (ANSI) C. It works on both
      Unix and DOS (TC2.0).

      Features of the assembler:
      - generates binary file starting at  the first address
        where code is actually generated. So an initial block of RMB's
        (maybe at a different ORG) is not included in the file.
      - Accepts standard syntax.
      - full expression evaluator.
      - Statements SET, MACRO, PUBLIC, EXTERN IF/ELSE/ENDIF INCLUDE not yet
        implemented. Some provisions are already made internally for macros
        and/or relocatable objects.

  v09s.c

      The (old) 6809 simulator. Loads a binary image (from a09) at adress $100
      and starts executing. SWI2 and SWI3 are for character output/input.
      SYNC stops simulation. When compiling set -DBIG_ENDIAN if your
      computer is big-endian. Set TERM_CONTROL for a crude single character
      (instead of ANSI line-by-line) input. Works on Unix.

  v09stc.c

      Same as v09s.c but for Turbo C. Has its own term control.

  v09.c

  engine.c

  io.c

      The 6809 single board simulator/emulator v09.
       
  mon2.asm

      Monitor progam, alternative version of monitor.asm
      (used in ROM image alt09.rom)

  monitor.asm

      Monitor progam (used in ROM image v09.rom for v09)

  makerom.c

      Helper tool to generate ROM images for v09.

as9/

  Grant's as9 assembler (better than a09)

basic/

  Basic interpreters ...

  - basic.asm

      Tiny Basic

  - fbasic.asm

      Tiny Basic with Lennarts floating point routines.


doc/

  Documentation ...


examples/

  Several test and benchmark programs, simple routines and some bigger stuff
  like a Forth system (ef09).

  ef09.asm Implementation of E-Forth, a very rudimentary and portable Forth.
      Type WORDS to see what words you have. You can evaluate RPN integer
      expressions, like "12 34 + 5 * . " You can make new words like
      " : SQUARED DUP * ; " etc.

as9/

examples_forth/

  Forth environment with examples.
  For the 6809 single board system.


Notes on Linux Fedora Core 6
----------------------------
2012-06-04

Compiling v09s, v09st:

 * BIG_ENDIAN (already used by LINUX itself, changed to CPU_BIG_ENDIAN)
   Now automatically set according to BIG_ENDIAN and BYTE_ORDER
   if existing.

 * If TERM_CONTROL mode is active the keyboard is not really in raw mode -
   keyboard signals are still allowed.

 * A tracefilter based on register values can be placed in the TRACE area to
   get tracing output triggered by special states 
   


a09 Assembler
-------------

Bugfixes:
 * addres modes  a,INDEXREG b,INDEXREG d,INDEXREG now known
    as *legal*!

Extended version:
    http://lennartb.home.xs4all.nl/A09.c
    (see above)

 * options -x and -s produces output in Intel Binary/Srecord format,
   contains the above mentioned bugfixes (but fixed by the original
   author).




v09s* Simulator
---------------

### CC register
```
        E F H I N Z V C  Flag
        8 7 6 5 4 3 2 1  Bit
        | | | | | | | |
        | | | | | | | +- $01
        | | | | | | +--- $02
        | | | | | +----- $04
        | | | | +------- $08
        | | | +--------- $10
        | | +----------- $20
        | +------------- $40
        +--------------- $80
```

# differences from real 6809:

    ldd #$0fc9
    addb #$40
    adca #$00

H is set on VCC but not on real 6809, sim6809 does what?

          
### special behavior

    swi2        output character (STDOUT) in register B
    swi3        read character from keyboard into register B
    sync        exit simulator

### Help
```
$ ./v09 -h
v09 1.0.1
Usage: v09 [-t tracefile [-l addr] [-h addr] ] [-e escchar] [-f filenom.rom] [-a 0xE000]
Default filename is v09.rom (if no -f is used)
Default ACIA locateion: 0xE000 (if no -a is used)
```

### start program
```
v09s -f BINARY
```

### start program with tracing output on STDOUT
```
v09st -f BINARY
```

### run program and leave memory dump (64k)
#### memory dump in file dump.v09
```
v09s -d BINARY 
```

Don't attempt a memory dump, this isn't currently an option. This is one of this *out of sync* document issues.

### Bugfixes

 * 20180531 - Added -a to allow the ACIA to be moved from 0xE000
 * 20180531 - Actually added the -f filename.rom to the options so you can run with files other than v09.rom

 * static int index;
   otherwise the global C library function index() is referenced!
   Write access on it leads to a core dump.

 * BIG_ENDIAN is not useable in FLAG because (POSIX?) Unix
   (especially Linux) defines its byte order.
   If BIG_ENDIAN == BYTE_ORDER -> architecture is big endian!
   Changed to CPU_BIG_ENDIAN, which is refering BIG_ENDIAN and
   BYTE_ORDER automatically (if existent).






eForth
------

Source:

    ef09.asm

    Backspace character changed from 127 to 8.


Memory-Layout:

    0100   At this address the binary is placed to, the Forth entry point
    03C0   USER area start
    4000   Memory TOP


I/O:
    Keyboard input:
     * ^H or BSP deletes character
     * RETURN -> interrupts (long) output

Start:

    ../v09s ef09


Bugs:
    SEE     ;
    STAR (*)    : * UM* DROP ;  ... wrong,
            : * M* DROP ; ... correct (sign!)

Typical commands:

 Commands alway in upper case!!!

WORD    list of defined words of the current vocabulary

BYE     exit Forth (back to shell)
DUMP        hex memory dump 
SEE         HL-word decompiler, corrected:
        * stops at EXIT
        * handles more special primitives (literals, strings,
          variable, constants))
        * handles Direct Threading
        * output line by line with address
.S      shows the content of the parameter stack

count FOR ... NEXT
        replacement for
        hi lo DO ...  I ... LOOP
        hi lo - 1+ FOR ... R@ lo + ... NEXT




Extensions:

    ZEQUAL      0=      Primitive
    PLUS1       1+      Primitive, added
2012-06-07
    ROLL        ROLL        HL, added
    CONST       CONSTANT    HL, added
    doCONST             Primitive, added

2012-06-08
    TWOSTAR     2*      Primtive, added
    TWOSLASH    2/      Primtive, added
    MINUS1      1-      Primtive, added
    SWAPHL      ><      Primtive, added
    STAR256     256*        Primtive, added
    SLASH256    256/        Primtive, added
    CMOVE       CMOVE       Primtive
    FILL        FILL        Primtive
2012-06-09
    ULESS       U<      Primitive
    LESS        <       Primitive
    DO      DO      HL, added
    QDO     ?DO     HL, added
    DODO        (DO)        Primitive, added
    DOQDO       (?DO)       Primitive, added
    LOOP        LOOP        HL, added
    PLOOP       +LOOP       HL, added
    DOLOOP      (LOOP)      Primitive, added
    DOPLOOP     (+LOOP)     Primitive, added
    
2012-06-11
    NEGAT       NEGATE      Primitive, alternative added
    UMSTA       UM*     Primitive, but without MUL
    LSHIFT      LSHIFT      Primitive, added
    RSHIFT      RSHIFT      Primitive, added
2012-06-12
    LEAVE       LEAVE       Primitive, added (fig Forth)
    MDO     -DO     HL, added
    DOMDO       (-DO)       Primitive, added
    I       I       Primitive, added (same as R@)
    CMOVEW      CMOVE       Primitive, other implementation
    STAR        *       korr.: uses M* (instead UM*)
    BLANK       BL      Constant

2012-06-19
    USLASH      U/      Primitive, same as UM/MOD
                    UM/MOD uses USLASH

2012-06-20
    DPLUS       D+      Primitive
    DSUB        D-      HL
    ZERO        0       Constant
    ONE     1       Constant
    TWO     2       Constant
    MONE        -1      Constant
    DOCLIT      doCLIT      Primitive
2012-06-21
    SEE     SEE     extended: handles LIT, CLIT
2012-06-22
    SEE     SEE     extended: handles
                    BRANCH,?BRANCH,?DO,-DO,LOOP,+LOOP,."..."

2012-09-07
    SEE     SEE     ABORT", (DO) added, remarks corrected.

TODO:
 * XXX marks points to open issues.
 * SEE command:
   handling of
    - [COMPILE]
    - DOCONST, DOVAR, DOUSE


TEST:

HEX ok
0 8000 8001 U/ . . FFFE 2 ok
FFFE 8001 U* . . U* ?  ok
FFFE 8001 UM* . . 7FFF FFFE ok
FFFE 8001 UM* 2 0 D+ . . 8000 0 ok

0 8000 7FFF U/ . . FFFF FFFF ok
0 FFFF FFFF U/ . . FFFF FFFF ok
0 FFFE FFFF U/ . . FFFE FFFE ok
FFFF FFFF UM* . . FFFE 1 ok
FFFF FFFE FFFF U/ . . FFFF FFFE ok


### mon2.asm commands

  - ^] (Ctrl-]  0x1D) Enters Emulator help
    - L - L<filename> - Log            (W)
    - S - S<filename> - S              (R)
    - X - eXit
    - U - U<filename> - Upload         (R)
    - D - D<filename> - Download       (W)
    - R - Reset
    - ! - Suspend to shell (Like ^Z)
    - H - Help
    - ? - Help
    Note: '*' means that the command is not enabled yet
  - A - Asm    - Aaddr
  - B - Break  - B or B<addr>. B displays, B<addr> sets or clears breakpoint
  - C - Calc   - Chexnum{+|-hexnum}
  - D - Dump   - D or D<addr> or D<addr>,<length>
  - E - Enter  - E or E<addr> or E<addr> <bytes> or E<addr>string
  - F - Find   - Faddr bytes or Faddr",34,"ascii",34,"
  - G - Go     - G or G<addr> Run 
  - H - Help   - H
  - I - Inp    - Iaddr - Inspect
  - J - Jump   - J<addr>
  - M - Move   - M<addr1>,<addr2>,<lenght>
  - P - Prog   - P (like continue, use after J<addr>)
  - R - Regs   - R or R<letter><hex>
  - S - Srec   - SO<addr> or SS<addr>,<len> or S1<bytes> or S9<bytes>
    - SS<addr> - S1 dump
    - S0/S1/S9 - S1 Load
  - T - Trace  - T (use after J<addr>)
  - U - Unasm  - U or Uaddr or Uaddr,length
  - X - Xmodem - XSaddr,len XLaddr,len XX XOcrlf,filler, XSSaddr,len
  - Z - User   - user defined
 - Help - H

Links/References
================


Project:
  https://github.com/6809/sbc09
  Maintained by the original author and others.

Source:
  http://groups.google.com/group/alt.sources/browse_thread/thread/8bfd60536ec34387/94a7cce3fdc5df67
  Autor: Lennart Benschop  lennart@blade.stack.urc.tue.nl, 
                lennartb@xs4all.nl (Webpage, Subject must start with "Your Homepage"!)

  Newsgroups: alt.sources
  From: lennart@blade.stack.urc.tue.nl (Lennart Benschop)
  Date: 3 Nov 1993 15:21:16 GMT
  Local: Mi 3 Nov. 1993 17:21
  Subject: 6809 assembler and simulator (examples) 2/2


Homepage/Download links of Lennart Benschop:
  http://lennartb.home.xs4all.nl/m6809.html
  http://lennartb.home.xs4all.nl/sbc09.tar.gz
  http://lennartb.home.xs4all.nl/A09.c


Emulator for 6809 written in Python, can run sbc09 ROM:
  https://github.com/jedie/DragonPy/


Newer posting in alt.sources (1994):

  Newsgroups: alt.sources
  From: lenn...@blade.stack.urc.tue.nl (Lennart Benschop)
  Date: 17 May 1994 08:13:25 GMT
  Local: Di 17 Mai 1994 10:13
  Subject: 6809 assembler/simulator (3 of 3)


Referenced by:

  http://foldoc.org/6809
    Reference points to posting with buggy version from 1993.

  http://lennartb.home.xs4all.nl/m6809.html
    BAD LINK: http://www.sandelman.ocunix.on.ca/People/Alan_DeKok/interests/6809.html
    -> http://www.sandelman.ottawa.on.ca/People/Alan_DeKok/interests/
    6809 specific site will be redirected, but does not exist.

    Internet-Archiv:
    https://web.archive.org/web/20070112041235/
    http://www.striker.ottawa.on.ca/6809/
    2014-05-01: Lennart B. lennartb@xs4all.nl has been informed.

  http://archive.worldofdragon.org/phpBB3/viewtopic.php?f=5&t=4308&start=60#p9750
