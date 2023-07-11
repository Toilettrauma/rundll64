#include "misc.h"


// automatically allocate buffer. Future deallocate required
inline int vasprintf(char*& _Buffer, const char* _Format, va_list args) {
    int messageSize = _vscprintf(_Format, args);
    if (messageSize <= 0) {
        _Buffer = nullptr;
        return -1;
    }
    _Buffer = new char[messageSize + 1ULL];
    vsnprintf(_Buffer, messageSize + 1ULL, _Format, args);
    return messageSize;
}

inline int asprintf(char*& _Buffer, const char* _Format, ...) {
    int _Result;
    va_list args;
    va_start(args, _Format);
    _Result = vasprintf(_Buffer, _Format, args);
    va_end(args);
    return _Result;
}
