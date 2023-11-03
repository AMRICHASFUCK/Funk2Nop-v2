#include <iostream>
#include <windows.h>
#include <vector>
#include <ctime>
#include <cmath>

#pragma optimize( "", off )

using namespace std;

DWORD g_startAddress = 0;
DWORD g_endAddress = 0;

int TestFunction(int n)
{
    __asm {
        call label1
        label1 :
        pop g_startAddress
    }
    int result = 0;
    std::vector<int> values;

    for (int i = 1; i <= n; i++) {
        values.push_back(std::sin(i) * n + std::cos(n - i) * i);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result += values[i] * values[j] % (i + j + 1);
        }
    }

    int seed = static_cast<int>(std::time(nullptr));
    srand(seed);

    for (int i = 0; i < n; i++) {
        result ^= (rand() % 255);
    }
    __asm {
        call label2
        label2 :
        pop g_endAddress
    }
    return result;
}

void Funk2Nop(DWORD funcAddress, int functionSize)
{
    DWORD oldProtect;
    VirtualProtect((LPVOID)funcAddress, functionSize, PAGE_EXECUTE_READWRITE, &oldProtect);
    memset((LPVOID)funcAddress, 0x90, functionSize);
    VirtualProtect((LPVOID)funcAddress, functionSize, oldProtect, &oldProtect);
}

int main()
{
    int n;
    cout << "Enter Value: ";
    cin >> n;
    cout << TestFunction(n) << endl;
    int functionSize = g_endAddress - g_startAddress;

    std::cout << "Function size: " << functionSize << " bytes" << std::endl;

    Funk2Nop((DWORD)&TestFunction, functionSize);
    Sleep(-1);
}