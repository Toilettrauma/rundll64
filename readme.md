This tool can run dll methods through cmd arguments. This tool uses WINAPI

Syntax:
```
rundll64 @<dllname> #<methodname> <args>... [[@<dllname> ]#<methodname> <args>...]... 
```
arg can be '%' to pass previous method return value

arg can be represented as hex value by entering "0x" before hex value

Example:
```
> rundll64 @kernel32.dll #GetStdHandle -11 #WriteConsoleA % testing 5 0 0
GetStdHandle -> 0x48
testingWriteConsoleA -> 0x1
> rundll64 @kernel32.dll #CreateConsoleScreenBuffer 0xC0000000 0 0 0 0 #SetConsoleActiveScreenBuffer %
CreateConsoleScreenBuffer -> 0x94

```