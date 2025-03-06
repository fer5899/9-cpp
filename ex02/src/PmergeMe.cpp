#include "PmergeMe.hpp"

static size_t jacobsthalNumber(size_t n) {
	return round((pow(2, n + 1) + pow(-1, n)) / 3);
}

static void nextIdxInJacobsthalSequence(size_t &current_idx, size_t &current_jacobsthal_idx) {
	current_idx--;
	if (current_idx <= jacobsthalNumber(current_jacobsthal_idx - 1))
	{
		current_jacobsthal_idx++;
		current_idx = jacobsthalNumber(current_jacobsthal_idx);
	}
}

static size_t findBoundingJacobsthalIndex(size_t number_of_pairs)
{
	size_t idx = 1;
	while (jacobsthalNumber(idx) <= number_of_pairs)
		idx++;
	return idx;
}

static void checkDuplicates(std::string numbers) {
	std::set<int> set;
	for (size_t i = 0; i < numbers.size(); i++)
	{
		if (numbers[i] == ' ')
			continue;
		else
		{
			long tmp = std::atol(numbers.c_str() + i);
			if (tmp < 0 || tmp > INT_MAX)
				throw PmergeMe::InvalidArgumentException();
			if (!set.insert(static_cast<int>(tmp)).second)
				throw PmergeMe::DuplicateElementException();
			while (numbers[i] && numbers[i] != ' ')
				i++;
		}
	}
	if (set.empty() || set.size() < 2)
		throw PmergeMe::InvalidArgumentException();
}

PmergeMe::PmergeMe(std::string numbers)
{
	this->checkNumbers(numbers);
}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	*this = other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
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

void PmergeMe::checkNumbers(std::string numbers)
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

void printList(std::list<int> list) {
	int len = static_cast<int>(list.size());
	// bool tooLong = false;
	// if (len > 8)
	// {
	// 	tooLong = true;
	// 	len = 7;
	// }
	std::list<int>::iterator it = list.begin();
	for (int i = 0; i < len; i++, ++it)
	{
		std::cout << *it;
		if (i != len - 1)
			std::cout << " ";
	}
	// if (tooLong)
	// 	std::cout << " [...]";
	std::cout << std::endl;
	std::cout << std::endl;

}

void PmergeMe::fillVector(std::string numbers)
{
	for (int i = 0; i < static_cast<int>(numbers.size()); i++)
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

void PmergeMe::fillList(std::string numbers)
{
	for (int i = 0; i < static_cast<int>(numbers.size()); i++)
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


static void advancePair_Vector(std::vector<int>::iterator &a, std::vector<int>::iterator &b, size_t element_size, size_t &pair_index) {
	std::advance(a, element_size * 2);
	std::advance(b, element_size * 2);
	pair_index++;
}

static void swapUnsortedPairs_Vector(std::vector<int> &vector, size_t element_size)
{
	typedef typename std::vector<int>::iterator Iterator;
	size_t number_of_pairs = vector.size() / (element_size * 2);

	Iterator b = vector.begin();
	std::advance(b, element_size - 1);
	Iterator a = b;
	std::advance(a, element_size);

	for (size_t pair_index = 0; pair_index < number_of_pairs; advancePair_Vector(a, b, element_size, pair_index))
	{
		if (*b < *a)
			continue;
		Iterator swap_b = b;
		Iterator swap_a = a;
		for (size_t swapped_elements = 0; swapped_elements < element_size; swapped_elements++)
		{
			std::swap(*swap_b, *swap_a);
			swap_b--;
			swap_a--;
		}
	}
}

static std::vector<int>::iterator getElementIterator_Vector(std::vector<int>::iterator first_element, size_t element_idx, size_t element_size)
{
	std::advance(first_element, element_size * element_idx);
	return first_element;
}

struct CompareIterators_Vector {
	bool operator()(const std::vector<int>::iterator &left, const std::vector<int>::iterator &right) const
	{
		return *left < *right;
	}
};

static void recursiveMergeInsertionSort_Vector(std::vector<int> &vector, size_t element_size)
{
	typedef typename std::vector<int>::iterator Iterator;
	size_t number_of_pairs = vector.size() / (element_size * 2);

	swapUnsortedPairs_Vector(vector, element_size);

	if (number_of_pairs <= 1)
		return;

	recursiveMergeInsertionSort_Vector(vector, element_size * 2);
	
	size_t number_of_elements = vector.size() / element_size;

	std::vector<Iterator> main;
	main.reserve(number_of_elements);
	std::vector<Iterator> a_elements;
	a_elements.reserve(number_of_elements / 2);
	std::vector<Iterator> b_elements;
	b_elements.reserve(number_of_elements / 2 + 1);
	
	
	Iterator first_element = vector.begin();
	std::advance(first_element, element_size - 1);
	
	// Initialize main with A1, B1
	main.push_back(getElementIterator_Vector(first_element, 0, element_size));
	main.push_back(getElementIterator_Vector(first_element, 1, element_size));
	
	// Initialize a_elements with all As, and initialize b_elements with the all Bs
	for (size_t element_idx = 0; element_idx < number_of_elements; element_idx++)
	{
		if (element_idx % 2 == 0)
			b_elements.push_back(getElementIterator_Vector(first_element, element_idx, element_size));
		else
		{
			a_elements.push_back(getElementIterator_Vector(first_element, element_idx, element_size));
			if (element_idx != 1)
				main.push_back(getElementIterator_Vector(first_element, element_idx, element_size));
		}
	}
			
	CompareIterators_Vector comp;

	// Perform binary insertion in the order dictated by Jacobsthal numbers if it can be done
	size_t bounding_jacobsthal_idx = findBoundingJacobsthalIndex(number_of_pairs);
	size_t current_jacobsthal_idx = 1;
	size_t inserted_elements = 1; // B1 counts as inserted
	// idx starts from 1 to work with jacobsthals, but the indexes to work with the lists start from 0. An offset of -1 is needed.
	for (size_t idx = 1; current_jacobsthal_idx < bounding_jacobsthal_idx; nextIdxInJacobsthalSequence(idx, current_jacobsthal_idx)) {
		if (idx == 1)
			continue;
		// The current B element will always be smaller than its A element counterpart, therefore
		// the bound is all the Bs inserted up to now plus all the As up to the index.
		size_t boundElementIdx = (idx - 1) + inserted_elements; // -1 for the offset
		std::vector<Iterator>::iterator bound = main.begin();
		std::advance(bound, boundElementIdx);
		std::vector<Iterator>::iterator to_insert = b_elements.begin();
		std::advance(to_insert, idx - 1);

		
		std::vector<Iterator>::iterator insert_before = std::upper_bound(main.begin(), bound, *to_insert, comp);
		main.insert(insert_before, *to_insert);
		inserted_elements++;
	}

	// Perform binary insertion in sequential order
	while (inserted_elements < b_elements.size()) {
		// The current B element will always be smaller than its A element counterpart, therefore
		// the bound is all the Bs inserted up to now plus all the As up to the index.
		size_t boundElementIdx = inserted_elements * 2;
		std::vector<Iterator>::iterator bound = main.begin();
		std::advance(bound, boundElementIdx);
		std::vector<Iterator>::iterator to_insert = b_elements.begin();
		std::advance(to_insert, inserted_elements);

		std::vector<Iterator>::iterator insert_before = std::upper_bound(main.begin(), bound, *to_insert, comp);
		main.insert(insert_before, *to_insert);
		inserted_elements++;
	}
	

	// Copy all the elements to a copy vector
	std::vector<int> copy_vector;
	for (std::vector<Iterator>::iterator element = main.begin(); element != main.end(); element++)
	{
		for (size_t i = 0; i < element_size; i++)
		{
			Iterator element_start = *element;
			std::advance(element_start, -element_size + i + 1);
			copy_vector.insert(copy_vector.end(), *element_start);
		}
	}
	
	// Replace values in the original list. The leftover numbers remain unchanged.
	Iterator vector_it = vector.begin();
	Iterator copy_it = copy_vector.begin();
	while (copy_it != copy_vector.end())
	{
		*vector_it = *copy_it;
		vector_it++;
		copy_it++;
	}

	return;
}



static void advancePair_List(std::list<int>::iterator &a, std::list<int>::iterator &b, size_t element_size, size_t &pair_index) {
	std::advance(a, element_size * 2);
	std::advance(b, element_size * 2);
	pair_index++;
}

static void swapUnsortedPairs_List(std::list<int> &list, size_t element_size)
{
	typedef typename std::list<int>::iterator Iterator;
	size_t number_of_pairs = list.size() / (element_size * 2);

	Iterator b = list.begin();
	std::advance(b, element_size - 1);
	Iterator a = b;
	std::advance(a, element_size);

	for (size_t pair_index = 0; pair_index < number_of_pairs; advancePair_List(a, b, element_size, pair_index))
	{
		if (*b < *a)
			continue;
		Iterator swap_b = b;
		Iterator swap_a = a;
		for (size_t swapped_elements = 0; swapped_elements < element_size; swapped_elements++)
		{
			std::swap(*swap_b, *swap_a);
			swap_b--;
			swap_a--;
		}
	}
}

static std::list<int>::iterator getElementIterator_List(std::list<int>::iterator first_element, size_t element_idx, size_t element_size)
{
	std::advance(first_element, element_size * element_idx);
	return first_element;
}

struct CompareIterators_List {
	bool operator()(const std::list<int>::iterator &left, const std::list<int>::iterator &right) const
	{
		return *left < *right;
	}
};

static void recursiveMergeInsertionSort_List(std::list<int> &list, size_t element_size)
{
	typedef typename std::list<int>::iterator Iterator;
	size_t number_of_pairs = list.size() / (element_size * 2);

	swapUnsortedPairs_List(list, element_size);

	if (number_of_pairs <= 1)
		return;

	recursiveMergeInsertionSort_List(list, element_size * 2);
	
	size_t number_of_elements = list.size() / element_size;

	std::list<Iterator> main;
	std::list<Iterator> a_elements;
	std::list<Iterator> b_elements;
	
	
	Iterator first_element = list.begin();
	std::advance(first_element, element_size - 1);
	
	// Initialize main with A1, B1
	main.push_back(getElementIterator_List(first_element, 0, element_size));
	main.push_back(getElementIterator_List(first_element, 1, element_size));
	
	// Initialize a_elements with all As, and initialize b_elements with the all Bs
	for (size_t element_idx = 0; element_idx < number_of_elements; element_idx++)
	{
		if (element_idx % 2 == 0)
			b_elements.push_back(getElementIterator_List(first_element, element_idx, element_size));
		else
		{
			a_elements.push_back(getElementIterator_List(first_element, element_idx, element_size));
			if (element_idx != 1)
				main.push_back(getElementIterator_List(first_element, element_idx, element_size));
		}
	}
			
	CompareIterators_List comp;

	// Perform binary insertion in the order dictated by Jacobsthal numbers if it can be done
	size_t bounding_jacobsthal_idx = findBoundingJacobsthalIndex(number_of_pairs);
	size_t current_jacobsthal_idx = 1;
	size_t inserted_elements = 1; // B1 counts as inserted
	// idx starts from 1 to work with jacobsthals, but the indexes to work with the lists start from 0. An offset of -1 is needed.
	for (size_t idx = 1; current_jacobsthal_idx < bounding_jacobsthal_idx; nextIdxInJacobsthalSequence(idx, current_jacobsthal_idx)) {
		if (idx == 1)
			continue;
		// The current B element will always be smaller than its A element counterpart, therefore
		// the bound is all the Bs inserted up to now plus all the As up to the index.
		size_t boundElementIdx = (idx - 1) + inserted_elements; // -1 for the offset
		std::list<Iterator>::iterator bound = main.begin();
		std::advance(bound, boundElementIdx);
		std::list<Iterator>::iterator to_insert = b_elements.begin();
		std::advance(to_insert, idx - 1);

		
		std::list<Iterator>::iterator insert_before = std::upper_bound(main.begin(), bound, *to_insert, comp);
		main.insert(insert_before, *to_insert);
		inserted_elements++;
	}

	// Perform binary insertion in sequential order
	while (inserted_elements < b_elements.size()) {
		// The current B element will always be smaller than its A element counterpart, therefore
		// the bound is all the Bs inserted up to now plus all the As up to the index.
		size_t boundElementIdx = inserted_elements * 2;
		std::list<Iterator>::iterator bound = main.begin();
		std::advance(bound, boundElementIdx);
		std::list<Iterator>::iterator to_insert = b_elements.begin();
		std::advance(to_insert, inserted_elements);

		std::list<Iterator>::iterator insert_before = std::upper_bound(main.begin(), bound, *to_insert, comp);
		main.insert(insert_before, *to_insert);
		inserted_elements++;
	}
	

	// Copy all the elements to a copy list
	std::list<int> copy_list;
	for (std::list<Iterator>::iterator element = main.begin(); element != main.end(); element++)
	{
		for (size_t i = 0; i < element_size; i++)
		{
			Iterator element_start = *element;
			std::advance(element_start, -element_size + i + 1);
			copy_list.insert(copy_list.end(), *element_start);
		}
	}
	
	// Replace values in the original list. The leftover numbers remain unchanged.
	Iterator list_it = list.begin();
	Iterator copy_it = copy_list.begin();
	while (copy_it != copy_list.end())
	{
		*list_it = *copy_it;
		list_it++;
		copy_it++;
	}

	return;
}

void PmergeMe::fillAndSort(std::string numbers)
{
	checkDuplicates(numbers);

	clock_t startVector = clock();
	this->fillVector(numbers);
	std::vector<int> sortedVector = this->_vector;
	recursiveMergeInsertionSort_Vector(sortedVector, 1);
	clock_t endVector = clock();

	clock_t startList = clock();
	this->fillList(numbers);
	std::list<int> sortedList = this->_list;
	recursiveMergeInsertionSort_List(sortedList, 1);
	clock_t endList = clock();

	std::cout << "Before: ";
	size_t len = _vector.size();
	bool tooLong = false;
	if (len > 8)
	{
		tooLong = true;
		len = 7;
	}
	for (size_t i = 0; i < len; i++)
	{
		std::cout << _vector[i];
		if (i != len - 1)
			std::cout << " ";
	}
	if (tooLong)
		std::cout << " [...]";
	std::cout << std::endl;

	std::cout << "After:  ";
	for (size_t i = 0; i < len; i++)
	{
		std::cout << sortedVector[i];
		if (i != len - 1)
			std::cout << " ";
	}
	if (tooLong)
		std::cout << " [...]";
	std::cout << std::endl;

	double elapsed_time = static_cast<double>(endVector - startVector) / CLOCKS_PER_SEC * 1000.0;
	std::cout << "Time to process a range of " << _vector.size() << " elements with std::vector : " << std::fixed << std::setprecision(6) << elapsed_time << " ms" << std::endl;

	elapsed_time = static_cast<double>(endList - startList) / CLOCKS_PER_SEC * 1000.0;
	std::cout << "Time to process a range of " << _list.size() << " elements with std::list   : " << std::fixed << std::setprecision(6) << elapsed_time << " ms" << std::endl;
}