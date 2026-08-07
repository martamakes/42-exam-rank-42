#ifndef BIG_INT
#define BIG_INT

#include <string>
#include <iostream>

/*! @brief Class representing an arbitrary precision unsigned integer. */
class bigint
{
	private:
		std::string _digits; /*!< String storage for the digits of the number. */

	public:
		/*! @brief Default constructor. Initializes to 0. */
		bigint( void );
		
		/*! @brief Constructor from unsigned long.
		    @param n The initial value. */
		bigint(unsigned long n);
		
		/*! @brief Copy constructor.
		    @param other The bigint to copy. */
		bigint(const bigint &other);
		
		/*! @brief Copy assignment operator.
		    @param other The bigint to assign from.
		    @return Reference to this bigint. */
		bigint &operator=(const bigint &other);
		
		/*! @brief Destructor. */
		~bigint( void );
		
		/*! @brief Addition operator.
		    @param other The bigint to add.
		    @return A new bigint result of addition. */
		bigint operator+(const bigint &other) const;
		
		/*! @brief Compound addition assignment operator.
		    @param other The bigint to add.
		    @return Reference to this bigint. */
		bigint &operator+=(const bigint &other);

		/*! @brief Prefix increment operator.
		    @return Reference to this incremented bigint. */
		bigint &operator++();
		
		/*! @brief Postfix increment operator.
		    @return A copy of the bigint before increment. */
		bigint operator++(int);

		/*! @brief Less-than comparison operator. */
		bool operator<(const bigint &other) const;
		
		/*! @brief Greater-than comparison operator. */
		bool operator>(const bigint &other) const;
		
		/*! @brief Equality comparison operator. */
		bool operator==(const bigint &other) const;
		
		/*! @brief Inequality comparison operator. */
		bool operator!=(const bigint &other) const;
		
		/*! @brief Less-than-or-equal comparison operator. */
		bool operator<=(const bigint &other) const;
		
		/*! @brief Greater-than-or-equal comparison operator. */
		bool operator>=(const bigint &other) const;

		/*! @brief Compound digit-shift left (multiplication by 10^n).
		    @param shift Number of digits to shift.
		    @return Reference to this bigint. */
		bigint &operator<<=(const bigint &shift);
		
		/*! @brief Compound digit-shift right (division by 10^n).
		    @param shift Number of digits to shift.
		    @return Reference to this bigint. */
		bigint &operator>>=(const bigint &shift);
		
		/*! @brief Digit-shift left operator. */
		bigint operator<<(const bigint &shift) const;
		
		/*! @brief Digit-shift right operator. */
		bigint operator>>(const bigint &shift) const;

		/*! @brief Getter for the digits string.
		    @return Reference to the string of digits. */
		const std::string& getDigits() const;	
};

/*! @brief Stream insertion operator for printing bigint. */
std::ostream &operator<<(std::ostream &os, const bigint &bg);

#endif
