#pragma once
#include <WS2tcpip.h>
#include <sys/types.h>
#include <optional>
#include <vector>
#include <string>
#include<iostream>

#pragma comment(lib, "ws2_32.lib")

const char* const TELLO_SERVER_IP{ "192.168.10.1" };
const char* const TELLO_SERVER_COMMAND_PORT{ "8889" };

const int LOCAL_CLIENT_COMMAND_PORT{ 8889 };
const int LOCAL_SERVER_STATE_PORT{ 8890 };

class Tello
{
public:
	Tello();
	~Tello();
	int initializeWinSocket();
	bool BindCommand();
	bool BindState();
	bool SendCommand(const std::string& command);
	std::pair<bool,std::string> ReceiveCommandResponse();
	std::string GetState();
	Tello(const Tello&) = delete;
	Tello(const Tello&&) = delete;

private:
	SOCKET m_command_sockfd;
	SOCKET  m_state_sockfd;
	sockaddr_storage m_tello_server_command_addr{};
	sockaddr_storage m_tello_server_state_addr{};

	void FindTello();
	std::pair<bool, std::string> BindStatusSocketToPort();
	std::pair<bool, std::string> BindCommandSocketToPort();
	std::pair<bool, std::string> FindSocketAddr(const char* const ip, 
		const char* const port, sockaddr_storage* const addr);
	std::pair<int, std::string> ReceiveFrom(const SOCKET sockfd);
	void ShowStatus(const std::string& state);

};

