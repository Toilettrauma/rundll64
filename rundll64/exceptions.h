#pragma once
#include <exception>
#include <Windows.h>
#include <stdio.h>
#include <sys/types.h>

#include "misc.h"

struct FormatException : std::exception {
    char* message;
    FormatException() noexcept;
    FormatException(const char* format, ...) noexcept;
    void init(const char* format, va_list args) noexcept;
    ~FormatException();
    const char* what() const noexcept override;
};

struct SEException : FormatException {
    unsigned int code;

    SEException(unsigned int u);
    SEException(unsigned int u, const char* format, ...) noexcept;
};

struct AccessViolationException : SEException {
    void* address;
    uintptr_t violationAddress;

    AccessViolationException(void* at, uintptr_t with);
    ~AccessViolationException();
};


struct LibNotFoundException : FormatException {
    char* name;
    LibNotFoundException(const char* name);
    ~LibNotFoundException();
};

struct MethodNotFoundException : FormatException {
    char* libName;
    char* name;
    MethodNotFoundException(const char* libName, const char* name);
    ~MethodNotFoundException();
};

struct FormatErrorException : FormatException {
    long long index;
    FormatErrorException(off_t offset);
};