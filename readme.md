# rundll64
## Features
This tool can run dll methods through command line. This tool uses WINAPI
## Using
Syntax:
```
rundll64 [[@<dllname> ]#<methodname> <arg1> ...] ... 
```
if arg is '%i', it will be replace by i index method out

arg can be represented as hex value by entering "0x" before hex value

Example:
```
> rundll64 @kernel32.dll #GetStdHandle -11 #WriteConsoleA %0 testing 7 0 0
testing

> rundll64 @kernel32.dll #GetProcessHeap #HeapAlloc %0 8 255 #GetCurrentDirectoryA 255 %1 #GetStdHandle -11 #WriteConsoleA %3 %1 %2 0 0
C:\Users\Admin\source\repos\x64\Release
```