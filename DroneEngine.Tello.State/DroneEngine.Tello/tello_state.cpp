#include "tello_state.h"

void Tello_State::ShowStatus(const std::string& state) {
	try
	{
		system("clear");

		int begin{ 0 };
		std::cout << "+-----------+-----------+" << std::endl;
		const int padding{ 10 };
		while (begin < state.size())
		{
			const auto split{ state.find(':', begin) };
			const auto name{ state.substr(begin, split - begin) };
			const auto end{ state.find(';', split) };
			const auto value{ state.substr(split + 1, end - split - 1) };

			begin = end + 1;

			std::cout << name << " : " << value << std::endl;

		}
		std::cout << "+-----------+-----------+" << std::endl;
	}
	catch (const std::exception& e){
		std::cout << e.what();
	}
}