#include <fmt/core.h>

#include "runner.h"

#include <Windows.h>

int main(int argc, char* argv[])
{
	//Temporary
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	//
	try
	{
		runner::parse_arguments(argc, argv);
		runner::start();
	}
	catch (const std::exception& ex)
	{
		fmt::print(ex.what());
	}
	//Temporary
	system("pause");
}
