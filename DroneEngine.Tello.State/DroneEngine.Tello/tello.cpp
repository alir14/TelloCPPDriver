#include "tello.h"
#include<iostream>
#include <errno.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sstream>

#pragma comment (lib, "ws2_32.lib")

Tello::Tello() {
	//start up winsock
	int result = initializeWinSocket();
}

Tello::~Tello() {
	std::cout << "closing sockets ....";
	closesocket(m_command_sockfd);
	closesocket(m_state_sockfd);

	WSACleanup();
}

int Tello::initializeWinSocket() {
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOK = WSAStartup(version, &data);

	if (wsOK != 0) {
		std::cout << "cannot start Winsock" << wsOK;
		return -1;
	}

	return 1;
}

bool Tello::BindCommand() {
	auto bind_result = BindCommandSocketToPort(); 
	if (!bind_result.first) {
		std::cout << bind_result.second << std::endl;
		return false;
	}

	bind_result = FindSocketAddr(TELLO_SERVER_IP, TELLO_SERVER_COMMAND_PORT, &m_tello_server_command_addr);
	if (!bind_result.first) {
		std::cout << bind_result.second << std::endl;
		return false;
	}

	std::cout << "Finding Tello ..." << std::endl;
	FindTello();
	std::cout << "Entered SDK mode ..." << std::endl;

	return true;

}

bool Tello::SendCommand(const std::string& command) {
	
	const socklen_t addr_len{ sizeof(m_tello_server_command_addr) };
	int result = sendto(m_command_sockfd, command.c_str(), command.size(), 0, 
		(sockaddr*)&m_tello_server_command_addr, addr_len);


	const int bytes{ result };
	if (bytes == -1) {
		std::cout << "failed to send the message ... " << errno << std::endl;
		return false;
	}

	std::cout << "127.0.0.1:" << "????" << " >>>> "
		<< bytes << " bytes >>>> " << TELLO_SERVER_IP << ":" << TELLO_SERVER_COMMAND_PORT << ":" << command;

	return true;
}

std::pair<bool, std::string> Tello::ReceiveCommandResponse() {
	const int size{ 1024 };
	std::vector<char> buffer(size, '\0');
	const auto result = ReceiveFrom(m_command_sockfd);

	const int bytes{ result.first };

	if (bytes < 1) {
		return {false, "failed to recive ...."};
	}

	std::string response{ buffer.cbegin(), buffer.cbegin() + bytes };
	response.erase(response.find_last_not_of(" \n\r\t") + 1);

	std::cout << "127.0.0.1: " << "??????" << " <<<< " << bytes <<
		" bytes " << "<<<< " << TELLO_SERVER_IP <<":" << TELLO_SERVER_COMMAND_PORT << 
		" " << response << std::endl;

	return { true, "" };
}

bool Tello::BindState() {
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

		int bytesIn = recvfrom(m_state_sockfd, buff, 1024, (sockaddr*)&client, &clientLength);

		if (bytesIn == SOCKET_ERROR) {
			std::cout << "error receiving from client " << WSAGetLastError() << std::endl;
			continue;
		}

		char clientIp[256];
		ZeroMemory(clientIp, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		std::cout << "drone: " << clientIp << "state :" << buff << std::endl;
	}
	
}

std::pair<bool, std::string> Tello::BindStatusSocketToPort() {

	m_state_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in listen_addr{ };
	listen_addr.sin_port = htons(9000);
	listen_addr.sin_addr.S_un.S_addr = ADDR_ANY;
	listen_addr.sin_family = AF_INET;
	int result = bind(m_state_sockfd, (sockaddr*)&listen_addr, sizeof(listen_addr));

	if (result == SOCKET_ERROR) {
		std::cout << "cannot bind socket " << WSAGetLastError() << std::endl;
		return { false, "cannot bind socket" };
	}

	return { true, "" };
}

std::pair<bool, std::string> Tello::BindCommandSocketToPort() {

	m_command_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in listen_addr{ };
	listen_addr.sin_port = htons(8890);
	listen_addr.sin_addr.S_un.S_addr = ADDR_ANY;
	listen_addr.sin_family = AF_INET;
	int result = bind(m_command_sockfd, (sockaddr*)&listen_addr, sizeof(listen_addr));

	if (result == SOCKET_ERROR) {
		std::cout << "cannot bind socket " << WSAGetLastError() << std::endl;
		return { false, "cannot bind socket" };
	}

	return { true, "" };
}

std::pair<bool, std::string> Tello::FindSocketAddr(const char* const ip,
	const char* const port, sockaddr_storage* const addr) {
	addrinfo* result_list{ nullptr };
	addrinfo hints{};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	int result = getaddrinfo(ip, port, &hints, &result_list);

	if (result != 0) {
		std::cout << "getaddrinfo " << result << std::endl;
		return { false, "cannot find the socket address" };
	}

	memcpy(addr, result_list->ai_addr, result_list->ai_addrlen);
	freeaddrinfo(result_list);

	return { true, "" };
}

void Tello::FindTello() {
	do {
		SendCommand("command");
		Sleep(1);
	} while ((!ReceiveCommandResponse().first));
}

std::pair<int, std::string> Tello::ReceiveFrom(const SOCKET sockfd) {

	sockaddr_in client;
	int clientSize = sizeof(client);
	ZeroMemory(&client, clientSize);

	char buf[1024];

	ZeroMemory(buf, 1024);
	while (true)
	{
		int byteIn = recvfrom(sockfd, buf, 1024, 0, (sockaddr*)&client, &clientSize);

		if (byteIn == SOCKET_ERROR) {
			std::cout << "recfrom: " << errno << std::endl;
			return { -1 , "failed to receive message" };
		}

		std::cout << "msg received: " << buf << std::endl;

		std::string resultMsg(buf);

		if(byteIn > 1)
			return { byteIn, resultMsg };
	}
}