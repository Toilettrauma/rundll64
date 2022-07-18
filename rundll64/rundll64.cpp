// python-change-color.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>
#include <exception>

#include <list>
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

typedef pair<int, char**> argsPair;

typedef pair<char*, argsPair> methodPair;
typedef pair<char*, list<methodPair>> libMethodsPair;

int getArgsCount(int argc, char** argv) {
    char** argvIter = argv;
    int argcIter = 0;
    for (;argcIter != argc && **argvIter != '#' && **argvIter != '@'; argvIter++, argcIter++) {}
    return argcIter;
}

list<methodPair>* orderMethods(char* method, int argc, char** argv) {
    list<methodPair>* orderedMethods = new list<methodPair>;
    char** argvIter = argv;
    int argcIter = 1;
    for (; argcIter <= argc && **argvIter != '@'; argvIter++, argcIter++) {
        if (**argvIter == '#') {
            (*argvIter)++;
            int argCount = getArgsCount(argc - argcIter, argvIter + 1);
            argcIter += argCount;
            argsPair args = make_pair(argCount, argvIter + 1);
            orderedMethods->push_back(make_pair(*argvIter, args));
        }
    }
    return orderedMethods;
}

list<libMethodsPair>* orderLibs(int argc, char** argv) {
    list<libMethodsPair>* orderedLibs = new list<libMethodsPair>;
    char** argvIter = argv;
    int argcIter = 1;
    for (; argcIter <= argc; argvIter++, argcIter++) {
        if (**argvIter == '@') {
            (*argvIter)++;
            list<methodPair>* orderedMethods = orderMethods(*argvIter, argc - argcIter, argvIter + 1);
            orderedLibs->push_back(make_pair(*argvIter, *orderedMethods));
        }
    }
    return orderedLibs;
}

int applyArgs(char** string, int size, long long callOut) {
    for (int i = 0; i < size; i++) {
        if (*string[i] == '%') {
            char* formatString = new char[16];
            int out = snprintf(formatString, 16, "%lld", callOut);
            string[i] = formatString;
            return out;
        }
    }
    return -1;
}

extern "C" void* __fastcall callF(void* functionP, long long argc, char* argv[]);

int main(int argc, char* argv[])
{
    _set_se_translator(handler);

    //int functionCallConvention;
    //if (strcmp(argv[1], "__fastcall") == 0) {
    //    argc--;
    //    argv++;
    //    functionCallConvention = CallingConvention::FASTCALL;
    //}
    //else if (strcmp(argv[1], "__stdcall") == 0) {
    //    argc--;
    //    argv++;
    //    functionCallConvention = CallingConvention::STDCALL;
    //}
    //else {
    //    functionCallConvention = CallingConvention::STDCALL;
    //}

    list<libMethodsPair>* orderedLibs = orderLibs(argc, argv);



    for (list<libMethodsPair>::iterator libIter = orderedLibs->begin(); libIter != orderedLibs->end(); libIter++) {
        HMODULE lib = LoadLibraryA(libIter->first);
        if (lib == nullptr) {
            throw LibNotFoundException(libIter->first);
        }
        bool isFirst = true;
        void* out = nullptr;
        for (list<methodPair>::iterator methodIter = libIter->second.begin(); methodIter != libIter->second.end(); methodIter++) {
            FARPROC method = GetProcAddress(lib, methodIter->first);
            if (method == nullptr) {
                throw MethodNotFoundException(methodIter->first);
            }
            if (!isFirst) {
                applyArgs(methodIter->second.second, methodIter->second.first, (long long)out);
            }
            out = callF(method, methodIter->second.first, methodIter->second.second);
            try {
                printf("%s -> %lld\n", methodIter->first, *(long long*)out);
            }
            catch (AccessViolationException) {
                printf("%s -> 0x%llX\n", methodIter->first, out);
            }
            isFirst = false;
        }
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
