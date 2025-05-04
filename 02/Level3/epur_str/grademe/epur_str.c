    #include <unistd.h>

    int is_space(char c)
    {
        return (c == ' ' || c == '\t');
    }

    int main(int argc, char **argv)
    {
        if(argc != 2)
        {
            write(1, "\n", 1);
            return 1;
        }
            
        char *s = argv[1];
        int printed = 0;
        
        while(*s)
        {
            if(is_space(*s)) s++;
            if(*s && !is_space(*s))
            {
                if(printed)
                    write(1, " ", 1);
                while(*s && !is_space(*s))
                {
                    write(1, s, 1);
                    s++;
                    printed = 1;
                }

            }
            else
                while(*s && is_space(*s)) s++;
        }
    write(1, "\n", 1);
        return 0;
    }
