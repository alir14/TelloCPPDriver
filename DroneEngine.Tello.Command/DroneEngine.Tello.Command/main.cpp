#include <iostream>
#include <conio.h> //this header is needed to use _getch()
#include <string>;
#include "tello.h"

int main()
{
	int inputChar;
	std::string inputValue;
	Tello tello;

	if (!tello.BindAndConnect()) {
		std::cout << "failed to connect and send command!\n";
		return 0;
	}

	puts("enter value , z will exit.");

	do
	{
		inputChar = _getch();

		switch (inputChar)
		{
		case 't':
			std::cout << "takeoff" << std::endl;
			tello.SendCommand("takeoff");
			break;
		case 'l':
			std::cout << "land" << std::endl;
			tello.SendCommand("land");
			break;
		case 'w':
			std::cout << "forward" << std::endl;
			tello.SendCommand("forward 45");
			break;
		case 's':
			std::cout << "backward" << std::endl;
			tello.SendCommand("back 45");
			break;
		case 'a':
			std::cout << "left" << std::endl;
			tello.SendCommand("left 45");
			break;
		case 'd':
			std::cout << "right" << std::endl;
			tello.SendCommand("right 45");
			break;
		case 'q':
			std::cout << "turn left" << std::endl;
			tello.SendCommand("ccw 45");
			break;
		case 'e':
			std::cout << "turn right" << std::endl;
			tello.SendCommand("cwt 45");
			break;
		case 'c':
			std::cout << "custom command" << std::endl;
			std::getline(std::cin, inputValue);
			std::cout << "sending: " << inputValue << std::endl;
			tello.SendCommand(inputValue);
			break;
		default:
			std::cout << "unknown command: " << inputChar << std::endl;
			break;
		}

		auto response = tello.ReceiveResponse();

		if (!response.first) {
			std::cout << response.second << std::endl;
		}

	} while (inputChar != 'z');

	std::cout << "Closing .... \n";

	return 0;

}
