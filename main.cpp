#include <stdio.h>
#include <Windows.h>

int main(int argc, char** argv) {
    HWND hWesnothWnd = FindWindowW(NULL, L"The Battle for Wesnoth - 1.14.9");
    
    DWORD dwWesnothProcessId = 0;
    GetWindowThreadProcessId(hWesnothWnd, &dwWesnothProcessId);

    HANDLE hWesnothProcess = OpenProcess(PROCESS_ALL_ACCESS, true, dwWesnothProcessId);
    intptr_t lpPlayerProfile = 0x017EECB8;

    SIZE_T cbRead = 0;
    DWORD dwGold = 0;
    intptr_t lpBuff = lpPlayerProfile + 0x60;
    ReadProcessMemory(  hWesnothProcess,
                        (LPCVOID)lpBuff,
                        &lpBuff,
                        4,
                        &cbRead);
    ReadProcessMemory(  hWesnothProcess,
                        (LPCVOID)(lpBuff + 0xA90),
                        &lpBuff,
                        4,
                        &cbRead);
    ReadProcessMemory(  hWesnothProcess,
                        (LPCVOID)(lpBuff + 0x4),
                        &dwGold,
                        4,
                        &cbRead);

    printf("cbRead: %lld\n", cbRead);
    printf("dwGold: %ld\n", dwGold);

    DWORD dwGoldNewValue = 999;
    SIZE_T cbWritten = 0;
    WriteProcessMemory( hWesnothProcess,
                        (LPVOID)(lpBuff + 0x4),
                        &dwGoldNewValue,
                        4,
                        &cbWritten);

    printf("cbWritten: %lld\n", cbWritten);

    return 0;
}