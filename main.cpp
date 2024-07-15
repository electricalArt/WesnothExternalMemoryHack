/*
    A console application that sets the player's gold in Wesnoth 1.14.9 to the value of 999 when run. 
    It makes use of ReadProcessMemory and WriteProcessMemory to achieve this. The address 0x017EED18
    represents the player's base pointer in Wesnoth. Finding the address and offsets are covered in
    https://gamehacking.academy/lesson/13
    
    This program must be run as an administrator.
*/

// FindWindow, GetWindowThreadProcessId, OpenProcess, ReadProcessMemory, and WriteProcessMemory are all contained inside windows.h
#include <stdio.h>
#include <Windows.h>

int main(int argc, char** argv) {
    /*
        To use ReadProcessMemory and WriteProcessMemory, we require a handle to the Wesnoth process.
        
        To get this handle, we require a process id. The quickest way to get a process id for a particular 
        process is to use GetWindowThreadProcessId.
        
        GetWindowThreadProcessId requires a window handle (different than a process handle). To get this 
        window handle, we use FindWindow.

        Using debugger, it is known that pointer to `Gold` value can be accessed at
        [[0x017EECB8 + 0x60] + 0xA90] + 0x4. `0x017EECB8` is static pointer to what seems to be `Player profile`.
    */
    
    // Find our Wesnoth window. Depending on your language settings, this might be different.
    HWND hWesnothWnd = FindWindowW(NULL, L"The Battle for Wesnoth - 1.14.9");
    
    // Get the process id for the Wesnoth process. GetWindowThreadProcessId does not return a process id, but
    // rather fills a provided variable with its value, hence the &.
    DWORD dwWesnothProcessId = 0;
    GetWindowThreadProcessId(hWesnothWnd, &dwWesnothProcessId);

    // Open our Wesnoth process. PROCESS_ALL_ACCESS means we can both read and write to the process. However,
    // it also means that this program must be executed as an administrator to work.
    HANDLE hWesnothProcess = OpenProcess(PROCESS_ALL_ACCESS, true, dwWesnothProcessId);
    intptr_t lpPlayerProfile = 0x017EECB8;

    // Read the value at `lpPlayerProfile` with offest and place its value into a variable.
    SIZE_T cbRead = 0;
    DWORD dwGold = 0;
    intptr_t lpBuff = lpPlayerProfile + 0x60;
    ReadProcessMemory(  hWesnothProcess,
                        (LPCVOID)lpBuff,
                        &lpBuff,
                        4,
                        &cbRead);
    // Add 0xA90 to the value read from the last step and then read the value at that new address.
    ReadProcessMemory(  hWesnothProcess,
                        (LPCVOID)(lpBuff + 0xA90),
                        &lpBuff,
                        4,
                        &cbRead);
    // Add 4 to the gold_value, which will then be pointing at the player's current `Gold` address.
    ReadProcessMemory(  hWesnothProcess,
                        (LPCVOID)(lpBuff + 0x4),
                        &dwGold,
                        4,
                        &cbRead);

    printf("cbRead: %lld\n", cbRead);
    printf("dwGold: %ld\n", dwGold);

    // Write the new `Gold` value (999) into this address.
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