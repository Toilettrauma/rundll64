#if false
#define ENABLE_DOUBLE
#endif

extern "C" void* __fastcall applyF(void* function, __int64* argv, long long argc);

// test
#if defined(ENABLE_DOUBLE)
extern "C" void* __fastcall applyF_double(char** argv_last, long long argc, void* functionP, unsigned long long doubleMap);
#define applyF applyF_double
#endif