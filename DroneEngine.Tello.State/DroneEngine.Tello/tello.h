#pragma once
#include <WS2tcpip.h>
#include <sys/types.h>
#include <optional>
#include <vector>
#include <string>

#pragma comment(lib, "ws2_32.lib")

const char* const TELLO_SERVER_IP{ "192.168.10.1" };
const char* const TELLO_SERVER_COMMAND_PORT{ "8889" };

const int LOCAL_CLIENT_COMMAND_PORT{ 9000 };
const int LOCAL_SERVER_STATE_PORT{ 8890 };

class Tello
{
public:
	Tello();
	~Tello();
	int initializeWinSocket();
	bool Bind(int local_client_command_port = LOCAL_CLIENT_COMMAND_PORT);
	bool SendCommand(const std::string& command);
	std::pair<bool,std::string> ReceiveResponse();
	std::string GetState();
	Tello(const Tello&) = delete;
	Tello(const Tello&&) = delete;

private:
	SOCKET m_command_sockfd;
	SOCKET  m_state_sockfd;
	int m_local_client_command_port{ LOCAL_CLIENT_COMMAND_PORT };
	sockaddr_storage m_tello_server_command_addr{};

	void FindTello();
	void ShowTelloInfo(std::string& command);
	std::pair<bool, std::string> BindStatusSocketToPort(); //const SOCKET sockfd, const int port
	std::pair<bool, std::string> BindCommandSocketToPort();
	std::pair<bool, std::string> FindSocketAddr(const char* const ip, 
		const char* const port, sockaddr_storage* const addr);
	std::pair<int, std::string> ReceiveFrom(const SOCKET sockfd,
		sockaddr_storage& addr,
		std::vector<char>& buffer, 
		const int buffer_size = 1024);

};

