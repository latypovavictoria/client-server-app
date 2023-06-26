#pragma once
#include <iostream>
#include <windows.h>

class Function {
public:
	std::string follow_activity();
	void get_screenshot();

private:
	int encoder_picture(const WCHAR* format, CLSID* p_cls_id);
	BOOL get_user_domain(PTSTR szUser, PDWORD pcchUser,
		PTSTR szDomain, PDWORD pcchDomain);
};