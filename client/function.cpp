#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "gdiplus.lib")
#include <chrono>
#include "function.h"
#include <conio.h>
#include <gdiplus.h>
#include <tchar.h>
#include<Strsafe.h>

int Function::encoder_picture(const WCHAR* format, CLSID* p_cls_id) {
	unsigned int num = 0;
	unsigned int size = 0;
	Gdiplus::ImageCodecInfo* p_image = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0) {
		return -1;
	}

	p_image = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (p_image == NULL) {
		return -1;
	}

	Gdiplus::GetImageEncoders(num, size, p_image);

	for (UINT j = 0; j < num; ++j) {
		if (wcscmp(p_image[j].MimeType, format) == 0) {
			*p_cls_id = p_image[j].Clsid;
			free(p_image);
			return j;
		}
	}

	free(p_image);
	return -1;
}

void Function::get_screenshot() {
	HDC hdcScreen = GetDC(NULL);
	HDC hdcMemDC = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmScreen = NULL;

	Gdiplus::GdiplusStartupInput gdip;
	ULONG_PTR gdipToken;
	Gdiplus::GdiplusStartup(&gdipToken, &gdip, NULL);

	hbmScreen = CreateCompatibleBitmap(hdcScreen, 3000, 2000);
	SelectObject(hdcMemDC, hbmScreen);

	BitBlt(hdcMemDC, 0, 0, 3000, 2000, hdcScreen, 0, 0, SRCCOPY);

	CLSID encoderID;

	encoder_picture(L"image/png", &encoderID);

	Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(hbmScreen, (HPALETTE)0);
	bmp->Save(L"screen.png", &encoderID, NULL);
	Gdiplus::GdiplusShutdown(gdipToken);

	DeleteObject(hbmScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);
}

std::string Function::follow_activity() {
	POINT cursor_value_first;
	POINT cursor_value_last;

	TCHAR user[1024], domain[1024];
	DWORD chUser = sizeof(user), chDomain = sizeof(domain);

	BOOL result = get_user_domain(user, &chUser, domain, &chDomain);

	std::string result_string_user, result_string_domain;
#ifndef UNICODE
	result_string_user = user;
	result_string_domain = domain;
#else
	std::wstring temp_user = user;
	result_string_user = std::string(temp_user.begin(), temp_user.end());

	std::wstring temp_domain = domain;
	result_string_domain = std::string(temp_domain.begin(), temp_domain.end());
#endif
	
	std::string result_string = result_string_user + result_string_domain;
	GetCursorPos(&cursor_value_first);
	Sleep(5000);
	GetCursorPos(&cursor_value_last);

	if (cursor_value_first.x == cursor_value_last.x && cursor_value_first.y == cursor_value_last.y) {
		auto now_time = std::chrono::system_clock::now();
		std::time_t last_time_act = std::chrono::system_clock::to_time_t(now_time);
		return std::ctime(&last_time_act) + result_string_user + result_string_domain;
	}
	else {
		return "User is working now" + result_string_user + result_string_domain;
	}
}

BOOL Function::get_user_domain(PTSTR szUser, PDWORD pcchUser,
	PTSTR szDomain, PDWORD pcchDomain) {

	BOOL         fSuccess = FALSE;
	HANDLE       hToken = NULL;
	PTOKEN_USER  ptiUser = NULL;
	DWORD        cbti = 0;
	SID_NAME_USE snu;

	__try {

		if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE,
			&hToken)) {

			if (GetLastError() != ERROR_NO_TOKEN)
				__leave;

			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY,
				&hToken))
				__leave;
		}

		if (GetTokenInformation(hToken, TokenUser, NULL, 0, &cbti)) {

			__leave;

		}
		else {

			if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
				__leave;
		}

		ptiUser = (PTOKEN_USER)HeapAlloc(GetProcessHeap(), 0, cbti);
		if (!ptiUser)
			__leave;

		if (!GetTokenInformation(hToken, TokenUser, ptiUser, cbti, &cbti))
			__leave;

		if (!LookupAccountSid(NULL, ptiUser->User.Sid, szUser, pcchUser,
			szDomain, pcchDomain, &snu))
			__leave;

		fSuccess = TRUE;

	}
	__finally {
		if (hToken)
			CloseHandle(hToken);

		if (ptiUser)
			HeapFree(GetProcessHeap(), 0, ptiUser);
	}

	return fSuccess;
}