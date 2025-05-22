#!/bin/bash

# Script para configurar enlaces simbólicos a los ejercicios originales

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}🔗 Configurando enlaces simbólicos a ejercicios...${NC}"

# Verificar que el directorio de ejercicios originales existe
if [[ ! -d "../02" ]]; then
    echo -e "${RED}❌ No se encuentra el directorio de ejercicios originales (../02)${NC}"
    exit 1
fi

# Crear directorio de enlaces si no existe
mkdir -p links

# Lista de todos los ejercicios y sus ubicaciones
declare -A exercises
exercises["ft_putstr"]="Level1"
exercises["ft_swap"]="Level1"
exercises["ft_strlen"]="Level1"
exercises["ft_strcpy"]="Level1"
exercises["first_word"]="Level1"
exercises["fizzbuzz"]="Level1"
exercises["repeat_alpha"]="Level1"
exercises["rev_print"]="Level1"
exercises["rot_13"]="Level1"
exercises["rotone"]="Level1"
exercises["search_and_replace"]="Level1"
exercises["ulstr"]="Level1"

exercises["alpha_mirror"]="Level2"
exercises["camel_to_snake"]="Level2"
exercises["do_op"]="Level2"
exercises["ft_atoi"]="Level2"
exercises["ft_strcmp"]="Level2"
exercises["ft_strcspn"]="Level2"
exercises["ft_strdup"]="Level2"
exercises["ft_strpbrk"]="Level2"
exercises["ft_strrev"]="Level2"
exercises["ft_strspn"]="Level2"
exercises["inter"]="Level2"
exercises["is_power_of_2"]="Level2"
exercises["last_word"]="Level2"
exercises["max"]="Level2"
exercises["print_bits"]="Level2"
exercises["reverse_bits"]="Level2"
exercises["snake_to_camel"]="Level2"
exercises["swap_bits"]="Level2"
exercises["union"]="Level2"
exercises["wdmatch"]="Level2"

exercises["add_prime_sum"]="Level3"
exercises["epur_str"]="Level3"
exercises["expand_str"]="Level3"
exercises["ft_atoi_base"]="Level3"
exercises["ft_list_size"]="Level3"
exercises["ft_range"]="Level3"
exercises["ft_rrange"]="Level3"
exercises["hidenp"]="Level3"
exercises["lcm"]="Level3"
exercises["paramsum"]="Level3"
exercises["pgcd"]="Level3"
exercises["print_hex"]="Level3"
exercises["rstr_capitalizer"]="Level3"
exercises["str_capitalizer"]="Level3"
exercises["tab_mult"]="Level3"

exercises["flood_fill"]="Level4"
exercises["fprime"]="Level4"
exercises["ft_itoa"]="Level4"
exercises["ft_list_foreach"]="Level4"
exercises["ft_list_remove_if"]="Level4"
exercises["ft_split"]="Level4"
exercises["rev_wstr"]="Level4"
exercises["rostring"]="Level4"
exercises["sort_int_tab"]="Level4"
exercises["sort_list"]="Level4"

# Crear enlaces simbólicos
created_links=0
for exercise in "${!exercises[@]}"; do
    level="${exercises[$exercise]}"
    source_path="../02/${level}/${exercise}"
    link_path="links/${exercise}"
    
    if [[ -d "$source_path" ]]; then
        # Eliminar enlace existente si existe
        [[ -L "$link_path" ]] && rm "$link_path"
        
        # Crear nuevo enlace simbólico
        if ln -s "$(realpath "$source_path")" "$link_path" 2>/dev/null; then
            echo -e "${GREEN}✓${NC} $exercise -> $level"
            ((created_links++))
        else
            echo -e "${RED}❌${NC} Error creando enlace para $exercise"
        fi
    else
        echo -e "${YELLOW}⚠️${NC}  No encontrado: $exercise en $level"
    fi
done

echo ""
echo -e "${GREEN}✓ Configuración completada: $created_links enlaces creados${NC}"
echo -e "${BLUE}Los ejercicios están disponibles en el directorio 'links/'${NC}"
