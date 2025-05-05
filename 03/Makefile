# Makefile para los modos guiados del Exam Rank 03
# Autor: Marta Vigaray

# Definir colores para la salida
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
PURPLE = \033[0;35m
RESET = \033[0m

# Directorios
PRINTF_DIR = ft_printf
GNL_DIR = get_next_line

# Archivos ft_printf
PRINTF_GUIDE_MAIN = $(PRINTF_DIR)/printf_guide.c
PRINTF_UTILS = $(PRINTF_DIR)/printf_guide_utils.c
PRINTF_STEPS_1_2 = $(PRINTF_DIR)/printf_steps_1_2.c
PRINTF_STEPS_3_4 = $(PRINTF_DIR)/printf_steps_3_4.c
PRINTF_STEPS_5_6 = $(PRINTF_DIR)/printf_steps_5_6.c
PRINTF_GUIDE_SRC = $(PRINTF_UTILS) $(PRINTF_STEPS_1_2) $(PRINTF_STEPS_3_4) $(PRINTF_STEPS_5_6) $(PRINTF_GUIDE_MAIN)

# Archivos get_next_line
GNL_GUIDE_MAIN = $(GNL_DIR)/gnl_guide.c
GNL_UTILS = $(GNL_DIR)/guide_utils.c
GNL_STEPS_1_2 = $(GNL_DIR)/gnl_steps_1_2.c
GNL_STEPS_3_4 = $(GNL_DIR)/gnl_steps_3_4.c
GNL_STEPS_5_6 = $(GNL_DIR)/gnl_steps_5_6.c
GNL_GUIDE_SRC = $(GNL_UTILS) $(GNL_STEPS_1_2) $(GNL_STEPS_3_4) $(GNL_STEPS_5_6) $(GNL_GUIDE_MAIN)

# Ejecutables
PRINTF_GUIDE_EXE = $(PRINTF_DIR)/printf_guide
GNL_GUIDE_EXE = $(GNL_DIR)/gnl_guide

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Reglas
all: $(PRINTF_GUIDE_EXE) $(GNL_GUIDE_EXE)
	@echo "$(GREEN)¡Modos guiados compilados con éxito!$(RESET)"
	@echo ""
	@echo "$(PURPLE)INSTRUCCIONES DE USO:$(RESET)"
	@echo "$(BLUE)1. Los archivos de solución deben crearse en:$(RESET)"
	@echo "   - Para ft_printf: rendu/ft_printf/ft_printf.c"
	@echo "   - Para get_next_line: rendu/get_next_line/get_next_line.c y get_next_line.h"
	@echo ""
	@echo "$(BLUE)2. Para iniciar los modos guiados:$(RESET)"
	@echo "   - Guía de ft_printf:     $(GREEN)make printf_guide$(RESET)"
	@echo "   - Guía de get_next_line: $(GREEN)make gnl_guide$(RESET)"
	@echo ""
	@echo "$(BLUE)3. Cada guía te llevará paso a paso por la implementación del ejercicio$(RESET)"
	@echo "   y probará tu código automáticamente."
	@echo ""
	@echo "$(PURPLE)¡BUENA SUERTE EN TU EXAMEN!$(RESET)"

$(PRINTF_GUIDE_EXE): $(PRINTF_GUIDE_SRC)
	@echo "$(PURPLE)Compilando modo guiado ft_printf...$(RESET)"
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "$(GREEN)¡Compilación de ft_printf_guide exitosa!$(RESET)"

$(GNL_GUIDE_EXE): $(GNL_GUIDE_SRC)
	@echo "$(PURPLE)Compilando modo guiado get_next_line...$(RESET)"
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "$(GREEN)¡Compilación de gnl_guide exitosa!$(RESET)"

printf_guide: $(PRINTF_GUIDE_EXE)
	@echo "$(BLUE)Iniciando modo guiado para ft_printf...$(RESET)"
	@echo "$(YELLOW)RECUERDA:$(RESET) Debes crear tu solución en $(GREEN)rendu/ft_printf/ft_printf.c$(RESET)"
	@cd $(PRINTF_DIR) && ./$(notdir $(PRINTF_GUIDE_EXE))

gnl_guide: $(GNL_GUIDE_EXE)
	@echo "$(BLUE)Iniciando modo guiado para get_next_line...$(RESET)"
	@echo "$(YELLOW)RECUERDA:$(RESET) Debes crear tu solución en: $(GREEN)rendu/get_next_line/get_next_line.c$(RESET) y $(GREEN)get_next_line.h$(RESET)"
	@cd $(GNL_DIR) && ./$(notdir $(GNL_GUIDE_EXE))

clean:
	@echo "$(RED)Limpiando archivos temporales...$(RESET)"
	@rm -f $(PRINTF_DIR)/*.o $(GNL_DIR)/*.o

fclean: clean
	@echo "$(RED)Limpiando ejecutables...$(RESET)"
	@rm -f $(PRINTF_GUIDE_EXE) $(GNL_GUIDE_EXE)

re: fclean all

.PHONY: all clean fclean re printf_guide gnl_guide
