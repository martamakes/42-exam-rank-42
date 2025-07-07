#include "bigint.hpp"

// clear && c++ main.cpp bigint.cpp bigint.hpp && valgrind --leak-check=full --show-leak-kinds=all ./a.out
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
		std::cout << "\"(a + b)\"--> " << a + b << std::endl;
		std::cout << "\"(c += a)\"--> " << (c += a) << std::endl;
		std::cout << "b = " << b << std::endl;
		std::cout << "++b = " << b << std::endl;
		std::cout << "b++ = " << b << std::endl;
		std::cout << "b = " << b << std::endl;
		std::cout << "\"((b << 10) + 42)\" = " << ((b << 10) + 42) << std::endl;
		std::cout << "\"((d <<= 4) + 42)\" = " << ((b << 10) + 42) << std::endl;
		std::cout << "\"((d >>= (const bigint)2)\" = " << (d >>= (const bigint)2) << std::endl;
		std::cout << "a = " << a << std::endl;
		std::cout << "d = " << d << std::endl;
		std::cout << "(d < a) = " << (d < a) << std::endl;
		std::cout << "(d > a) = " << (d > a) << std::endl;
		std::cout << "(d == a) = " << (d == a) << std::endl;
		std::cout << "(d != a) = " << (d != a) << std::endl;
		std::cout << "(d == a) = " << (d == a) << std::endl;
		std::cout << "(d <= a) = " << (d <= a) << std::endl;
		std::cout << "(d >= a) = " << (d >= a) << std::endl;

		bigint x(1234);
		std::cout << std::endl;
		std::cout << "\"(x << 2)\"--> " << (x << 2) << "\n";   // prints 123400
		std::cout << "\"(x >> 2)\"--> " << (x >> 2) << "\n";   // prints   12}
	}

	// {
	// 	bigint a(40);				// construct from unsigned int
	// 	bigint b("00100");			// construct from string
	// 	bigint c = 0;				// perform addition (42 + 123)

	// 	std::cout << "a: " << a << std::endl;
	// 	std::cout << "b: " << b << std::endl;
	// 	std::cout << "c: " << c << std::endl;

	// 	// addtion
	// 	std::cout << std::endl;
	// 	bigint d("5");
	// 	std::cout << "a + 1 + b + 1: " << (a + 1 + b + 1) << std::endl;
	// 	std::cout << "c + 2: " << (c + 2) << std::endl;
	// 	std::cout << "d: " << d << std::endl;
	// 	bigint e("5");
	// 	std::cout << "d += e: " << (d += e) << std::endl;
	// 	std::cout << "d += 5: " << (d += 5) << std::endl;
	// 	bigint f("0");
	// 	std::cout << "  f: " << f << std::endl;// 0
	// 	std::cout << "f++: " << f++ << std::endl;// 0
	// 	std::cout << "  f: " << f << std::endl;// 1
	// 	std::cout << "++f: " << ++f << std::endl;// 2

	// 	// comparison
	// 	std::cout << std::endl;
	// 	bigint g("3");
	// 	std::cout << "is 'g' more than f?: " << (g > f) << std::endl;// return true
	// 	std::cout << "is 'g' more than e?: " << (g > e) << std::endl;// return false
	// 	std::cout << "is 'g' more than 2?: " << (g > 2) << std::endl;// return true
	// 	std::cout << "is 'g' more than 5?: " << (g > 5) << std::endl;// return false
	// 	std::cout << std::endl;
	// 	std::cout << "is 'g' less than f?: " << (g < f) << std::endl;// return true
	// 	std::cout << "is 'g' less than e?: " << (g < e) << std::endl;// return false
	// 	std::cout << "is 'g' less than 2?: " << (g < 2) << std::endl;// return true
	// 	std::cout << "is 'g' less than 5?: " << (g < 5) << std::endl;// return false

	// 	std::cout << std::endl;
	// 	bigint h("9"); //shift left "<<" is '*' by that value
	// 	std::cout << "(h << 1): " << (h << 1) << std::endl;  // 9 * 10^1 = 90
	// 	std::cout << "(h << 2): " << (h << 2) << std::endl;  // 9 * 10^2 = 900
	// 	std::cout << "(h << 5): " << (h << 5) << std::endl;  // 9 * 10^5 = 900000
	// 	bigint i("1920"); //shift right ">>" is '/' by that value
	// 	std::cout << "(i << 1): " << (i << 1) << std::endl;  // Should be 90
	// 	std::cout << "(i << 2): " << (i << 2) << std::endl;  // Should be 900
	// 	std::cout << "(i << 5): " << (i << 5) << std::endl;  // Should be 900000
	// 	std::cout << "------" << std::endl;
	// 	std::cout << "(i >> 1): " << (i >> 1) << std::endl;  // 9 / 10^1 = 0
	// 	std::cout << "(i >> 2): " << (i >> 2) << std::endl;  // 9 / 100  = 0
	// 	std::cout << "(i >> 5): " << (i >> 5) << std::endl;  // 9 / 100000 = 0

	// 	std::cout << std::endl;
	// 	bigint j("9");
	// 	bigint j2("9792");
	// 	std::cout << "\"((j << 10))\"--> " << (j << 10) << std::endl;
	// 	std::cout << "\"((j << 10) + 42)\"--> " << ((j << 10) + j2) << std::endl; // (9 * 10^10) + 42 = "90000000042"
	// 	std::cout << "\"((j << 10) + 42)\"--> " << ((j << 10) + 42 + 1) << std::endl; // (9 * 10^10) + 42 = "90000000042"

	// 	std::cout << std::endl;
	// 	bigint j3("942");
	// 	bigint j4("9792");
	// 	std::cout << "\"(j << 2)\"--> " << (j3 << 2) << std::endl;
	// 	std::cout << "\"(j << 2) + 42)\"--> " << ((j3 << 2) + j4) << std::endl;
	// 	// equals

	// 	return (0);
	// }
}