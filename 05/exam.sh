#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'

# Directorios de trabajo
EXAM_DIR="$(cd "$(dirname "$0")" && pwd)"
RENDU_DIR="$EXAM_DIR/rendu"

# Verificar argumentos
if [ "$1" = "clean" ]; then
    echo -e "${RED}Limpiando...${NC}"
    rm -f $RENDU_DIR/cpp_module_00/cpp_module_00
    rm -f $RENDU_DIR/cpp_module_01/cpp_module_01
    rm -f $RENDU_DIR/cpp_module_02/cpp_module_02
    echo -e "${GREEN}Limpieza completada.${NC}"
    exit 0
fi

# Título
echo -e "${BLUE}${BOLD}==========================${NC}"
echo -e "${BLUE}${BOLD}   EXAM RANK 05 - TESTER${NC}"
echo -e "${BLUE}${BOLD}==========================${NC}"
echo ""

# Funciones
show_subject() {
    local module=$1
    clear
    echo -e "${BLUE}=== SUBJECT: cpp_module_$module ===${NC}\n"
    cat "$EXAM_DIR/cpp_module_$module/subject.en.txt"
    echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
    read
}

run_test() {
    local module=$1
    clear
    echo -e "${BLUE}=== TESTING: cpp_module_$module ===${NC}\n"
    
    # Verificar existencia del script de pruebas
    if [ ! -f "$EXAM_DIR/cpp_module_$module/test.sh" ]; then
        echo -e "${RED}Error: No se encuentra el script de pruebas para cpp_module_$module${NC}"
        echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
        read
        return
    fi
    
    # Dar permisos de ejecución
    chmod +x "$EXAM_DIR/cpp_module_$module/test.sh"
    
    # Ejecutar pruebas
    cd "$EXAM_DIR/cpp_module_$module"
    ./test.sh
    
    echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
    read
}

create_template() {
    local module=$1
    
    # Verificar si el directorio existe
    if [ ! -d "$RENDU_DIR/cpp_module_$module" ]; then
        echo -e "${RED}Error: No se encuentra el directorio cpp_module_$module${NC}"
        return
    fi
    
    case $module in
        00)
            echo -e "${YELLOW}Creando template para cpp_module_00...${NC}"
            cat > "$RENDU_DIR/cpp_module_00/Warlock.hpp" << 'EOL'
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Warlock.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvigara- <mvigara-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:44:49 by mvigara-          #+#    #+#             */
/*   Updated: 2025/05/09 16:44:59 by mvigara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WARLOCK_HPP
# define WARLOCK_HPP

# include <iostream>
# include <string>

class Warlock {
private:
    // Atributos privados
    
    // Constructor por defecto (privado)
    
    // Constructor de copia (privado)
    
    // Operador de asignación (privado)

public:
    // Constructor con nombre y título
    
    // Destructor
    
    // Getters (name y title)
    
    // Setter (title)
    
    // Método introduce
};

#endif
EOL
            cat > "$RENDU_DIR/cpp_module_00/Warlock.cpp" << 'EOL'
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Warlock.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvigara- <mvigara-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:44:49 by mvigara-          #+#    #+#             */
/*   Updated: 2025/05/09 16:44:59 by mvigara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Warlock.hpp"

// Implementación del constructor

// Implementación del destructor

// Implementación de los getters

// Implementación del setter

// Implementación del método introduce

EOL
            echo -e "${GREEN}Template para cpp_module_00 creado.${NC}"
            ;;
        01)
            echo -e "${YELLOW}Creando templates para cpp_module_01...${NC}"
            
            # Copiar archivos del módulo 00 si existen y modificar Warlock
            if [ -f "$RENDU_DIR/cpp_module_00/Warlock.hpp" ] && [ -f "$RENDU_DIR/cpp_module_00/Warlock.cpp" ]; then
                cp "$RENDU_DIR/cpp_module_00/Warlock.hpp" "$RENDU_DIR/cpp_module_01/"
                cp "$RENDU_DIR/cpp_module_00/Warlock.cpp" "$RENDU_DIR/cpp_module_01/"
                echo -e "${GREEN}Copiados archivos Warlock desde cpp_module_00.${NC}"
                echo -e "${YELLOW}Recuerda modificar Warlock para añadir:\n- learnSpell\n- forgetSpell\n- launchSpell${NC}"
            else
                # Crear templates desde cero
                cat > "$RENDU_DIR/cpp_module_01/Warlock.hpp" << 'EOL'
#ifndef WARLOCK_HPP
# define WARLOCK_HPP

# include <iostream>
# include <string>
# include <map>
# include "ASpell.hpp"

class Warlock {
private:
    std::string name;
    std::string title;
    std::map<std::string, ASpell*> spells;
    
    Warlock();
    Warlock(const Warlock&);
    Warlock& operator=(const Warlock&);

public:
    Warlock(const std::string& name, const std::string& title);
    ~Warlock();
    
    const std::string& getName() const;
    const std::string& getTitle() const;
    
    void setTitle(const std::string& title);
    
    void introduce() const;
    
    void learnSpell(ASpell* spell);
    void forgetSpell(const std::string& spellName);
    void launchSpell(const std::string& spellName, const ATarget& target);
};

#endif
EOL
            fi
            
            # Crear ASpell.hpp
            cat > "$RENDU_DIR/cpp_module_01/ASpell.hpp" << 'EOL'
#ifndef ASPELL_HPP
# define ASPELL_HPP

# include <string>

class ATarget;

class ASpell {
protected:
    std::string name;
    std::string effects;

public:
    ASpell();
    ASpell(const std::string& name, const std::string& effects);
    ASpell(const ASpell& other);
    ASpell& operator=(const ASpell& other);
    virtual ~ASpell();
    
    const std::string& getName() const;
    const std::string& getEffects() const;
    
    virtual ASpell* clone() const = 0;
    
    void launch(const ATarget& target) const;
};

# include "ATarget.hpp"

#endif
EOL
            
            # Crear archivos restantes
            echo -e "${GREEN}Template base para cpp_module_01 creado.${NC}"
            echo -e "${YELLOW}Falta implementar:\n- ASpell.cpp\n- ATarget.hpp y ATarget.cpp\n- Fwoosh.hpp y Fwoosh.cpp\n- Dummy.hpp y Dummy.cpp${NC}"
            ;;
        02)
            echo -e "${YELLOW}Creando templates básicos para cpp_module_02...${NC}"
            
            # Copiar archivos del módulo 01 si existen
            if [ -d "$RENDU_DIR/cpp_module_01" ]; then
                for file in "$RENDU_DIR/cpp_module_01"/*; do
                    basename=$(basename "$file")
                    cp "$file" "$RENDU_DIR/cpp_module_02/"
                done
                echo -e "${GREEN}Copiados archivos desde cpp_module_01.${NC}"
            fi
            
            # Crear SpellBook.hpp
            cat > "$RENDU_DIR/cpp_module_02/SpellBook.hpp" << 'EOL'
#ifndef SPELLBOOK_HPP
# define SPELLBOOK_HPP

# include <map>
# include <string>
# include "ASpell.hpp"

class SpellBook {
private:
    std::map<std::string, ASpell*> spells;
    
    SpellBook(const SpellBook&);
    SpellBook& operator=(const SpellBook&);

public:
    SpellBook();
    ~SpellBook();
    
    void learnSpell(ASpell* spell);
    void forgetSpell(const std::string& spellName);
    ASpell* createSpell(const std::string& spellName);
};

#endif
EOL
            
            echo -e "${GREEN}Template base para cpp_module_02 creado.${NC}"
            echo -e "${YELLOW}Falta implementar:\n- SpellBook.cpp\n- TargetGenerator.hpp y TargetGenerator.cpp\n- Fireball.hpp y Fireball.cpp\n- Polymorph.hpp y Polymorph.cpp\n- BrickWall.hpp y BrickWall.cpp${NC}"
            ;;
        *)
            echo -e "${RED}Módulo inválido: $module${NC}"
            ;;
    esac
}

# Menú principal
while true; do
    clear
    echo -e "${BLUE}=== 42 EXAM RANK 05 ====${NC}"
    echo -e "${YELLOW}Selecciona una opción:${NC}"
    echo "1. Ver Subject cpp_module_00"
    echo "2. Ver Subject cpp_module_01"
    echo "3. Ver Subject cpp_module_02"
    echo "4. Ejecutar Test cpp_module_00"
    echo "5. Ejecutar Test cpp_module_01"
    echo "6. Ejecutar Test cpp_module_02" 
    echo "7. Crear Template cpp_module_00"
    echo "8. Crear Template cpp_module_01"
    echo "9. Crear Template cpp_module_02"
    echo "0. Salir"
    
    read -p "Selección: " choice
    
    case $choice in
        1)
            show_subject "00"
            ;;
        2)
            show_subject "01"
            ;;
        3)
            show_subject "02"
            ;;
        4)
            run_test "00"
            ;;
        5)
            run_test "01"
            ;;
        6)
            run_test "02"
            ;;
        7)
            create_template "00"
            echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
            read
            ;;
        8)
            create_template "01"
            echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
            read
            ;;
        9)
            create_template "02"
            echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
            read
            ;;
        0)
            echo -e "${GREEN}¡Hasta luego!${NC}"
            exit 0
            ;;
        *)
            echo -e "${RED}Opción inválida${NC}"
            sleep 1
            ;;
    esac
done
