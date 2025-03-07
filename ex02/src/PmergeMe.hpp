#ifndef PMERGEME_hpp
# define PMERGEME_hpp

#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <list>
#include <climits>
#include <ctime>
#include <iterator>
#include <climits>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <set>


class PmergeMe
{
	private:
		std::vector<int>	_vector;
		std::list<int>		_list;

	public:
		PmergeMe(std::string numbers);
		PmergeMe(const PmergeMe& other);
		PmergeMe& operator=(const PmergeMe& other);
		~PmergeMe(void);

		void	checkNumbers(std::string numbers);
		void	checkDuplicates(std::string numbers);
		void	fillVector(std::string numbers);
		void	fillList(std::string numbers);
		void	fillAndSort(std::string numbers);

	class InvalidArgumentException : public std::exception
	{
		public:
			virtual const char* what() const throw()
			{
				return ("Error: invalid argument");
			}
	};

	class DuplicateElementException : public std::exception
	{
		public:
			virtual const char* what() const throw()
			{
				return ("Error: duplicate element found in input sequence");
			}
	};
};


#endif
