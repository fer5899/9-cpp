#include"BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(std::string prices_filename, std::string rates_filename)
{
	try
	{
		std::ifstream prices_file(prices_filename.c_str());
		std::ifstream rates_file(rates_filename.c_str());
		if (prices_file == NULL || rates_file == NULL)
			throw BitcoinExchange::InvalidFileException();
		this->parsePrices(prices_filename);
		this->outputRatedPrices(rates_filename);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
{
	*this = other;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
	{
		_prices = other._prices;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange(void)
{
}

void	BitcoinExchange::parsePrices(std::string prices_filename)
{
	std::ifstream prices_file(prices_filename.c_str());
	std::string	line;
	std::string date_word;
	std::string price_word;

	while (std::getline(prices_file, line))
	{
		if (line == "date | value")
			continue;

		std::stringstream line_stream(line);
		
		std::getline(line_stream, date_word, ',');
		std::getline(line_stream, price_word, ',');

		_prices[std::atof(parseDate(date_word).c_str())] = std::atof(price_word.c_str());
	}
}

std::string	BitcoinExchange::parseDate(std::string date_word)
{
	std::string year_word;
	std::string month_word;
	std::string day_word;
	std::stringstream date_stream(date_word);
	std::getline(date_stream, year_word, '-');
	std::getline(date_stream, month_word, '-');
	std::getline(date_stream, day_word, '-');
	return year_word + month_word + day_word;
}

void	BitcoinExchange::outputRatedPrices(std::string rates_filename)
{
	std::ifstream rates_file(rates_filename.c_str());
	std::string line;
	std::string date_word;
	std::string rate_word;

	while (std::getline(rates_file, line))
	{
		if (line == "date | value")
		{
			std::cout << "date | value | result" << std::endl;
			continue;
		}
		if (isBadFormat(line))
			continue;

		std::stringstream line_stream(line);

		std::getline(line_stream, date_word, '|');
		std::getline(line_stream, rate_word, '|');
		
		float result = std::atof(rate_word.c_str()) * this->findClosestPrice(parseDate(date_word));
		std::cout << date_word << "=>" << rate_word << " = " << result << std::endl;
	}
}

bool	BitcoinExchange::isBadFormat(std::string line)
{
	std::string date_word;
	std::string rate_word;
	std::stringstream line_stream(line);
	std::getline(line_stream, date_word, '|');
	std::getline(line_stream, rate_word, '|');

	if (date_word.empty() || rate_word.empty())
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return true;
	}
	if (std::atof(rate_word.c_str()) > 1000)
	{
		std::cout << "Error: too large a number." << std::endl;
		return true;
	}
	if (std::atof(rate_word.c_str()) < 0)
	{
		std::cout << "Error: not a positive number." << std::endl;
		return true;
	}
	if (isBadDateFormat(date_word))
	{
		std::cout << "Error: bad date format." << std::endl;
		return true;
	}
	return false;
}

bool	BitcoinExchange::isBadDateFormat(std::string date_word)
{
	std::string year_word;
	std::string month_word;
	std::string day_word;
	std::stringstream date_stream(date_word);
	std::getline(date_stream, year_word, '-');
	std::getline(date_stream, month_word, '-');
	std::getline(date_stream, day_word, '-');

	if (std::atof(year_word.c_str()) <= 0 || std::atof(month_word.c_str()) <= 0 || std::atof(day_word.c_str()) <= 0
			|| std::atof(year_word.c_str()) > 2050 || std::atof(month_word.c_str()) > 12 || std::atof(day_word.c_str()) > 31 
			|| (std::atof(month_word.c_str()) == 2 && std::atof(day_word.c_str()) > 29) 
			|| (std::atof(month_word.c_str()) == 4 && std::atof(day_word.c_str()) > 30) 
			|| (std::atof(month_word.c_str()) == 6 && std::atof(day_word.c_str()) > 30) 
			|| (std::atof(month_word.c_str()) == 9 && std::atof(day_word.c_str()) > 30) 
			|| (std::atof(month_word.c_str()) == 11 && std::atof(day_word.c_str()) > 30))
	{
		return true;
	}
	return false;
}

float		BitcoinExchange::findClosestPrice(std::string date)
{
	int dateInt = std::atof(date.c_str());
	std::map<int, float>::iterator it = _prices.begin();
	while (it != _prices.end())
	{
		if (it->first == dateInt)
			return it->second;
		else if (it->first > dateInt)
		{
			it--;
			return it->second;
		}
		it++;
	}
	return 0;
}

