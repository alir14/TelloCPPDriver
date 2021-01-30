// DroneEngine.Tello.Command.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <conio.h> //this header is needed to use _getch()

int main()
{
	int c;
	puts("enter value , z will exit.");

	//char input;
	do
	{
		//c = getchar();
		c = _getch();
		putchar(c);

		//std::cin >> input;

		switch (c)
		{
		case 'w':
			std::cout << "forward" << std::endl;
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
			std::cout << "unknown command: " << c << std::endl;
			break;
		}

	} while (c != 'z');

    std::cout << "Closing .... \n";

	return 0;

}
