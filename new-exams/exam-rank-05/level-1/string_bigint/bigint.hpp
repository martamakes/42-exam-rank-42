#include <string>
#include <iostream>
#include <ostream>
#include <sstream>  // Required for stringstream

class	bigint
{
	private:
		std::string _string;

	public:
		// Constructors
		bigint();                    // defaults to 0
		bigint(unsigned int n);     // constructs from int
		bigint(const std::string& str_argument); // constructs from string
		bigint(const bigint& original);   // copy constructor
		~bigint();

		//getter function
		std::string	getArbiter_value() const;
		//addtion
			// +
			bigint	operator+(const bigint& other) const;
			bigint	operator+(unsigned int value) const;
			// +=
			bigint	operator+=(const bigint& other);
			bigint	operator+=(unsigned int value);
			// ++
			bigint	operator++(int);
			bigint	operator++(void);
		//comparison
			// >
			bool	operator>(const bigint& other);
			bool	operator>(unsigned int value);
			// <
			bool	operator<(const bigint& other);
			bool	operator<(unsigned int value);
			// << -digitshift-
			bigint	operator<<(unsigned int shift) const;
			// >> -digitshift-
			bigint	operator>>(unsigned int shift) const;
			// <<= -digitshift-
			bigint operator<<=(unsigned int shift);
			// >>= -digitshift-
			bigint operator>>=(unsigned int shift);
			// <=
			bool	operator<=(const bigint& other);
			bool	operator<=(unsigned int shift);
			// >=
			bool	operator>=(const bigint& other);
			bool	operator>=(unsigned int shift);

			// operator=()// Copy Assignment Operator
			bigint&	operator=(const bigint& other);
			// ==
			bool	operator==(const bigint& other);
			bool	operator==(unsigned int shift);
			// !=
			bool	operator!=(const bigint& other);
			bool	operator!=(unsigned int shift);
};

std::ostream& operator<<(std::ostream& out, const bigint& value);
