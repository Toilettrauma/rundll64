#pragma once
#include <stdio.h>
#include <stdarg.h>

// automatically allocate buffer. Future deallocate required
extern inline int vasprintf(char*& _Buffer, const char* _Format, va_list args);
extern inline int asprintf(char*& _Buffer, const char* _Format, ...);