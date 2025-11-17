#include "repaired_gnl.h"

char	*ft_strchr(char *s, int c)
{
	int	i = 0;
	while (s[i] != c && s[i] != '\0')
		i++;
	if (s[i] == c)
		return (s + i);
	else
		return (NULL);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	while (n-- > 0)     //comprueba si n > 0, después decrementa n en una unidad.
		((char *)dest)[n] = ((char *)src)[n];
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
		return (ft_memcpy(dest, src, n));
	else if (dest == src)
		return (dest);
	//size_t	i = ft_strlen((char *)src) - 1; // Depende del contenido de src
	int i = n - 1;                       	// Depende de cuántos bytes queremos
	while (i >= 0)
	{
		((char *)dest)[i] = ((char *)src)[i];
		i--;
	}
	return (dest);
}

char	*get_next_line(int fd)
{
	static char	b[BUFFER_SIZE + 1] = "";
	char	*ret = NULL;

	char	*tmp = ft_strchr(b, '\n');  //pasamos el buffer y busca '/n'
                                        //Si encuentra '\n', tmp apunta a esa posición, sino tmp=NULL.
	while (!tmp)
	{
		if (!str_append_str(&ret, b))
			return (NULL);
   
		int	read_ret = read(fd, b, BUFFER_SIZE); //read() pone los datos leídos en b
		if (read_ret == -1)
			return (NULL);
        if (read_ret == 0)                      //(fin de archivo)
            break;
		b[read_ret] = 0;                        //añade el terminador '\0' al final
        tmp = ft_strchr(b, '\n');
    }
    
    if (tmp)
    {                                    //si encontramos '/n'
        if (!str_append_mem(&ret, b, tmp - b + 1))
        {
            free(ret);
            return (NULL);
        }
        ft_memmove(b, tmp + 1, ft_strlen(tmp + 1) + 1);
        return (ret);
    } 
    else                            // Si llegamos al final sin '\n'
    {
        b[0] = '\0';  // AQUÍ SÍ vacías el buffer (fin de archivo)
        if (ret && *ret)              // Si leímos algo antes
            return (ret);
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