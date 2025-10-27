

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <fcntl.h>

//cc broken_gnl.c -Wall -Wextra -Werror -D BUFFER_SIZE=3 -o gnl


int ft_strlen(char *c)
{
	int i;


	return(i);
}

char *ft_strchr(char *s, int c)
{


	return(NULL);
}

char *ft_memmove(char *dest, char *src, int n)
{


	return(dest);
}

char *ft_join(char *s1, char *s2, int n)
{

	ft_memmove(retj, s1, size1);
	ft_memmove(retj + size1, s2, n);
	return(retj);
}

char *b_gnl(fd)
{
	static char buf;
	char *tmp = NULL;
	char *rest = NULL;
	int nb_read = 0;

//	printf("buffer_size  = %d\n", BUFFER_SIZE);

	while()
	{
		if(buf[0] == '\0')
		{

		}
		rest = ft_strchr(buf, '\n');
		if()
		{
			return();
		}
		else
		{

		}
	}
	return();
}

int main(void)
{
	int fd = 0;

	fd = open("text1.txt", O_RDONLY);
	if(fd < 0)
		perror("open");

	printf("line = %s", b_gnl(fd));
	printf("line = %s", b_gnl(fd));
	printf("line = %s", b_gnl(fd));
	printf("line = %s", b_gnl(fd));
	printf("line = %s", b_gnl(fd));
	printf("line = %s", b_gnl(fd));
	printf("line = %s", b_gnl(fd));


	return(0);
}
