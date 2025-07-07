# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marta <marta@student.42madrid.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/06 00:00:00 by marta             #+#    #+#              #
#    Updated: 2025/07/06 00:00:00 by marta            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# EXAMSHELL - 42 School Exam Practice System Makefile (Modular Version)

NAME = examshell

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

# Archivos fuente (modular)
SRCS = main.c \
       ui.c \
       persistence.c \
       exercises.c \
       exam_session.c

# Archivos objeto
OBJS = $(SRCS:.c=.o)

# Archivos de cabecera
HEADERS = examshell.h

# Colores para output
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
BLUE = \033[0;34m
NC = \033[0m # No Color

# Regla principal
all: $(NAME)

# Compilación del ejecutable
$(NAME): $(OBJS)
	@echo "$(YELLOW)Linking examshell...$(NC)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)✅ examshell compiled successfully!$(NC)"
	@echo "$(BLUE)📁 Modular structure:$(NC)"
	@echo "$(BLUE)  - main.c: Entry point and argument handling$(NC)"
	@echo "$(BLUE)  - ui.c: User interface functions$(NC)"
	@echo "$(BLUE)  - persistence.c: Progress saving/loading$(NC)"
	@echo "$(BLUE)  - exercises.c: Exercise management$(NC)"
	@echo "$(BLUE)  - exam_session.c: Main exam logic$(NC)"
	@echo "$(GREEN)Usage: ./examshell [rank_number]$(NC)"

# Compilación de archivos objeto
%.o: %.c $(HEADERS)
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Instalación global (opcional)
install: $(NAME)
	@echo "$(YELLOW)Installing examshell globally...$(NC)"
	@sudo cp $(NAME) /usr/local/bin/
	@echo "$(GREEN)✅ examshell installed! You can now use 'examshell' from anywhere.$(NC)"

# Desinstalación
uninstall:
	@echo "$(YELLOW)Uninstalling examshell...$(NC)"
	@sudo rm -f /usr/local/bin/examshell
	@echo "$(GREEN)✅ examshell uninstalled.$(NC)"

# Limpiar archivos objeto
clean:
	@echo "$(YELLOW)Cleaning object files...$(NC)"
	@rm -f $(OBJS)
	@echo "$(GREEN)✅ Object files cleaned.$(NC)"

# Limpiar todo
fclean: clean
	@echo "$(YELLOW)Cleaning executable and progress files...$(NC)"
	@rm -f $(NAME)
	@rm -f .examshell_progress
	@echo "$(GREEN)✅ All files cleaned.$(NC)"

# Recompilar todo
re: fclean all

# Compilar con debug
debug: CFLAGS += -g -fsanitize=address
debug: re
	@echo "$(YELLOW)Debug version compiled with AddressSanitizer$(NC)"

# Test básico del sistema modular
test: $(NAME)
	@echo "$(YELLOW)Running modular system test...$(NC)"
	@echo "$(BLUE)Testing file structure:$(NC)"
	@ls -la *.c *.h 2>/dev/null || echo "$(RED)Missing source files$(NC)"
	@echo "$(BLUE)Testing exam-rank detection:$(NC)"
	@ls -la | grep exam-rank || echo "$(YELLOW)No exam-rank directories found$(NC)"
	@echo "$(GREEN)✅ Modular test completed!$(NC)"

# Mostrar estructura del proyecto
structure:
	@echo "$(BLUE)📁 EXAMSHELL - Modular Structure:$(NC)"
	@echo ""
	@echo "$(GREEN)Core Files:$(NC)"
	@echo "  examshell.h     - Main header with all prototypes"
	@echo "  main.c          - Entry point and argument parsing"
	@echo ""
	@echo "$(GREEN)Feature Modules:$(NC)"
	@echo "  ui.c            - User interface and display functions"
	@echo "  persistence.c   - Progress saving/loading system"
	@echo "  exercises.c     - Exercise and rank management"
	@echo "  exam_session.c  - Main exam execution logic"
	@echo ""
	@echo "$(GREEN)Build System:$(NC)"
	@echo "  Makefile        - Modular compilation system"
	@echo ""
	@echo "$(GREEN)Data Files:$(NC)"
	@echo "  .examshell_progress - User progress (auto-generated)"

# Ayuda extendida
help:
	@echo "$(GREEN)EXAMSHELL - 42 School Exam Practice System (Modular)$(NC)"
	@echo ""
	@echo "$(YELLOW)Build Targets:$(NC)"
	@echo "  all       - Compile examshell (default)"
	@echo "  clean     - Remove object files"
	@echo "  fclean    - Remove all generated files + progress"
	@echo "  re        - Recompile everything"
	@echo "  debug     - Compile with debug flags"
	@echo ""
	@echo "$(YELLOW)Installation:$(NC)"
	@echo "  install   - Install examshell globally (requires sudo)"
	@echo "  uninstall - Remove global installation"
	@echo ""
	@echo "$(YELLOW)Testing:$(NC)"
	@echo "  test      - Run basic system tests"
	@echo "  structure - Show project structure"
	@echo ""
	@echo "$(YELLOW)Usage after compilation:$(NC)"
	@echo "  ./examshell [rank_number]"
	@echo "  ./examshell 03"
	@echo "  ./examshell 04"
	@echo "  ./examshell 05"
	@echo ""
	@echo "$(YELLOW)Features:$(NC)"
	@echo "  ✅ Modular architecture"
	@echo "  ✅ Progress persistence"
	@echo "  ✅ Statistics tracking"
	@echo "  ✅ Automatic subject display"
	@echo "  ✅ 1 exercise per level (like real exam)"
	@echo "  ✅ Cross-platform compatibility"

# Evitar conflictos con archivos del mismo nombre
.PHONY: all clean fclean re install uninstall test debug structure help

# Regla por defecto si no se especifica target
.DEFAULT_GOAL := all
