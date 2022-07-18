// python-change-color.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>

#include <tuple>

enum CallingConvention {
    //CDECL, unsupported
    //CLRCALL, unsupported
    STDCALL,
    FASTCALL,
    //THISCALL, unsupported
    //VECTORCALL unsupported
};
#define QWORDPTR *(long long*)
#define DWORDPTR *(int*)
#define BYTEPTR *(char*)

void handler(unsigned int u, PEXCEPTION_POINTERS pExceptionInfo) {
    throw std::exception("Exception");
}

extern "C" void* __fastcall callF(void* functionP, long long argc, char* argv[]);

int main(int argc, char *argv[])
{
    if (argc < 3) {
        return -1;
    }
    
    int functionCallConvention;
    if (strcmp(argv[1], "__fastcall") == 0) {
        argc--;
        argv++;
        functionCallConvention = CallingConvention::FASTCALL;
    }
    else if (strcmp(argv[1], "__stdcall") == 0) {
        argc--;
        argv++;
        functionCallConvention = CallingConvention::STDCALL;
    }
    else {
        functionCallConvention = CallingConvention::STDCALL;
    }

    HMODULE moduleLib = LoadLibraryA(argv[1]);
    if (moduleLib == nullptr) {
        return -1;
    }
    FARPROC moduleProc = GetProcAddress(moduleLib, argv[2]);
    if (moduleProc == nullptr) {
        return -1;
    }


    void* out = callF(moduleProc, argc - 3, argv + 3);
    _set_se_translator(handler);
    try {
        int y = 0;
        int a = 1 / y;
        printf("%ll", QWORDPTR out);
    }
    catch (std::exception e) {
        printf("Exception");
        return -1;
    }
    
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
