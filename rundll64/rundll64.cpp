#include <iostream>
#include <vector>

#ifdef _WIN32
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>
#else
// should work but not tested
#include <dlfcn.h>
#define LoadLibraryA(name) dlopen(name, RTLD_LAZY)
#define GetProcAddress(handle, name) dlsym(handle, name)
#define CloseLibrary(handle) dlclose(handle)

typedef HMODULE void*;
typedef void(*FARPROC)();
#endif

#include "options_call.h"
#include "exceptions.h"

#pragma unmanaged
void SEHandler(unsigned int u, PEXCEPTION_POINTERS pExceptionInfo) {
    if (u == EXCEPTION_ACCESS_VIOLATION) {
        throw AccessViolationException(pExceptionInfo->ExceptionRecord->ExceptionAddress, pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
    }
    else {
        throw SEException(u);
    }
}
#pragma managed

long long* createApplyFArgs(const char** argv, const long long argc) {
    long long* out = new long long [argc];
    char* err;
    long long argLL;
    for (long long i = 0; i < argc; ++i) {
        argLL = strtoll(argv[i], &err, 0);
        if (err[0] == 0) {
            out[i] = argLL;
            continue;
        }
        out[i] = reinterpret_cast<long long>(argv[i]);
    }
    return out;
}

#pragma unmanaged
void* applyF_handler(void* function, const char** argv, long long argc) {
    void* out = nullptr;
    __try {
        long long* procArgs = createApplyFArgs(argv, argc);
        out = applyF(function, procArgs, argc);
        delete[] procArgs;
    }
    __except (STATUS_HEAP_CORRUPTION) {
        printf("Error\n");
    }
    return out;
}
#pragma managed

std::vector<uintptr_t> invokeLibMethods(int argc, char** argv) {
    std::vector<uintptr_t> outs;
    std::vector<char*> formattedOuts;
    char* arg, *formattedArg, *formatErr;
    const char** procArgv = nullptr;
    long long *procArgs, procArgc = 0, formatIndex, formattedTmp;
    HMODULE module = nullptr;
    char* moduleName = nullptr;
    FARPROC proc = nullptr;
    
    for (int i = 0; i < argc; ++i) {
        arg = argv[i];
        switch (arg[0]) {
        case '@':
        {
            if (module != nullptr) {
                FreeLibrary(module);
            }
            module = LoadLibraryA(arg + 1);
            moduleName = arg + 1;
            if (module == nullptr) {
                throw LibNotFoundException(moduleName);
            }
            break;
        }
        case '#':
        {
            if (module != nullptr) {
                if (proc != nullptr) {
                    outs.push_back(reinterpret_cast<uintptr_t>(applyF_handler(proc, procArgv, procArgc)));
                }
                proc = GetProcAddress(module, arg + 1);
                if (proc == nullptr) {
                    throw MethodNotFoundException(moduleName, arg + 1);
                }
                procArgv = const_cast<const char**>(argv + i + 1);
                procArgc = 0;
            }
            break;
        }
        case '%':
        {
            formattedOuts.resize(outs.size());
            formatIndex = strtoll(arg + 1, &formatErr, 10);
            if (formatErr[0] != 0) {
                throw FormatErrorException(i);
            }
            if (formatIndex >= outs.size()) {
                throw FormatErrorException(i);
            }
            if (formattedOuts.at(formatIndex) != nullptr) {
                argv[i] = formattedOuts.at(formatIndex);
            }
            formattedTmp = outs.at(formatIndex); // todo add check
            asprintf(formattedArg, "%lld", formattedTmp);
            formattedOuts.push_back(formattedArg);
            argv[i] = formattedArg;
            ++procArgc;
            break;
        }
        default:
            ++procArgc;
            break;
        }
    }
    if (proc != nullptr) {
        applyF_handler(proc, procArgv, procArgc);
    }
    if (module != nullptr) {
        FreeLibrary(module);
    }

    for (char* arg : formattedOuts) {
        delete[] arg;
    }

    return outs;
}

int main(int argc, char** argv) {
	_set_se_translator(SEHandler);

	try {
		std::vector<uintptr_t> outs = invokeLibMethods(argc, argv);
    }
    catch (FormatException& exception) {
        printf("%s\n", exception.what());
        return -1;
    }
    return 0;
}