#include <unistd.h>

int ft_popen(const char *file, char *const argv[], char type);

int main()                                                                             
  {                                                                                      
      int fd;                                                                            
      char buffer[4096];                                                                 
      int bytes = 0;                                                                         
                                                                                         
      fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');                            
      if(fd != -1)
      {
        bytes = read(fd, buffer, sizeof(buffer) - 1); 
        if(bytes > 0) write(1, buffer, bytes); 
      }                                                   
      close(fd);                                                                                   
      return (0);                                                                        
  } 
