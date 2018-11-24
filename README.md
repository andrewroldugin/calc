# Calc - calculate arithmetic expression using PEG
## How to use
Run interactive mode by running (`-a` or `--ast` key for AST printing)
```
./calc
```
or use pepared test data:
```
./calc < data/test.txt
```
## Example of work
```
$ ./calc --ast
Enter math expression followed by Enter key
or Ctrl-C to exit
2+5+3*4
2+5+3*4 = 19
............2.
............N4calc6NumberE
.........2.
.........N4calc7ProductE
.........+.
.........N4calc3AddE
............5.
............N4calc6NumberE
.........5.
.........N4calc7ProductE
.........+.
.........N4calc3AddE
..............3.
..............N4calc6NumberE
..............*.
..............N4calc3MulE
..............4.
..............N4calc6NumberE
.........3*4.
.........N4calc7ProductE
2+5+3*4.
N4calc3SumE
```
## How to build
### Requirements:
It requires `gtest` to be installed on your system for building tests.

To install required packages on ArchLinux run the following:
```
sudo pacman -S gtest
```
on Windows MSYS2:
```
pacman -S mingw-w64-x86_64-gtest
```
### Building:
To build both program & tests:
```
make
```
To build only program without tests:
```
make calc
```
To build & run tests:
```
make test
```
Program was tested under ArchLinux and Windows7 using MSYS2.
