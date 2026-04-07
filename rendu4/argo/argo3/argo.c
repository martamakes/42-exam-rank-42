#include "argo.h"

int parse_string(char **dst, FILE *stream)
{ 
    if (!expect(stream, '"'))           // 1. abrir
          return -1;

      char *buf = malloc(42);
      if (!buf) return -1;
      int len = 0, cap = 42;

      int c;
      while ((c = getc(stream)) != '"')   // 2. bucle hasta cierre
      {
          if (c == EOF)                   // nunca cerró
              { free(buf); unexpected(stream); return -1; }
          if (c == '\\')                  // escape
          {
              c = getc(stream);
              if (c != '\\' && c != '"')
                  { free(buf); unexpected(stream); return -1; }
          }
          if (len + 2 > cap)             // crecer buffer
          {
              cap *= 2;
              char *tmp = realloc(buf, cap);
              if (!tmp) { free(buf); return -1; }
              buf = tmp;
          }
          buf[len++] = c;                // guardar char
      }
      buf[len] = '\0';                    // 3. terminar
      *dst = buf;
      return 1;

}

int parse_map(json *dst, FILE *stream)
{
   if (!expect(stream, '{'))                   // 1. abrir                                                                                                        
          return -1;
                                                                                                                                                                     
      if (accept(stream, '}'))                    // 2. map vacío {}                                                                                                 
          return 1;
                                                                                                                                                                     
      pair *tmp;  
      while (1)
      {
          // 3. crecer array de pares
          tmp = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
          if (!tmp)                                                                                                                                                  
              return -1;
          dst->map.data = tmp;                                                                                                                                       
                  
          // 4. parsear key (siempre es string)                                                                                                                      
          if (parse_string(&dst->map.data[dst->map.size].key, stream) != 1)
              return -1;                                                                                                                                             
                  
          // 5. separador :
          if (!expect(stream, ':'))
              return -1;                                                                                                                                             
   
          // 6. parsear value (puede ser string, int o map)                                                                                                          
          if (argo(&dst->map.data[dst->map.size].value, stream) != 1)
              return -1;                                                                                                                                             
   
          dst->map.size++;                                                                                                                                           
                  
          // 7. ¿hay más pares? si no hay coma, salir                                                                                                                
          if (!accept(stream, ','))
              break;                                                                                                                                                 
      }           

      if (!expect(stream, '}'))                   // 8. cerrar                                                                                                       
          return -1;
      return 1;   
}
int     argo(json *dst, FILE *stream)
{
    int c = peek(stream);
    switch(c)
    {
        case '"':
            dst->type = STRING;
            dst->string = NULL;
            return parse_string(&dst->string, stream);
        case '{':
            dst->type = MAP;
            dst->map.data = NULL;
            dst->map.size = 0;
            return parse_map(dst, stream);
        case '-':
        case '0' : case '1' : case '2' : case '3' : case '4' : case '5' : case '6' : case '7' : case '8' : case '9' : 
            dst->type = INTEGER;
            if(fscanf(stream, "%d", &dst->integer) != 1)
            {
                unexpected(stream);
                return -1;
            }
            return 1;
        default:
            unexpected(stream);
            return -1;
    }
    
}