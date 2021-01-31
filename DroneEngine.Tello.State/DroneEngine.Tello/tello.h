#pragma once
#include <WS2tcpip.h>
#include <vector>
#include <string>
#include<iostream>

#pragma comment(lib, "ws2_32.lib")

const char* const TELLO_SERVER_IP{ "192.168.10.1" };
const char* const TELLO_SERVER_COMMAND_PORT{ "8889" };

const int CLIENT_COMMAND_PORT{ 8889 };
const int SERVER_STATE_PORT{ 8890 };

class Tello
{
public:
	Tello();
	~Tello();
	void initializeWinSocket();
	bool BindSocketAndConnect();
	bool BindStateSocket();
	bool SendCommand(const std::string& command);
	std::string GetState();
	Tello(const Tello&) = delete;
	Tello(const Tello&&) = delete;

private:
	SOCKET command_sockfd;
	SOCKET  state_sockfd;
	sockaddr_storage tello_server_command_addr{};

	void SetTelloToCommandMode();
	std::pair<bool, std::string> BindStatusSocketToPort();
	std::pair<bool, std::string> BindCommandSocketToPort();
	std::pair<bool, std::string> GetSocketAddr();
	std::pair<bool, std::string> ReceiveCommandResponse();
	void ShowStatus(const std::string& state);

};

