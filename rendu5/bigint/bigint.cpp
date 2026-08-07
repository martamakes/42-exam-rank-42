#include "bigint.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>

/* Constructors */

bigint::bigint( void ) : _digits("0") {
	// If empty start with 0
}

bigint::bigint(unsigned long n) {

	/* _digits = std::to_string(n); This is c++11 */
	std::ostringstream outstringstream; // integer to string
	outstringstream << n;
	_digits = outstringstream.str();
}

bigint::bigint(const bigint &other){
	this->_digits = other.getDigits();
}

bigint &bigint::operator=(const bigint &other) {
	if (this != &other) {
		this->_digits = other.getDigits();
	}
	return *this;
}

bigint::~bigint( void ) {

}

/* Getters */

const std::string &bigint::getDigits() const {
	return (_digits);
}


/* Arithmetic */

bigint bigint::operator+(const bigint &other) const
{
	std::string resString = "";
	int i = this->_digits.length() - 1;
	int j = other._digits.length() - 1;
	int carry = 0;

	while (i >= 0 || j >= 0 || carry > 0)
	{
		int d1 = (i >= 0) ? this->_digits[i] - '0' : 0;
		int d2 = (j >= 0) ? other._digits[j] - '0' : 0;
		int sum = d1 + d2 + carry;
		resString += (sum % 10) + '0';
		carry = sum / 10;

		i--;
		j--;
	}
	std::reverse(resString.begin(), resString.end());
	bigint result;
	result._digits = resString;
	return result;
}

bigint &bigint::operator+=(const bigint &other) {
	*this = *this + other;
	return *this;
}

bigint &bigint::operator++() {
	*this = *this + bigint(1);
	return *this;
}

bigint bigint::operator++(int) {
	bigint copy = *this;
	operator++();
	return copy;
}

/* 4. Comparison */

bool bigint::operator==(const bigint &other) const {
	return (this->getDigits() == other.getDigits());
}

bool bigint::operator!=(const bigint &other) const {
	return !(*this == other);
}

bool bigint::operator<=(const bigint &other) const {
	return operator<(other) || operator==(other);
}

bool bigint::operator>=(const bigint &other) const {
	return operator>(other) || operator==(other);
}

bool bigint::operator<(const bigint &other) const {

	if (this->_digits.length() != other._digits.length()) {
		return this->_digits.length() < other._digits.length(); // If length is diffrent the longer is the biger
	}
	else {
		return this->_digits < other._digits; // else we compaire 2 str and c++ will compaire char by char from Left and return boo expretion for the biger
	}
}

bool bigint::operator>(const bigint &other) const {
	return other < *this; // Resver the operator< function: if we want to knw if this is biger than other we jast have to prof that other is < this
}


/*  Shift */

bigint &bigint::operator<<=(const bigint &shift) // Shift Left (42 << 3 = 42000)
{
	// 1. If I am "0", shifting changes nothing. (0 * 10 = 0)
	if (this->_digits == "0")
		return *this;
	
	// 2. If shift amount is "0", nothing changes.
	if (shift._digits == "0")
		return *this;
	
	std::istringstream iss(shift.getDigits());
	unsigned long n;
	iss >> n;
	
	this->_digits.append(n,'0');

	return *this;
}

bigint &bigint::operator>>=(const bigint &shift) // Shift Right (1337 >> 2 = 13)
{
	std::istringstream iss(shift.getDigits());
	size_t shft;
	iss >> shft;
	
	if (shft >= _digits.length()) {
		_digits = "0";
		return *this;
	}
	int newlength = _digits.length() - shft;
	_digits.resize(newlength);
	return *this;
}

bigint bigint::operator<<(const bigint &shift) const {
	bigint res = *this;
    res <<= shift;
    return res;
}

bigint bigint::operator>>(const bigint &shift) const {
	bigint res = *this;
    res >>= shift;
    return res;
}


std::ostream &operator<<(std::ostream &os, const bigint &bg) {
	os << bg.getDigits();
	return os;
}


