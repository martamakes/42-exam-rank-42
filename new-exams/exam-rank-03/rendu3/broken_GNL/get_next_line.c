#include "broken_gnl.h"

char	*ft_strchr(char *s, int c)
{
	int	i = 0;
	// while(s[i] != c) Bucle infinito
	while (s[i] != c && s[i] != '\0')
		i++;
	if (s[i] == c)
		return (s + i);
	else
		return (NULL);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	//while(--n > 0) -> MAL
		//((char *)dest)[n - 1] = ((char *)src)[n - 1]; -> MAL

	size_t i = 0;                  
	while (i < n)
	{
		((char *)dest)[i] = ((char *)src)[i];
		i++;
	}
	return (dest);
}
//Explicación
// n        1   2   3   4
// s        h   o   l   a
// s[n]    [0] [1] [2] [3]   

size_t	ft_strlen(char *s)
{
	size_t	ret = 0;
	while (*s)
	{
		s++;
		ret++;
	}
	return (ret);
}

int	str_append_mem(char **s1, char *s2, size_t size2)
{
	// size_t size1 = ft_strlen(*s1); --> si s1 es NULL -> CRASH
	size_t size1 = (*s1) ? ft_strlen(*s1) : 0;  // Si *s1 es NULL, size1 = 0
	char	*tmp = malloc(size2 + size1 + 1);
	if (!tmp)
		return (0);
    if (*s1)  // Solo copia *s1 si no es NULL
	    ft_memcpy(tmp, *s1, size1);
	ft_memcpy(tmp + size1, s2, size2);
	tmp [size1 + size2] = 0;
	free(*s1);
	*s1 = tmp;
	return (1);
}

int	str_append_str(char **s1, char *s2)
{
	return (str_append_mem(s1, s2, ft_strlen(s2)));
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	if (dest > src)
		//return ft_memmove(dest, src, n); // -> recursividad (bucle infinito)
	{
		int i = n - 1;                       		// Depende de cuántos bytes queremos
		while (i >= 0)								//lo movemos a la condicion (dest > src)
		{
			((char *)dest)[i] = ((char *)src)[i];
			i--;
		}
	}
	//else if (dest == src)
	else if (dest < src) 
		//return (dest);
		return (ft_memcpy(dest, src, n));
	
	//size_t	i = ft_strlen((char *)src) - 1; // Depende del contenido de src
	return (dest);
}
/* Función correcta limpia:
void *ft_memmove(void *dest, const void *src, size_t n)
{
    if (dest > src)                      // dest está DESPUÉS → copiar DER→IZQ
    {
        int i = n - 1;
        while (i >= 0)
        {
            ((char *)dest)[i] = ((char *)src)[i];
            i--;
        }
    }
    else if (dest < src)                 // dest está ANTES → copiar IZQ→DER
    {
        return (ft_memcpy(dest, src, n));
    }
    return (dest);
}*/

char	*get_next_line(int fd)
{
	static char	b[BUFFER_SIZE + 1] = "";

	char	*ret = NULL;

	char	*tmp = ft_strchr(b, '\n');  //pasamos el buffer y busca '/n'
                                        //Si encuentra '\n', tmp apunta a esa posición, sino tmp=NULL.
	while (!tmp)
	{
		if (!str_append_str(&ret, b)) 			// añade el contenido actual de b a ret.
			return (NULL);
   
		int	read_ret = read(fd, b, BUFFER_SIZE); //read() lee fd BUFFER_SIZE bytes y los guarda en b.
		if (read_ret == -1)
			return (NULL);
        if (read_ret == 0)                      // -> Falta valorer si hay EOF (fin de archivo)
            break;
		b[read_ret] = 0;                        // añade el terminador '\0' al final
        tmp = ft_strchr(b, '\n');				// -> Falta esta linea (actualización)
    }
    
    if (tmp)									// --> FALTA si encontramos '/n' CASI TODO
    {                                    
        if (!str_append_mem(&ret, b, tmp - b + 1)) // Copia hasta '\n' incluido
        {
            free(ret);
            return (NULL);
        }
        ft_memmove(b, tmp + 1, ft_strlen(tmp + 1) + 1); // Guarda el resto en buffer para próxima llamada
        return (ret);
    } 
    else                            // Si llegamos al final sin '\n' (// tmp es NULL (no hay '\n', llegamos a EOF))
    {
        b[0] = '\0';  				// Vacías el buffer (fin de archivo)
        if (ret && *ret)            // Si leímos algo antes del EOF
            return (ret);			// Sí → devuelve lo que leíste
        else
        {
            free(ret);
            return (NULL);
        }
    }
}
#include <stdio.h>
#include <fcntl.h>
int main(void)
{
	int fd = open("test.txt", O_RDONLY);
	char *line;

	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}