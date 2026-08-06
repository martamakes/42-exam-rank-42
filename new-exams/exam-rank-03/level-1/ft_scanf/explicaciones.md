- ft_vfscanf = función genérica que lee de cualquier
  FILE *
- ft_scanf = versión específica que siempre lee de
  stdin
- ft_fscanf = versión específica que lee del archivo
  que elijas

ft_vfscanf es la "función motor" que hace todo el trabajo real.


// ft_scanf - lee de stdin
  int ft_scanf(const char *format, ...)
    return ft_vfscanf(stdin, format, ap);  // Pasa stdin

// ft_fscanf - lee de un archivo específico  
  int ft_fscanf(FILE *archivo, const char *format, ...)
    return ft_vfscanf(archivo, format, ap);  // Pasa el archivo


  // ft_vfscanf - lee del FILE* que le den
  int ft_vfscanf(FILE *f, const char *format, va_list ap)
  {
      // Lee de 'f' (puede ser stdin, un archivo, etc.)
  }

  