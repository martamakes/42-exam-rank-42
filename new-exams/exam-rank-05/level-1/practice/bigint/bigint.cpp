#include "bigint.hpp"

bigint::bigint()
{
	this->_string = "0";
}

bigint::bigint(unsigned int n)
{
	std::ostringstream oss;
	oss << n;

	this->_string = oss.str();
}

bigint::bigint(const bigint& original)
{
	*this = original;
}

bigint::~bigint() {}


//getter
std::string		bigint::getvalue() const
{
	return (this->_string);
}


//operator time
bigint	bigint::operator+(const bigint& other) const
{
	bigint	copy(*this);
	std::string	ss1 = this->_string;
	std::string	ss2 = other._string;

	//check which is longer
	if (ss2.size() > ss1.size())
		std::swap(ss1, ss2);

	std::string	result = "";
	int	i = ss1.length() - 1;// to prevent hitting the .end() part
	int	j = ss2.length() - 1;
	int	carry = 0;

	while (i >= 0 || j >= 0 || carry)
	{
		int	digit1 = i >= 0 ? ss1[i] - '0' : 0;
		int	digit2 = j >= 0 ? ss2[i] - '0' : 0;
		int	sum = digit1 + digit2 + carry;

		i--;
		j--;
		result.insert(result.begin(), (sum % 10) + '0');
		carry = (sum / 10);
	}
	copy._string = result;
	return(copy);
}

//outside class
std::ostream&	operator<<(std::ostream& out, const bigint& other)
{
	out << other.getvalue();
	return (out);
}