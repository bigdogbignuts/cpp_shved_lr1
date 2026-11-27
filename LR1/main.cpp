#include <clocale>
#include <iostream>
#include <string>
#include <windows.h>

#include "GameLoop.h"

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	std::setlocale(LC_ALL, ".UTF8");

	std::string save_path = ".\\save.txt";

	auto game = GameLoop();
	game.save_file_path = save_path;
	game.mainLoop();

	return 0;
}
