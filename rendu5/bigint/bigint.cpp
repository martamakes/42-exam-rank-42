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

/*! PSEUDOCODE — operator+ (add two arbitrary-precision numbers):
 *  1. walk BOTH digit strings from the END (rightmost = least
 *     significant digit) with two independent indices i, j
 *  2. loop while i >= 0 OR j >= 0 OR carry > 0 (carry can create ONE
 *     extra digit past both operands, e.g. 9 + 9 -> "18")
 *  3. d1 = digit at i, or 0 if i ran out; d2 = digit at j, or 0
 *  4. sum = d1 + d2 + carry; append (sum % 10) as a char; carry = sum/10
 *  5. decrement i and j every loop, regardless of which one ran out
 *  6. the result string was built BACKWARDS -> reverse it before
 *     returning
 */
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

/*! PSEUDOCODE — prefix ++x vs postfix x++:
 *  1. prefix ++x: add 1 in place (reuse operator+), return *this by
 *     reference (caller sees the NEW value)
 *  2. postfix x++ (dummy `int` param only to distinguish the overload):
 *       - copy = *this (snapshot the OLD state)
 *       - call operator++() to actually increment
 *       - return `copy` by VALUE (the old state can't be returned by
 *         reference — it stops existing once *this changes)
 */
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

/*! PSEUDOCODE — operator< (comparison, the only one written by hand):
 *  1. an unsigned number with MORE digits is always bigger — compare
 *     LENGTHS first (never compare the strings directly when lengths
 *     differ: "9" vs "10" would compare wrong char-by-char)
 *  2. only if lengths are EQUAL, compare the digit strings lexically —
 *     equal-length numeric strings sort the same as their numeric value
 *  3. derive >, <=, >=, ==, != from this ONE function (avoid
 *     duplicating the comparison logic 6 times)
 */
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

/*! PSEUDOCODE — operator<<= (digit-shift LEFT = multiply by 10^shift):
 *  1. shift amount is itself a bigint, not a plain int — convert it to
 *     an unsigned long first (istringstream >> n)
 *  2. if *this is "0", or shift is "0": nothing to do, return early
 *     (0 shifted stays 0; shifting by 0 changes nothing)
 *  3. otherwise: append `n` '0' characters to the digit string — that's
 *     literally what multiplying by 10^n does to the decimal digits
 */
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

/*! PSEUDOCODE — operator>>= (digit-shift RIGHT = integer-divide by 10^shift):
 *  1. convert shift (bigint) to an unsigned long / size_t
 *  2. if shift >= number of digits: the WHOLE number shifts away ->
 *     result is "0" (never an empty string!)
 *  3. otherwise: resize() the digit string to (length - shift), which
 *     drops the LAST `shift` characters (the least significant digits)
 */
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


