# rundll64
## Features
This tool can run dll methods through command line. This tool uses WINAPI
## Using
Syntax:
```
rundll64 @<dllname> #<methodname> <args>... [[@<dllname> ]#<methodname> <args>...]... 
```
if arg is '%i', it will be replace by i index method out

arg can be represented as hex value by entering "0x" before hex value

Example:
```
> rundll64 @kernel32.dll #GetStdHandle -11 #WriteConsoleA %0 testing 7 0 0
testing
```