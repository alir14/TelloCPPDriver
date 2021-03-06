#include "tello.h"
#include <errno.h>
#include <memory.h>

#pragma comment (lib, "ws2_32.lib")

Tello::Tello() {
	//start up winsock
	initializeWinSocket();
}

Tello::~Tello() {
	std::cout << "closing sockets ....";
	closesocket(command_sockfd);
	closesocket(state_sockfd);

	WSACleanup();
}

void Tello::initializeWinSocket() {
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOK = WSAStartup(version, &data);

	if (wsOK != 0) {
		throw "cannot start socket ...";
	}
}

bool Tello::BindSocketAndConnect() {
	auto result = BindCommandSocketToPort();
	if (!result.first) {
		std::cout << result.second << std::endl;
		return false;
	}

	result = GetSocketAddr();
	if (!result.first) {
		std::cout << result.second << std::endl;
		return false;
	}

	std::cout << "Connecting Tello ..." << std::endl;
	SetTelloToCommandMode();
	std::cout << "Entered SDK mode ..." << std::endl;

	return true;

}

bool Tello::SendCommand(const std::string& command) {
	
	const socklen_t addr_len{ sizeof(tello_server_command_addr) };
	int result = sendto(command_sockfd, command.c_str(), command.size(), 0, 
		(sockaddr*)&tello_server_command_addr, addr_len);


	if (result == SOCKET_ERROR) {
		std::cout << "failed to send the message ... " << errno << std::endl;
		return false;
	}

	std::cout << "127.0.0.1:" << "????" << " >>>> "
		<< result << " bytes >>>> " << TELLO_SERVER_IP << ":" << TELLO_SERVER_COMMAND_PORT << ":" << command;

	return true;
}

bool Tello::BindStateSocket() {
	auto bind_result = BindStatusSocketToPort();

	if (!bind_result.first) {
		std::cout << bind_result.second << std::endl;
		return false;
	}

	return true;

}

std::string Tello::GetState() {

	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);

	char buff[1024];

	while (true)
	{
		ZeroMemory(buff, 1024);

		int bytesIn = recvfrom(state_sockfd, buff, 1024, 0, (sockaddr*)&client, &clientLength);

		if (bytesIn == SOCKET_ERROR) {
			std::cout << "error receiving from client " << WSAGetLastError() << std::endl;
			continue;
		}

		char clientIp[256];
		ZeroMemory(clientIp, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		std::cout << "drone: " << clientIp << "state :" << buff << std::endl;

		ShowStatus(buff);
	}
}

std::pair<bool, std::string> Tello::BindStatusSocketToPort() {
	state_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in listen_addr{ };
	listen_addr.sin_port = htons(SERVER_STATE_PORT);
	listen_addr.sin_addr.S_un.S_addr = ADDR_ANY;
	listen_addr.sin_family = AF_INET;
	int result = bind(state_sockfd, (sockaddr*)&listen_addr, sizeof(listen_addr));

	if (result == SOCKET_ERROR) {
		std::cout << "cannot bind socket " << WSAGetLastError() << std::endl;
		return { false, "cannot bind socket" };
	}

	return { true, "bind socket - OK" };
}

std::pair<bool, std::string> Tello::BindCommandSocketToPort() {

	command_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in listen_addr{ };
	listen_addr.sin_port = htons(CLIENT_COMMAND_PORT);
	listen_addr.sin_addr.S_un.S_addr = ADDR_ANY;
	listen_addr.sin_family = AF_INET;
	int result = bind(command_sockfd, (sockaddr*)&listen_addr, sizeof(listen_addr));

	if (result == SOCKET_ERROR) {
		std::cout << "cannot bind socket " << WSAGetLastError() << std::endl;
		return { false, "cannot bind socket" };
	}

	return { true, "socket bind - OK" };
}

std::pair<bool, std::string> Tello::GetSocketAddr() {
	addrinfo* result_list{ nullptr };
	addrinfo hints{};

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	int result = getaddrinfo(TELLO_SERVER_IP, TELLO_SERVER_COMMAND_PORT, &hints, &result_list);

	if (result != 0) {
		std::cout << "getaddrinfo " << result << std::endl;
		return { false, "cannot find the socket address" };
	}

	memcpy(&tello_server_command_addr, result_list->ai_addr, result_list->ai_addrlen);
	freeaddrinfo(result_list);

	return { true, "" };
}

void Tello::SetTelloToCommandMode() {
	do {
		SendCommand("command");
		Sleep(1);
	} while ((!ReceiveCommandResponse().first));
}

std::pair<bool, std::string> Tello::ReceiveCommandResponse() {
	sockaddr_in client;
	int clientSize = sizeof(client);
	ZeroMemory(&client, clientSize);

	char buf[1024];

	ZeroMemory(buf, 1024);
	while (true)
	{
		int byteIn = recvfrom(command_sockfd, buf, 1024, 0, (sockaddr*)&client, &clientSize);

		if (byteIn == SOCKET_ERROR) {
			std::cout << "recfrom: " << errno << std::endl;
			return { -1 , "failed to receive message" };
		}

		std::cout << "msg received: " << buf << std::endl;

		std::string resultMsg(buf);

		resultMsg.erase(resultMsg.find_last_not_of(" \n\r\t") + 1);

		if(byteIn > 1)
			return { true, resultMsg };
		else
			return { false, "failed to recieve ...." };
	}
}

void Tello::ShowStatus(const std::string& state) {
	try
	{
		system("clear");

		size_t begin{ 0 };
		std::cout << "+-----------+-----------+" << std::endl;
		const int padding{ 10 };
		while (begin < state.size())
		{
			const auto split{ state.find(':', begin) };
			const auto name{ state.substr(begin, split - begin) };
			const auto end{ state.find(';', split) };
			const auto value{ state.substr(split + 1, end - split - 1) };

			begin = end + 1;

			std::cout << name << " : " << value << std::endl;

		}
		std::cout << "+-----------+-----------+" << std::endl;
	}
	catch (const std::exception& e) {
		std::cout << e.what();
	}
}