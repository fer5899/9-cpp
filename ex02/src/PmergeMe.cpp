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

std::vector<int>	mergeVector(std::vector<int>& left, std::vector<int>& right)
{
	std::vector<int> result;
	int i = 0;
	int j = 0;

	while (i < static_cast<int>(left.size()) && j < static_cast<int>(right.size()))
	{
		if (left[i] < right[j])
		{
			result.push_back(left[i]);
			i++;
		}
		else
		{
			result.push_back(right[j]);
			j++;
		}
	}
	while (i < static_cast<int>(left.size()))
	{
		result.push_back(left[i]);
		i++;
	}
	while (j < static_cast<int>(right.size()))
	{
		result.push_back(right[j]);
		j++;
	}
	return result;
}

std::vector<int>	MergeSortVector(std::vector<int> vector)
{
	if (vector.size() <= 1)
		return vector;

	std::vector<int> left;
	std::vector<int> right;
	int middle = vector.size() / 2;

	for (int i = 0; i < middle; i++)
		left.push_back(vector[i]);
	for (int i = middle; i < static_cast<int>(vector.size()); i++)
		right.push_back(vector[i]);

	left = MergeSortVector(left);
	right = MergeSortVector(right);

	return mergeVector(left, right);
}

std::list<int>	mergeList(std::list<int>& left, std::list<int>& right)
{
	std::list<int> result;
	std::list<int>::iterator itLeft = left.begin();
	std::list<int>::iterator itRight = right.begin();

	while (itLeft != left.end() && itRight != right.end())
	{
		if (*itLeft < *itRight)
		{
			result.push_back(*itLeft);
			itLeft++;
		}
		else
		{
			result.push_back(*itRight);
			itRight++;
		}
	}
	while (itLeft != left.end())
	{
		result.push_back(*itLeft);
		itLeft++;
	}
	while (itRight != right.end())
	{
		result.push_back(*itRight);
		itRight++;
	}
	return result;
}

std::list<int>	MergeSortList(std::list<int> list)
{
	if (list.size() <= 1)
		return list;

	std::list<int> left;
	std::list<int> right;
	int middle = list.size() / 2;

	for (int i = 0; i < middle; i++)
	{
		left.push_back(list.front());
		list.pop_front();
	}
	while (!list.empty())
	{
		right.push_back(list.front());
		list.pop_front();
	}

	left = MergeSortList(left);
	right = MergeSortList(right);

	return mergeList(left, right);
}

void	PmergeMe::fillAndSort(std::string numbers)
{
	clock_t startVector = clock();
	this->fillVector(numbers);
	std::vector<int> sortedVector = MergeSortVector(_vector);
	clock_t endVector = clock();

	clock_t startList = clock();
	this->fillList(numbers);
	std::list<int> sortedList = MergeSortList(_list);
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