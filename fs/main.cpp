#include <iostream>

#include "syscall.h"

int main()
{

	if (Mount("fs.img") < 0) {
		std::cout << "文件系统格式不对\n是否要格式化\n";

		std::string input;
		std::cin >> input;
		if (input == "y") {
			Format();
		}
		else {
			exit(0);
		}
	}

	ProcessCommand();

	return 0;
}
