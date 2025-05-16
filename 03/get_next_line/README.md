# get_next_line

## Enunciado

Escribe una función llamada `get_next_line` que devuelva una línea leída de un file descriptor.

## Prototipo

```c
char *get_next_line(int fd);
```

## Parámetros

- `fd`: El file descriptor desde donde leer

## Retorno

- Si todo va bien, la función devuelve la línea leída.
- Si no hay nada más que leer o si ocurre un error, devuelve NULL.

## Descripción

- La línea devuelta debe incluir el carácter de nueva línea final '\n', excepto si se alcanza el final del archivo (EOF) y éste no termina con un carácter de nueva línea.
- El buffer size para la lectura está definido como BUFFER_SIZE.
- La función debe ser capaz de manejar cualquier BUFFER_SIZE.

## Restricciones

- Tu función debe gestionar correctamente memoria: no debe tener fugas de memoria.
- No puedes usar funciones de la librería estándar excepto las que ya permiten en el subject original: read, malloc y free.

## Ejemplos

```c
int fd = open("archivo.txt", O_RDONLY);
char *line;

// Leer primera línea
line = get_next_line(fd);
printf("%s", line);
free(line);

// Leer segunda línea
line = get_next_line(fd);
printf("%s", line);
free(line);

// Continuar leyendo hasta el final
while ((line = get_next_line(fd)) != NULL)
{
    printf("%s", line);
    free(line);
}

close(fd);
```

## Consejos

- Piensa en cómo gestionar un buffer de lectura
- Necesitarás guardar lo que queda del buffer entre llamadas a la función
- Ten en cuenta que puedes necesitar realizar múltiples lecturas para obtener una línea completa
- Siempre verifica el valor de retorno de read y gestiona correctamente los errores