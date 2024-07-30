#ifndef BitcoinExchange_hpp
# define BitcoinExchange_hpp

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <algorithm>
#include <limits>
#include <cfloat>
#include <climits>

class BitcoinExchange
{
	private:
		std::map<int, float>	_prices;

		bool		isBadDateFormat(std::string date_word);
		bool		isBadFormat(std::string line);
		std::string	parseDate(std::string date_word);

	public:
		BitcoinExchange(std::string prices_filename, std::string rates_filename);
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange(void);

		void	parsePrices(std::string prices_filename);
		void	outputRatedPrices(std::string rates_filename);
		float	findClosestPrice(std::string date);

	class InvalidFileException : public std::exception
	{
		public:
			virtual const char* what() const throw()
			{
				return ("Error: could not open file");
			}
	};
};


#endif
