#include "exceptions.h"

FormatException::FormatException() noexcept {
    message = nullptr;
}

FormatException::FormatException(const char* format, ...) noexcept {
    va_list args;
    va_start(args, format);
    vasprintf(message, format, args);
    va_end(args);
}

void FormatException::init(const char* format, va_list args) noexcept {
    if (message == nullptr) {
        vasprintf(message, format, args);
    }
}

FormatException::~FormatException() {
    delete[] message;
}

const char* FormatException::what() const noexcept {
    return message;
}

SEException::SEException(unsigned int u) {
    code = u;
}

SEException::SEException(unsigned int u, const char* format, ...) noexcept {
    code = u;
    va_list args;
    va_start(args, format);
    __super::init(format, args);
    va_end(args);
}

AccessViolationException::AccessViolationException(void* at, uintptr_t with) : SEException(EXCEPTION_ACCESS_VIOLATION,
    "Access violation at address 0x%p with address 0x%p",
    at,
    with) {
    address = at;
    violationAddress = with;
}

AccessViolationException::~AccessViolationException() {
    delete[] message;
}

LibNotFoundException::LibNotFoundException(const char* name) : FormatException("Library %s not found", name) {
    this->name = new char[strlen(name) + 1];
    strcpy_s(this->name, strlen(name) + 1, name);
}

LibNotFoundException::~LibNotFoundException() {
    delete[] name;
}

MethodNotFoundException::MethodNotFoundException(const char* libName, const char* name) : FormatException("Method %s not found in library %s", libName, name) {
    this->libName = new char[strlen(libName) + 1];
    this->name = new char[strlen(name) + 1];
    strcpy_s(this->libName, strlen(libName) + 1, libName);
    strcpy_s(this->name, strlen(name) + 1, name);
}

MethodNotFoundException::~MethodNotFoundException() {
    delete[] libName;
    delete[] name;
}

FormatErrorException::FormatErrorException(off_t offset) : FormatException("Formatting error at arg %llu", offset) {

}
