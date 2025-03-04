#include "PmergeMe.hpp"

PmergeMe::PmergeMe(std::string numbers)
{
	this->checkNumbers(numbers);
}

PmergeMe::PmergeMe(const PmergeMe& other)
{
	*this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other)
	{
		this->_vector = other._vector;
		this->_list = other._list;
	}
	return *this;
}

PmergeMe::~PmergeMe(void)
{
	return;
}

void	PmergeMe::checkNumbers(std::string numbers)
{
	for (int i = 0; numbers[i]; i++)
	{
		if (numbers[i] == ' ')
			continue;
		if (numbers[i] >= '0' && numbers[i] <= '9')
			continue;
		throw PmergeMe::InvalidArgumentException();
	}
}

void	PmergeMe::fillVector(std::string numbers)
{
	for (int i = 0; i < static_cast<int>(numbers.size()) ; i++)
	{
		if (numbers[i] == ' ')
			continue;
		else
		{
			long tmp = std::atol(numbers.c_str() + i);
			if (tmp < 0 || tmp > INT_MAX)
				throw PmergeMe::InvalidArgumentException();
			_vector.push_back(static_cast<int>(tmp));
			while (numbers[i] && numbers[i] != ' ')
				i++;
		}
	}
	if (_vector.empty() || _vector.size() < 2)
		throw PmergeMe::InvalidArgumentException();
}

void	PmergeMe::fillList(std::string numbers)
{
	for (int i = 0; i < static_cast<int>(numbers.size()) ; i++)
	{
		if (numbers[i] == ' ')
			continue;
		else
		{
			long tmp = std::atol(numbers.c_str() + i);
			if (tmp < 0 || tmp > INT_MAX)
				throw PmergeMe::InvalidArgumentException();
			_list.push_back(static_cast<int>(tmp));
			while (numbers[i] && numbers[i] != ' ')
				i++;
		}
	}
	if (_list.empty() || _list.size() < 2)
		throw PmergeMe::InvalidArgumentException();
}

std::vector<int>	mergeVector(std::vector<int>& b, std::vector<int>& a)
{
	std::vector<int> result;
	int i = 0;
	int j = 0;

	while (i < static_cast<int>(b.size()) && j < static_cast<int>(a.size()))
	{
		if (b[i] < a[j])
		{
			result.push_back(b[i]);
			i++;
		}
		else
		{
			result.push_back(a[j]);
			j++;
		}
	}
	while (i < static_cast<int>(b.size()))
	{
		result.push_back(b[i]);
		i++;
	}
	while (j < static_cast<int>(a.size()))
	{
		result.push_back(a[j]);
		j++;
	}
	return result;
}

std::vector<int>	MergeSortVector(std::vector<int> vector)
{
	if (vector.size() <= 1)
		return vector;

	std::vector<int> b;
	std::vector<int> a;
	int middle = vector.size() / 2;

	for (int i = 0; i < middle; i++)
		b.push_back(vector[i]);
	for (int i = middle; i < static_cast<int>(vector.size()); i++)
		a.push_back(vector[i]);

	b = MergeSortVector(b);
	a = MergeSortVector(a);

	return mergeVector(b, a);
}


std::list<int>	MergeInsertionSortList(std::list<int> list, size_t element_size)
{
	size_t number_of_pairs = list.size() / (element_size * 2);
	if (number_of_pairs == 0)
		return list;

	std::list<int>::iterator b = list.begin();
	std::advance(b, element_size - 1);
	std::list<int>::iterator a = b;
	std::advance(a, element_size);

	for (size_t pair_index = 0; pair_index < number_of_pairs; pair_index++) {
		
	}

}

void	PmergeMe::fillAndSort(std::string numbers)
{
	clock_t startVector = clock();
	this->fillVector(numbers);
	std::vector<int> sortedVector = MergeSortVector(_vector);
	clock_t endVector = clock();

	clock_t startList = clock();
	this->fillList(numbers);
	std::list<int> sortedList = MergeInsertionSortList(_list, 1);
	clock_t endList = clock();

	std::cout << "Before: ";
	int len = static_cast<int>(_vector.size());
	bool tooLong = false;
	if (len > 8)
	{
		tooLong = true;
		len = 7;
	}
	for (int i = 0; i < len; i++)
	{
		std::cout << _vector[i];
		if (i != len - 1)
			std::cout << " ";
	}
	if (tooLong)
		std::cout << " [...]";
	std::cout << std::endl;

	std::cout << "After:  ";
	for (int i = 0; i < len; i++)
	{
		std::cout << sortedVector[i];
		if (i != len - 1)
			std::cout << " ";
	}
	if (tooLong)
		std::cout << " [...]";
	std::cout << std::endl;

	double elapsed_time = static_cast<double>(endVector - startVector) / CLOCKS_PER_SEC * 1000.0;
	std::cout << "Time to process a range of " << _vector.size() << " elements with std::vector : " << elapsed_time << " ms" << std::endl;

	elapsed_time = static_cast<double>(endList - startList) / CLOCKS_PER_SEC * 1000.0;
	std::cout << "Time to process a range of " << _list.size() << " elements with std::list   : " << elapsed_time << " ms" << std::endl;
}