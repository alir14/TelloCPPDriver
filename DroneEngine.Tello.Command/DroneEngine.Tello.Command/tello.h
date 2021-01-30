#pragma once
#include <WS2tcpip.h>
#include <sys/types.h>
#include <vector>
#include <string>
#include<iostream>

#pragma comment(lib, "ws2_32.lib")


const char* const TELLO_SERVER_IP = "192.168.10.1";
const char* const TELLO_SERVER_COMMAND_PORT = "8889";

const int LOCAL_CLIENT_COMMAND_PORT = 8889;

class Tello
{
public:
	Tello();
	~Tello();
	int initializeWinSocket();
	bool BinCommandSocket();
	bool SendCommand(const std::string& command);
	std::pair<bool, std::string> ReceiveResponse();
	
private:
	SOCKET command_socket;
	sockaddr_storage server_command_addr;

	void ConnectToTello();
	std::pair<bool, std::string> FindSocketAddr(const char* const ip, const char* const port, sockaddr_storage* const addr);
};

