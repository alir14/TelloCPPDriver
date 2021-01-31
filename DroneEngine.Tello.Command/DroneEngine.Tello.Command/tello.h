#pragma once
#include <WS2tcpip.h>
#include <vector>
#include <string>
#include<iostream>

#pragma comment(lib, "ws2_32.lib")


const char* const TELLO_SERVER_IP = "192.168.10.1";
const char* const TELLO_SERVER_COMMAND_PORT = "8889";

const int CLIENT_COMMAND_PORT = 8889;

class Tello
{
public:
	Tello();
	~Tello();
	void initializeWinSocket();
	bool BindAndConnect();
	bool SendCommand(const std::string& command);
	std::pair<bool, std::string> ReceiveResponse();
	
private:
	SOCKET command_socket;
	sockaddr_storage server_command_addr;

	std::pair<bool, std::string> BindSocket();
	std::pair<bool, std::string> GetSocketAddr();
	void SetTelloToCommandMode();
};

