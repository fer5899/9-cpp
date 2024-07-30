#include "RPN.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./RPN \"[calculations]\"" << std::endl;
		return 1;
	}
	try
	{
		RPN rpn(argv[1]);
		if (!rpn.isInputValid())
			throw RPN::InvalidArgumentException();
		std::cout << rpn.fillAndCalculate() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}