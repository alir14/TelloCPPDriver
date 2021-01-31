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
		case 'w':
			std::cout << "forward" << std::endl;
			tello.SendCommand("");
			break;
		case 's':
			std::cout << "backward" << std::endl;
			break;
		case 'a':
			std::cout << "left" << std::endl;
			break;
		case 'd':
			std::cout << "right" << std::endl;
			break;
		case 'q':
			std::cout << "turn left" << std::endl;
			break;
		case 'e':
			std::cout << "turn right" << std::endl;
			break;
		default:
			std::cout << "unknown command: " << inputChar << std::endl;
			break;
		}

	} while (inputChar != 'z');

    std::cout << "Closing .... \n";

	return 0;

}
