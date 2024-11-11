// // Función para calcular el Máximo Común Divisor (GCD)
// unsigned int gcd(unsigned int a, unsigned int b)
// {
//     // Caso base: si b es 0, el GCD es a
//     if (b == 0)
//         return a;
    
//     // Llamada recursiva utilizando el algoritmo de Euclides
//     // El GCD de a y b es el mismo que el GCD de b y el residuo de a/b
//     return gcd(b, a % b);
// }

// // Función para calcular el Mínimo Común Múltiplo (LCM)
// unsigned int lcm(unsigned int a, unsigned int b)
// {
//     // Caso especial: si alguno de los números es 0, el LCM es 0
//     if (a == 0 || b == 0)
//         return 0;
    
//     // Calculamos el Máximo Común Divisor (GCD) de a y b
//     unsigned int hcf = gcd(a, b);

//     // Calculamos el producto de a y b
//     // No es necesario calcular el valor absoluto ya que estamos usando unsigned int
//     unsigned int abs_product = a * b;

//     // Aplicamos la fórmula: LCM(a, b) = |a * b| / GCD(a, b)
//     return abs_product / hcf;
// }

unsigned int lcm(unsigned int a, unsigned int b)
{
	unsigned int n;	

	if (a == 0 || b == 0)
		return (0);
	if (a > b)
		n = a;
	else
		n = b;
	while (1)
	{
		if (n % a == 0 && n % b == 0)
			return (n);
		++n;
	}
}