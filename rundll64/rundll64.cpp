// python-change-color.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>
#include <exception>

#include <list>
#include <vector>
using namespace std;

//enum CallingConvention {
//    //CDECL, unsupported
//    //CLRCALL, unsupported
//    STDCALL,
//    FASTCALL,
//    //THISCALL, unsupported
//    //VECTORCALL unsupported
//};

struct LibNotFoundException : std::exception {
    LibNotFoundException(const char* const libName) : std::exception(libName) {}
};
struct MethodNotFoundException : std::exception {
    MethodNotFoundException(const char* const methodName) : std::exception(methodName) {}
};

struct AccessViolationException : std::exception {
    AccessViolationException() : std::exception("Access violation") {}
};

void handler(unsigned int u, PEXCEPTION_POINTERS pExceptionInfo) {
    if (u == EXCEPTION_ACCESS_VIOLATION) {
        throw AccessViolationException();
    }
    else {
        throw std::exception("SEH exception");
    }
}

extern "C" void* __fastcall callF(void* functionP, long long argc, char* argv[]);

vector<void*>* invokeLibMethods(int argc, char* argv[]) {
    HMODULE lib = nullptr;
    FARPROC method = nullptr;
    char** methodArgsStart = nullptr;
    int argCount = 0;
    vector<void*> *outs = new vector<void*>;
    for (int i = 0; i < argc; i++) {
        char* arg = argv[i];
        switch (*arg) {
        case '@':
            if (lib != nullptr) {
                method = nullptr;
                FreeLibrary(lib);
            }
            lib = LoadLibraryA(arg + 1);
            if (lib == nullptr) {
                throw LibNotFoundException(arg + 1);
            }
            break;
        case '#':
            if (lib != nullptr) {
                if (method != nullptr) {
                    outs->push_back(callF(method, argCount, methodArgsStart));
                }
                argCount = 0;
                method = GetProcAddress(lib, arg + 1);
                if (method == nullptr) {
                    throw MethodNotFoundException(arg + 1);
                }
                if (i + 1 < argc) {
                    methodArgsStart = argv + i + 1;
                }
            }
            break;
        case '%':
        {
            char* formattedArg = new char[16];
            long long outToFormat = (long long)(*outs)[strtol(arg + 1, nullptr, 10)];
            snprintf(formattedArg, 16, "%lld", outToFormat);
            argv[i] = formattedArg;
        }
        default:
            argCount++;
            break;
        }
    }
    if (method != nullptr) {
        outs->push_back(callF(method, argCount, methodArgsStart));
    }
    if (lib != nullptr) {
        FreeLibrary(lib);
    }
    return outs;
}

int main(int argc, char* argv[])
{
    _set_se_translator(handler);

    try {
        vector<void*>* outs = invokeLibMethods(argc, argv);
    }
    catch (LibNotFoundException e) {
        printf("Lib not found: %s", e.what());
    }
    catch (MethodNotFoundException e) {
        printf("Method not found: %s", e.what());
    }
    catch (std::exception e) {
        printf("Unknown exception: %s", e.what());
    }


    return 0;

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
