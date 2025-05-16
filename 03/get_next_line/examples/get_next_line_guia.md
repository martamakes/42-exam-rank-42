## Guía para implementar get_next_line

Esta guía te ayudará a entender cómo implementar `get_next_line` paso a paso.

### 1. Entendiendo el prototipo

```c
char *get_next_line(int fd);
```

- `fd`: File descriptor desde donde leer
- Retorna: Una línea leída del fd (incluyendo '\n') o NULL si ocurre un error o se alcanza EOF

### 2. Lógica fundamental

La función debe:
1. Leer del file descriptor en trozos de tamaño BUFFER_SIZE
2. Buscar un salto de línea ('\n') en lo leído
3. Si encuentra un salto, devolver la línea hasta ese punto (incluido el '\n')
4. Si no encuentra un salto, seguir leyendo
5. Guardar cualquier exceso de lectura para la próxima llamada

### 3. Variables estáticas

Necesitas guardar lo que sobra después de un '\n' para la próxima llamada. Esto se hace con una variable estática:

```c
static char *remaining_text = NULL;
```

### 4. Implementación básica

```c
char *get_next_line(int fd)
{
    static char *remaining = NULL;  // Para guardar lo que sobra entre llamadas
    char *line = NULL;              // La línea que vamos a devolver
    char *buffer = NULL;            // Buffer para leer
    int bytes_read = 0;
    
    // Verificar argumentos válidos
    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    
    // Asignar memoria para el buffer
    buffer = malloc(BUFFER_SIZE + 1);
    if (!buffer)
        return (NULL);
    
    // Usar lo que quedó de la llamada anterior (si hay algo)
    if (remaining)
    {
        line = process_remaining(&remaining);
        if (line)  // Si ya tenemos una línea completa, devolverla
        {
            free(buffer);
            return (line);
        }
    }
    
    // Leer del file descriptor
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        buffer[bytes_read] = '\0';  // Asegurar terminación de string
        
        // Procesar lo leído
        line = process_buffer(buffer, &remaining);
        
        if (line)  // Si tenemos una línea, devolverla
        {
            free(buffer);
            return (line);
        }
    }
    
    free(buffer);
    
    // Si llegamos aquí, o hubo error de lectura o llegamos al EOF
    if (bytes_read < 0)  // Error de lectura
        return (free_remaining(&remaining), NULL);
    
    // Devolver lo que queda (sin '\n', estamos en EOF)
    return (get_final_line(&remaining));
}
```

### 5. Funciones auxiliares para procesar texto

#### Procesar texto remanente:
```c
char *process_remaining(char **remaining)
{
    char *newline_ptr;
    char *line;
    char *temp;
    
    // Buscar un salto de línea
    newline_ptr = ft_strchr(*remaining, '\n');
    
    if (newline_ptr)  // Si hay un salto de línea
    {
        *newline_ptr = '\0';  // Reemplazar '\n' con '\0' temporalmente
        line = ft_strdup(*remaining);  // Duplicar hasta el '\0'
        *newline_ptr = '\n';  // Restaurar el '\n'
        
        // Añadir manualmente el '\n' a la línea
        line = ft_strjoin_free(line, "\n");
        
        // Actualizar el remanente para la próxima llamada
        temp = ft_strdup(newline_ptr + 1);
        free(*remaining);
        *remaining = temp;
        
        return (line);
    }
    
    // Si no hay '\n', todo el remanente es parte de la línea actual
    line = ft_strdup(*remaining);
    free(*remaining);
    *remaining = NULL;
    
    return (line);
}
```

#### Procesar buffer recién leído:
```c
char *process_buffer(char *buffer, char **remaining)
{
    char *newline_ptr;
    char *line;
    
    // Buscar un salto de línea en el buffer
    newline_ptr = ft_strchr(buffer, '\n');
    
    if (newline_ptr)  // Si hay un salto de línea
    {
        *newline_ptr = '\0';  // Reemplazar '\n' con '\0'
        
        // Crear la línea (con los remanentes anteriores si los hay)
        if (*remaining)
        {
            line = ft_strjoin(*remaining, buffer);
            free(*remaining);
        }
        else
            line = ft_strdup(buffer);
        
        // Añadir el '\n'
        line = ft_strjoin_free(line, "\n");
        
        // Guardar lo que sobra después del '\n'
        *remaining = ft_strdup(newline_ptr + 1);
        
        return (line);
    }
    
    // Si no hay '\n', añadir todo el buffer al remanente
    if (*remaining)
    {
        line = ft_strjoin(*remaining, buffer);
        free(*remaining);
        *remaining = line;
    }
    else
        *remaining = ft_strdup(buffer);
    
    return (NULL);  // No tenemos una línea completa todavía
}
```

### 6. Funciones auxiliares de string

Necesitarás implementar varias funciones auxiliares:

- `ft_strchr`: Buscar la primera ocurrencia de un carácter en una string
- `ft_strdup`: Duplicar una string
- `ft_strjoin`: Unir dos strings
- `ft_strjoin_free`: Unir dos strings y liberar la primera

### 7. Consideraciones importantes

- Gestión de memoria: Liberar todo lo que reserves
- BUFFER_SIZE puede ser cualquier número positivo
- Manejar correctamente los errores de read()
- Cuidado con los casos límite (fd inválido, buffer size negativo)
- La línea devuelta debe incluir '\n' a menos que sea EOF

### 8. Versión simplificada (solo para examen)

Para el examen, puedes usar una versión más simple que lee carácter a carácter:

```c
char *get_next_line(int fd)
{
    char *line;
    char character;
    int i = 0;
    int rd;
    
    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    
    line = malloc(10000);  // Tamaño arbitrario grande
    if (!line)
        return (NULL);
    
    rd = read(fd, &character, 1);
    while (rd > 0)
    {
        line[i++] = character;
        if (character == '\n')
            break;
        rd = read(fd, &character, 1);
    }
    
    line[i] = '\0';
    
    if (rd <= 0 && i == 0)
    {
        free(line);
        return (NULL);
    }
    
    return (line);
}
```

Recuerda que esta implementación simplificada no es óptima pero puede funcionar para el examen si te bloqueas con la versión completa.