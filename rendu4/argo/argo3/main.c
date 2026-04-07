#include "argo.h"

int     peek(FILE *stream)
{
        int     c = getc(stream);
        ungetc(c, stream);
        return c;
}

void    unexpected(FILE *stream)
{
        if (peek(stream) != EOF)
                printf("unexpected token '%c'\n", peek(stream));
        else
                printf("unexpected end of input\n");
}

int     accept(FILE *stream, char c)
{
        if (peek(stream) == c)
        {
                (void)getc(stream);
                return 1;
        }
        return 0;
}

int     expect(FILE *stream, char c)
{
        if (accept(stream, c))
                return 1;
        unexpected(stream);
        return 0;
}

void    free_json(json j)
{
        switch (j.type)
        {
                case MAP:
                        for (size_t i = 0; i < j.map.size; i++)
                        {
                                free(j.map.data[i].key);
                                free_json(j.map.data[i].value);
                        }
                        free(j.map.data);
                        break ;
                case STRING:
                        free(j.string);
                        break ;
                default:
                        break ;
        }
}

void    serialize(json j)
{
        switch (j.type)
        {
                case INTEGER:
                        printf("%d", j.integer);
                        break ;
                case STRING:
                        putchar('"');
                        for (int i = 0; j.string[i]; i++)
                        {
                                if (j.string[i] == '\\' || j.string[i] == '"')
                                        putchar('\\');
                                putchar(j.string[i]);
                        }
                        putchar('"');
                        break ;
                case MAP:
                        putchar('{');
                        for (size_t i = 0; i < j.map.size; i++)
                        {
                                if (i != 0)
                                        putchar(',');
                                serialize((json){.type = STRING, .string = j.map.data[i].key});
                                putchar(':');
                                serialize(j.map.data[i].value);
                        }
                        putchar('}');
                        break ;
        }
}

int     main(int argc, char **argv)
{
        if (argc != 2)
                return 1;
        char *filename = argv[1];
        FILE *stream = fopen(filename, "r");
        json    file;
        if (argo (&file, stream) != 1)
        {
                free_json(file);
                return 1;
        }
        serialize(file);
        printf("\n");
}
