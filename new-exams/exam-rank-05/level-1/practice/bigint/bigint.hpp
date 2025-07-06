#include <string>
#include <iostream>
#include <ostream>
#include <sstream>// memorise this for exam**

class	bigint
{
	private:
		std::string	_string;;

	public:
		bigint();
		bigint(unsigned int n);
		bigint(const bigint& original);
		~bigint();

	//getValue
		std::string		getvalue() const;

	//operators
		bigint	operator+(const bigint& other) const;
};

std::ostream&	operator<<(std::ostream& out, const bigint& other);

