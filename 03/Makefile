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
PRINTF_GUIDE = $(PRINTF_DIR)/ft_printf_guide.c

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
	@echo "$(GREEN)Modos guiados listos para usar:$(RESET)"
	@echo "$(BLUE)- Modo guiado ft_printf: $(PRINTF_GUIDE_EXE)$(RESET)"
	@echo "$(BLUE)- Modo guiado get_next_line: $(GNL_GUIDE_EXE)$(RESET)"

$(PRINTF_GUIDE_EXE): $(PRINTF_GUIDE)
	@echo "$(PURPLE)Compilando modo guiado ft_printf...$(RESET)"
	@$(CC) $(CFLAGS) -o $@ $<
	@echo "$(GREEN)¡Compilación de ft_printf_guide exitosa!$(RESET)"

$(GNL_GUIDE_EXE): $(GNL_GUIDE_SRC)
	@echo "$(PURPLE)Compilando modo guiado get_next_line...$(RESET)"
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "$(GREEN)¡Compilación de gnl_guide exitosa!$(RESET)"

printf_guide: $(PRINTF_GUIDE_EXE)
	@echo "$(BLUE)Iniciando modo guiado para ft_printf...$(RESET)"
	@cd $(PRINTF_DIR) && ./$(notdir $(PRINTF_GUIDE_EXE))

gnl_guide: $(GNL_GUIDE_EXE)
	@echo "$(BLUE)Iniciando modo guiado para get_next_line...$(RESET)"
	@cd $(GNL_DIR) && ./$(notdir $(GNL_GUIDE_EXE))

clean:
	@echo "$(RED)Limpiando archivos temporales...$(RESET)"
	@rm -f $(PRINTF_DIR)/*.o $(GNL_DIR)/*.o

fclean: clean
	@echo "$(RED)Limpiando ejecutables...$(RESET)"
	@rm -f $(PRINTF_GUIDE_EXE) $(GNL_GUIDE_EXE)

re: fclean all

.PHONY: all clean fclean re printf_guide gnl_guide
