#include "argo.h"

int parse_object(json *dst, FILE *stream)
  {
      dst->type = MAP;
      dst->map.data = NULL;
      dst->map.size = 0;

      // Consumir '{'
      if (!expect(stream, '{'))
          return -1;

      // Caso objeto vacío: {}
      if (peek(stream) == '}')
      {
          getc(stream);
          return 1;
      }

      // Parsear pares
      while (1)
      {
          // Incrementar size y hacer realloc
          dst->map.size++;
          dst->map.data = realloc(dst->map.data, dst->map.size * sizeof(pair));
          if (!dst->map.data)
              return -1;

          pair *current = &dst->map.data[dst->map.size - 1];

          // Parsear key (siempre es un string)
          json key_json;
          if (parse_string(&key_json, stream) != 1)
              return -1;
          current->key = key_json.string;

          // Consumir ':'
          if (!expect(stream, ':'))
              return -1;

          // Parsear value (puede ser cualquier cosa)
          if (parse_value(&current->value, stream) != 1)
              return -1;

          // Verificar qué viene después
          int c = peek(stream);
          if (c == '}')
          {
              getc(stream);
              break;  // Fin del objeto
          }
          else if (c == ',')
          {
              getc(stream);
              continue;  // Hay más pares
          }
          else
          {
              unexpected(stream);
              return -1;
          }
      }

      return 1;
  }

int parse_integer(json *dst, FILE *stream)
{
    dst->type = INTEGER;
    int res = fscanf(stream, "%d", &dst->integer);
    if(res > 0) return 1;
    else return -1;
}

int parse_string(json *dst, FILE *stream)
{
    dst->type = STRING;
    dst->string = NULL;
    
    if (!expect(stream, '"')) return -1;

    int size = 0;
    int capacity = 16;
    char *buf = malloc(capacity);
    if(!buf)
        return -1;
    

    //leer hasta '"' final
    int c;
    while((c = peek(stream)) != '"')
    {
        if(c == EOF)
        {
            free(buf);
            unexpected(stream);
            return -1;
        }
        if(c == '\\')
        {
            getc(stream);
            c = getc(stream);
            if(c == EOF)
            {
                free(buf);
                unexpected(stream);
                return -1;
            }
        }
        else getc(stream);

        if (size >= capacity)
        {
            capacity *= 2;
            char *tmp = realloc(buf, capacity);
            if (!tmp)
            {
                free(buf);
                return -1;
            }
            buf = tmp;
        }

        buf[size++] = c;
    }
    getc(stream);
    //añadif \0 al final de buf
    if (size >= capacity)
    {
        char *tmp = realloc(buf, capacity);
        if (!tmp)
        {
            free(buf);
            return -1;
        }
        buf = tmp;
    }
    buf[size] = '\0';

    dst->string = buf;
    return 1;
}

int parse_value(json *dst, FILE *stream)
{
    int c = peek(stream);

    if (c == '"')
        return parse_string(dst, stream);
    else if (c == '-' || isdigit(c))
        return parse_integer(dst, stream);
    else if (c == '{')
        return parse_object(dst, stream);
    else
    {
        unexpected(stream);
        return -1;
    }
}

int argo(json *dst, FILE *stream)
{
    if (!dst || !stream)
        return -1;
    return parse_value(dst, stream);
}