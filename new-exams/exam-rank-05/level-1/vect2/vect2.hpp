#include <string>
#include <iostream>
#include <ostream>
#include <vector>

class vect2
{
	private:
		std::vector<int>	_container;

	public:
		vect2();//default constructor, add ": _container()"
		vect2(int v1, int v2);//initalise constructor
		vect2(const vect2& original);//copy constructor
		~vect2();//	Destructor

	int	getContainer_elements(int array) const;

	
	//handle operators for '+', '-' and '*';
	//	addition ASSIGNMENT operators
		// +
		vect2	operator+(int add) const;
		vect2	operator+(const vect2 addon) const;
		vect2&	operator++(void);// ++(pre)Increment
		vect2	operator++(int);// ++(post)Increment

	//	substraction ASSIGNMENT operators
		// -
		vect2	operator-(int add) const;
		vect2	operator-(const vect2 addon) const;
		vect2&	operator--(void);// --(pre)decrement operators
		vect2	operator--(int);// --(post)decrement operators

	//	multiplication ASSIGNMENT operators
		// *
		vect2	operator*(int multi) const;
		vect2	operator*(const vect2 &multion) const;

	//	equal(==) ASSIGNMENT operators
		vect2&	operator=(const vect2& other);//	Copy Assignment Operator
		bool	operator==(const vect2& other) const;// ==
		bool	operator!=(const vect2& other) const;// !=
		// +=
		vect2   operator+=(int add);
		vect2   operator+=(const vect2 &addon);
		// -=
		vect2   operator-=(int add);
		vect2   operator-=(const vect2 &addon);
		// *=
		vect2   operator*=(int multi);
		vect2   operator*=(const vect2 &multion);
};

vect2	operator*(int number, const vect2& vector);
std::ostream& operator<<(std::ostream& out, const vect2& v);
