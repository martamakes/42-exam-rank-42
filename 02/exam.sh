#!/bin/bash

# Función para seleccionar un ejercicio al azar de un nivel
select_random_exercise() {
    level=$1
    exercises=($(ls Level$level))
    random_index=$((RANDOM % ${#exercises[@]}))
    echo "${exercises[$random_index]}"
}

# Seleccionar ejercicios al azar para cada nivel
exercise1=$(select_random_exercise 1)
exercise2=$(select_random_exercise 2)
exercise3=$(select_random_exercise 3)
exercise4=$(select_random_exercise 4)

# Pedir confirmación para iniciar el examen
echo "Ejercicios seleccionados:"
echo "Nivel 1: $exercise1"
echo "Nivel 2: $exercise2"
echo "Nivel 3: $exercise3"
echo "Nivel 4: $exercise4"
echo "¿Quieres iniciar el examen? (s/n)"
read confirmation

if [ "$confirmation" != "s" ]; then
    echo "Examen cancelado."
    exit 0
fi

# Crear directorios necesarios
mkdir -p rendu subject

# Copiar los README.md de los ejercicios seleccionados
cp "Level1/$exercise1/README.md" "subject/level_1_$exercise1.md"
cp "Level2/$exercise2/README.md" "subject/level_2_$exercise2.md"
cp "Level3/$exercise3/README.md" "subject/level_3_$exercise3.md"
cp "Level4/$exercise4/README.md" "subject/level_4_$exercise4.md"

# Función para mostrar la cuenta regresiva
countdown() {
    start=$(($(date +%s) + 3*3600))
    while [ $start -ge $(date +%s) ]; do
        time_left=$((start - $(date +%s)))
        printf "\rTiempo restante: %02d:%02d:%02d" $((time_left/3600)) $(((time_left%3600)/60)) $((time_left%60))
        sleep 1
    done
    echo -e "\n¡Tiempo terminado!"
}

# Iniciar la cuenta regresiva en segundo plano
countdown &

# Abrir el primer subject en otra terminal
gnome-terminal -- less "subject/level_1_$exercise1.md"

echo "El examen ha comenzado. Buena suerte!"