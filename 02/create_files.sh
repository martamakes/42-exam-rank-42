#!/bin/bash

# Recorre todas las carpetas en el directorio actual
for dir in */; do
    # Elimina la barra final del nombre de la carpeta
    dir=${dir%/}
    
    # Crea el archivo README.md en la carpeta
    echo "# $dir" > "$dir/README.md"
    
    
    # Crea el archivo .c con el nombre de la carpeta
    touch "$dir/$dir.c"
    
    
    echo "Creados README.md y $dir.c en $dir"
done
