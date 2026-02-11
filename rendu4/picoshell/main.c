#include <stdio.h>                                                  
                                                                    
int picoshell(char **cmds[]);                                       
                                                                    
int main(void)                                                      
{
    // test 2
    char *cmd2[] = {"grep", "picoshell", NULL};
    char **cmds[] = {cmd1, cmd2, NULL};

    return picoshell(cmds);
}

/*
  char *cmd1[] = {"echo", "squalala", NULL};
  char *cmd2[] = {"cat", NULL};
  char *cmd3[] = {"sed", "s/a/b/g", NULL};
  char **cmds[] = {cmd1, cmd2, cmd3, NULL};
  return picoshell(cmds);
*/