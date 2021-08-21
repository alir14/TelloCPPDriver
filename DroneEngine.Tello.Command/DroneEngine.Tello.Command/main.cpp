#include <iostream>
#include <conio.h> //this header is needed to use _getch()
#include "tello.h"

int main()
{
	int inputChar;
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
				tello.SendCommand("rc 0 20 0 0");
				break;
			case 's':
				std::cout << "backward" << std::endl;
				tello.SendCommand("rc 0 -20 0 0");
				break;
			case 'a':
				std::cout << "left" << std::endl;
				tello.SendCommand("rc -20 0 0 0");
				break;
			case 'd':
				std::cout << "right" << std::endl;
				tello.SendCommand("rc 20 0 0 0");
				break;
			case 'c':
				std::cout << "right" << std::endl;
				tello.SendCommand("rc 0 0 0 20");
				break;
			case 'x':
				std::cout << "right" << std::endl;
				tello.SendCommand("rc 0 0 0 -20");
				break;
			case 'q':
				std::cout << "turn left" << std::endl;
				tello.SendCommand("ccw 30");
				break;
			case 'e':
				std::cout << "turn right" << std::endl;
				tello.SendCommand("cw 15");
				break;
			default:
				std::cout << "unknown command: " << inputChar << std::endl;
				break;
		}

	} while (inputChar != 'z');

    std::cout << "Closing .... \n";

	return 0;

}
