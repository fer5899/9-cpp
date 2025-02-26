#include"PmergeMe.hpp"

int main(int argc, char **argv)
{
	std::string numbers;
	if (argc < 2)
	{
		std::cerr << "Usage: ./PmergeMe [numbers]" << std::endl;
		return 1;
	}
	else if (argc > 2)
	{
		numbers = argv[1];
		for (int i = 2; i < argc; i++)
		{
			numbers += " ";
			numbers += argv[i];
		}
	}
	else
		numbers = argv[1];
	try
	{
		PmergeMe PmergeMe(numbers);
		PmergeMe.fillAndSort(numbers);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}