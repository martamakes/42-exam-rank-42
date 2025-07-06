#include "bigint.hpp"

// clear && c++ -g main.cpp bigint.cpp bigint.hpp && valgrind --leak-check=full --show-leak-kinds=all ./a.out
int	main(void)
{
	{
		const bigint	a(42);
		bigint			b(21), c, d(1337), e(d);
		
		//All are in base10
		std::cout << a << std::endl;
		std::cout << b << std::endl;
		std::cout << c << std::endl;
		std::cout << d << std::endl;
		std::cout << e << std::endl;
		std::cout << "a + b = " << a + b << std::endl; // a + b = 63
		// std::cout << "\"(c += a)\"--> " << (c += a) << std::endl;
		// std::cout << "b = " << b << std::endl;
		// std::cout << "++b = " << b << std::endl;
		// std::cout << "b++ = " << b << std::endl;
		// std::cout << "b = " << b << std::endl;
		// std::cout << "\"((b << 10) + 42)\" = " << ((b << 10) + 42) << std::endl;
		// std::cout << "\"((d <<= 4) + 42)\" = " << ((b << 10) + 42) << std::endl;
		// std::cout << "\"((d >>= (const bigint)2)\" = " << (d >>= (const bigint)2) << std::endl;
		// std::cout << "a = " << a << std::endl;
		// std::cout << "d = " << d << std::endl;
		// std::cout << "(d < a) = " << (d < a) << std::endl;
		// std::cout << "(d > a) = " << (d > a) << std::endl;
		// std::cout << "(d == a) = " << (d == a) << std::endl;
		// std::cout << "(d != a) = " << (d != a) << std::endl;
		// std::cout << "(d == a) = " << (d == a) << std::endl;
		// std::cout << "(d <= a) = " << (d <= a) << std::endl;
		// std::cout << "(d >= a) = " << (d >= a) << std::endl;

		// bigint x(1234);
		// std::cout << std::endl;
		// std::cout << "\"(x << 2)\"--> " << (x << 2) << "\n";   // prints 123400
		// std::cout << "\"(x >> 2)\"--> " << (x >> 2) << "\n";   // prints   12}
	}
	return (0);
}