# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mvigara- <mvigara-@student.42school.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/11 18:44:00 by mvigara-          #+#    #+#              #
#    Updated: 2024/11/11 20:26:05 by mvigara-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colores para la salida
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
NC = \033[0m

# Directorios y scripts
MASTER_SCRIPT = exam_master.sh
EXAM_DIR_02 = 02
EXAM_DIR_03 = 03
EXAM_DIR_04 = 04
EXAM_DIR_05 = 05
RENDU_DIR = rendu

# Regla principal
all: check_dir run_master

# Verificar que existen los directorios y los archivos necesarios
check_dir:
	@if [ ! -d "$(EXAM_DIR_02)" ]; then \
		echo "$(RED)Error: No se encuentra el directorio $(EXAM_DIR_02)$(NC)"; \
		exit 1; \
	fi
	@if [ ! -f "$(EXAM_DIR_02)/exam.sh" ]; then \
		echo "$(RED)Error: No se encuentra $(EXAM_DIR_02)/exam.sh$(NC)"; \
		exit 1; \
	fi
	@if [ ! -f "$(MASTER_SCRIPT)" ]; then \
		echo "$(RED)Error: No se encuentra $(MASTER_SCRIPT)$(NC)"; \
		exit 1; \
	fi
	@mkdir -p $(RENDU_DIR)
	@mkdir -p $(EXAM_DIR_02)/exam_progress
	@mkdir -p $(EXAM_DIR_03)/exam_progress
	@mkdir -p $(EXAM_DIR_04)/exam_progress
	@mkdir -p $(EXAM_DIR_05)/exam_progress
	@chmod +x $(EXAM_DIR_02)/exam.sh
	@chmod +x $(EXAM_DIR_02)/init.sh
	@chmod +x $(MASTER_SCRIPT)
	@if [ -f "$(EXAM_DIR_03)/exam.sh" ]; then chmod +x $(EXAM_DIR_03)/exam.sh; fi
	@if [ -f "$(EXAM_DIR_04)/exam.sh" ]; then chmod +x $(EXAM_DIR_04)/exam.sh; fi
	@if [ -f "$(EXAM_DIR_05)/exam.sh" ]; then chmod +x $(EXAM_DIR_05)/exam.sh; fi

# Ejecutar el script maestro
run_master:
	@echo "$(BLUE)Iniciando sistema de exámenes...$(NC)"
	@./$(MASTER_SCRIPT)

# Ejecutar exámenes individuales
run_exam_02:
	@echo "$(BLUE)Iniciando examen 02...$(NC)"
	@cd $(EXAM_DIR_02) && ./exam.sh

run_exam_03:
	@echo "$(BLUE)Iniciando examen 03...$(NC)"
	@if [ -f "$(EXAM_DIR_03)/exam.sh" ]; then \
		cd $(EXAM_DIR_03) && ./exam.sh; \
	else \
		echo "$(RED)Error: No se encuentra exam.sh en $(EXAM_DIR_03)$(NC)"; \
	fi

run_exam_04:
	@echo "$(BLUE)Iniciando examen 04...$(NC)"
	@if [ -f "$(EXAM_DIR_04)/exam.sh" ]; then \
		cd $(EXAM_DIR_04) && ./exam.sh; \
	else \
		echo "$(RED)Error: No se encuentra exam.sh en $(EXAM_DIR_04)$(NC)"; \
	fi

run_exam_05:
	@echo "$(BLUE)Iniciando examen 05...$(NC)"
	@if [ -f "$(EXAM_DIR_05)/exam.sh" ]; then \
		cd $(EXAM_DIR_05) && ./exam.sh; \
	else \
		echo "$(RED)Error: No se encuentra exam.sh en $(EXAM_DIR_05)$(NC)"; \
	fi

# Limpiar archivos temporales y progreso
clean:
	@echo "$(BLUE)Limpiando archivos temporales...$(NC)"
	@rm -rf $(EXAM_DIR_02)/exam_progress/*
	@rm -rf $(EXAM_DIR_03)/exam_progress/*
	@rm -rf $(EXAM_DIR_04)/exam_progress/*
	@rm -rf $(EXAM_DIR_05)/exam_progress/*
	@find . -name "*.o" -type f -delete
	@find . -name "a.out" -type f -delete

# Limpiar todo y resetear el progreso
fclean: clean
	@echo "$(BLUE)Reseteando todo el progreso...$(NC)"
	@rm -rf $(EXAM_DIR_02)/exam_progress/*
	@rm -rf $(EXAM_DIR_03)/exam_progress/*
	@rm -rf $(EXAM_DIR_04)/exam_progress/*
	@rm -rf $(EXAM_DIR_05)/exam_progress/*
	@rm -rf $(RENDU_DIR)/*

# Reinstalar/Resetear todo
re: fclean all

.PHONY: all clean fclean re check_dir run_master run_exam_02 run_exam_03 run_exam_04 run_exam_05