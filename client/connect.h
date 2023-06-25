#pragma once
#include <iostream>
#include <winsock2.h> 
#pragma comment(lib, "WS2_32.lib")

class Connect {
public:
	void establish_connection();
	void send_file(SOCKET server, const std::string& file_name);
	int send_buffer(SOCKET server, const char* buffer, int buffer_size);

private:
	void install_settings();
	
	unsigned int port;
	std::string ip_address;
};