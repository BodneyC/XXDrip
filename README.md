XXDrip
===

I wanted to make a small program that functioned like XXD but for Windows; yes, I realise that "XXD for Windows" exists already but this was more of a practice thing in a couple of areas including binary IO. I also wanted (needed) some practice in moving from C to C++ and though this would be a good project.

## Compilation

Clone the [repository](https://github.com/BodneyC/XXDrip.git) and compile the program with "comp.bat"; use in situ or add to PATH.

```batch
git clone https://github.com/BodneyC/XXDrip.git
cd XXDrip\
comp.bat
```

## Usage

With no other command line arguments XXDrip takes an input **[no switch]** and outputs the entirety of the file in formatted hex(similar to XXD) until EOF.

**Example Output**
```
> XXDrip hex_out.h
Hexadecimal Output

          0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f  : 0 1 2 3 4 5 6 7 8 9 a b c d e f
          ----------------------------------------------- : -------------------------------
      0 | 23 69 66 6E 64 65 66 20 48 45 58 5F 4F 55 54 5F : # i f n d e f . H E X _ O U T _
   0x10 | 48 0D 0A 23 64 65 66 69 6E 65 20 48 45 58 5F 4F : H . . # d e f i n e . H E X _ O
   0x20 | 55 54 5F 48 0D 0A 0D 0A 23 69 6E 63 6C 75 64 65 : U T _ H . . . . # i n c l u d e
   0x30 | 20 3C 69 6F 73 74 72 65 61 6D 3E 0D 0A 23 69 6E : . < i o s t r e a m > . . # i n
   0x40 | 63 6C 75 64 65 20 3C 69 6F 6D 61 6E 69 70 3E 0D : c l u d e . < i o m a n i p > .
   0x50 | 0A 23 69 6E 63 6C 75 64 65 20 3C 76 65 63 74 6F : . # i n c l u d e . < v e c t o
   0x60 | 72 3E 0D 0A 23 69 6E 63 6C 75 64 65 20 3C 66 73 : r > . . # i n c l u d e . < f s
   0x70 | 74 72 65 61 6D 3E 0D 0A 0D 0A 74 79 70 65 64 65 : t r e a m > . . . . t y p e d e
   0x80 | 66 20 75 6E 73 69 67 6E 65 64 20 63 68 61 72 20 : f . u n s i g n e d . c h a r .
   0x90 | 42 59 54 45 3B 0D 0A 0D 0A 69 6E 74 20 68 65 78 : B Y T E ; . . . . i n t . h e x
   0xa0 | 62 65 67 69 6E 20 28 69 6E 74 20 6A 2C 20 73 74 : b e g i n . ( i n t . j , . s t
   0xb0 | 64 3A 3A 6F 73 74 72 65 61 6D 20 26 73 74 72 65 : d : : o s t r e a m . & s t r e
   0xc0 | 61 6D 2C 20 69 6E 74 20 72 6F 77 4E 75 6D 29 3B : a m , . i n t . r o w N u m ) ;
   0xd0 | 0D 0A 76 6F 69 64 20 68 65 78 6F 75 74 70 75 74 : . . v o i d . h e x o u t p u t
   0xe0 | 20 28 42 59 54 45 20 73 65 63 74 6F 72 5B 5D 2C : . ( B Y T E . s e c t o r [ ] ,
   0xf0 | 20 73 74 64 3A 3A 6F 73 74 72 65 61 6D 20 26 73 : . s t d : : o s t r e a m . & s
  0x100 | 74 72 65 61 6D 2C 20 69 6E 74 20 6A 2C 20 69 6E : t r e a m , . i n t . j , . i n
  0x110 | 74 20 62 79 74 65 63 6F 75 6E 74 2C 20 69 6E 74 : t . b y t e c o u n t , . i n t
  0x120 | 20 6D 6F 64 65 29 3B 0D 0A 76 6F 69 64 20 68 65 : . m o d e ) ; . . v o i d . h e
  0x130 | 78 4C 54 31 36 6F 75 74 20 28 42 59 54 45 20 2A : x L T 1 6 o u t . ( B Y T E . *
  0x140 | 73 65 63 74 6F 72 2C 20 73 74 64 3A 3A 6F 73 74 : s e c t o r , . s t d : : o s t
  0x150 | 72 65 61 6D 20 26 73 74 72 65 61 6D 2C 20 69 6E : r e a m . & s t r e a m , . i n
  0x160 | 74 20 69 6E 69 74 4A 2C 20 69 6E 74 20 73 74 61 : t . i n i t J , . i n t . s t a
  0x170 | 72 74 2C 20 69 6E 74 20 62 79 74 65 63 6F 75 6E : r t , . i n t . b y t e c o u n
  0x180 | 74 29 3B 0D 0A 0D 0A 23 65 6E 64 69 66 0D 0A    : t ) ; . . . . # e n d i f . .

Program complete
```

Switches can be provided at the command line to output the hex raw **[-r]** (no formatting), and to a file **[-o]** as either plain-text or as binary **[-b]** (similar function to dd).

Two additional switches allow the user to specify a starting offset **[-s]** and an ending offset **[-e]**.

**Command format (TCLAP output):**
```
USAGE:

   XXDrip.exe  [-e <Int>] [-s <Int>] [-o <String>] [-b] [-r] [--] [--version]
             [-h] <String>


Where:

   -e <Int>,  --end <Int>
     Offset of End Read

   -s <Int>,  --start <Int>
     Offset of Begin Read

   -o <String>,  --output <String>
     Output to File-path

   -b,  --binary
     Output in binary mode

   -r,  --raw
     Output Raw Hex

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   <String>
     (required)  Input File-path


   XXDrip
```
