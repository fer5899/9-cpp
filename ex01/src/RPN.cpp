#include "RPN.hpp"

RPN::RPN(std::string input)
{
	_input = input;
}

RPN::RPN(const RPN& other)
{
	*this = other;
}

RPN& RPN::operator=(const RPN& other)
{
	if (this != &other)
	{
		this->_input = other._input;
		this->_stack = other._stack;
	}
	return *this;
}

RPN::~RPN(void)
{
	return;
}

bool	RPN::isInputValid()
{
	for (int i = 0; _input[i]; i++)
	{
		if ((_input[i] == ' ' || _input[i] == '\t') 
			|| (_input[i] == '+' || _input[i] == '-' || _input[i] == '/' || _input[i] == '*') 
			|| (_input[i] >= '0' && _input[i] <= '9' && (!_input[i + 1] || (_input[i + 1] < '0' || _input[i + 1] > '9'))))
			continue;
		return false;
	}
	return true;
}

int	RPN::fillAndCalculate(void)
{
	for (int i = 0; _input[i]; i++)
	{
		if (_input[i] == ' ' || _input[i] == '\t')
			continue;
		if (_input[i] == '+' || _input[i] == '-' || _input[i] == '/' || _input[i] == '*')
		{
			if (this->_stack.size() < 2)
				throw RPN::InvalidArgumentException();
			float a = this->_stack.top();
			this->_stack.pop();
			float b = this->_stack.top();
			this->_stack.pop();
			if (_input[i] == '+')
				this->_stack.push(b + a);
			else if (_input[i] == '-')
				this->_stack.push(b - a);
			else if (_input[i] == '/')
			{
				if (a == 0)
					throw RPN::InvalidArgumentException();
				this->_stack.push(b / a);
			}
			else if (_input[i] == '*')
				this->_stack.push(b * a);
		}
		else
			this->_stack.push(_input[i] - '0');
	}
	if (this->_stack.size() != 1)
		throw RPN::InvalidArgumentException();
	return this->_stack.top();
}