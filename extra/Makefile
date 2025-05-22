# Makefile para Sistema de Práctica por Patrones - 42 School Exam Rank 02
# Creado para optimizar el aprendizaje por conceptos en lugar de niveles

.PHONY: help start menu patterns progress setup clean reset status

# Colores para output
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
WHITE = \033[0;37m
NC = \033[0m # No Color

help:
	@echo "$(CYAN)═══════════════════════════════════════════════════════════════════════════════$(NC)"
	@echo "$(CYAN)█                    SISTEMA DE PRÁCTICA POR PATRONES - 42 SCHOOL             █$(NC)"
	@echo "$(CYAN)═══════════════════════════════════════════════════════════════════════════════$(NC)"
	@echo ""
	@echo "$(GREEN)Comandos disponibles:$(NC)"
	@echo "  $(YELLOW)make start$(NC)     - Inicia el sistema de práctica por patrones"
	@echo "  $(YELLOW)make menu$(NC)      - Muestra el menú principal interactivo"
	@echo "  $(YELLOW)make patterns$(NC)  - Lista todos los patrones disponibles"
	@echo "  $(YELLOW)make progress$(NC)  - Muestra tu progreso actual"
	@echo "  $(YELLOW)make setup$(NC)     - Configura el sistema (crear enlaces simbólicos)"
	@echo "  $(YELLOW)make status$(NC)    - Estado del sistema y estadísticas"
	@echo "  $(YELLOW)make reset$(NC)     - Reinicia todo el progreso (¡CUIDADO!)"
	@echo "  $(YELLOW)make clean$(NC)     - Limpia archivos temporales"
	@echo ""
	@echo "$(BLUE)Los 10 patrones conceptuales organizados para aprendizaje progresivo:$(NC)"
	@echo "  $(PURPLE)01.$(NC) Manipulación básica de caracteres"
	@echo "  $(PURPLE)02.$(NC) Procesamiento de cadenas palabra por palabra"
	@echo "  $(PURPLE)03.$(NC) Funciones de conversión de tipos"
	@echo "  $(PURPLE)04.$(NC) Algoritmos de búsqueda"
	@echo "  $(PURPLE)05.$(NC) Operaciones con conjuntos"
	@echo "  $(PURPLE)06.$(NC) Manipulación de memoria y punteros"
	@echo "  $(PURPLE)07.$(NC) Algoritmos matemáticos"
	@echo "  $(PURPLE)08.$(NC) Manipulación de listas enlazadas"
	@echo "  $(PURPLE)09.$(NC) Algoritmos de ordenación"
	@echo "  $(PURPLE)10.$(NC) Recursividad y algoritmos de inundación"
	@echo ""

start: setup
	@./scripts/main_menu.sh

menu: setup
	@./scripts/main_menu.sh

setup:
	@echo "$(YELLOW)Configurando sistema de patrones...$(NC)"
	@chmod +x scripts/*.sh
	@./scripts/setup_links.sh
	@echo "$(GREEN)✓ Sistema configurado correctamente$(NC)"

patterns:
	@echo "$(CYAN)═══════════════════════════════════════════════════════════════════════════════$(NC)"
	@echo "$(CYAN)█                           PATRONES DISPONIBLES                              █$(NC)"
	@echo "$(CYAN)═══════════════════════════════════════════════════════════════════════════════$(NC)"
	@ls -1 patterns/ | nl -v 1 -n ln | sed 's/^/  /'

progress:
	@./scripts/show_progress.sh

status:
	@./scripts/system_status.sh

reset:
	@echo "$(RED)¿Estás seguro de que quieres reiniciar TODO el progreso? [y/N]$(NC)"
	@read -r response; \
	if [ "$$response" = "y" ] || [ "$$response" = "Y" ]; then \
		rm -f progress/*.txt; \
		echo "$(GREEN)✓ Progreso reiniciado$(NC)"; \
	else \
		echo "$(YELLOW)Operación cancelada$(NC)"; \
	fi

clean:
	@echo "$(YELLOW)Limpiando archivos temporales...$(NC)"
	@find . -name "*.o" -delete
	@find . -name "a.out" -delete
	@find . -name ".DS_Store" -delete
	@echo "$(GREEN)✓ Limpieza completada$(NC)"

# Targets individuales para cada patrón
pattern-01:
	@./scripts/pattern_practice.sh 01-manipulacion-caracteres

pattern-02:
	@./scripts/pattern_practice.sh 02-procesamiento-palabras

pattern-03:
	@./scripts/pattern_practice.sh 03-conversion-tipos

pattern-04:
	@./scripts/pattern_practice.sh 04-algoritmos-busqueda

pattern-05:
	@./scripts/pattern_practice.sh 05-operaciones-conjuntos

pattern-06:
	@./scripts/pattern_practice.sh 06-memoria-punteros

pattern-07:
	@./scripts/pattern_practice.sh 07-algoritmos-matematicos

pattern-08:
	@./scripts/pattern_practice.sh 08-listas-enlazadas

pattern-09:
	@./scripts/pattern_practice.sh 09-algoritmos-ordenacion

pattern-10:
	@./scripts/pattern_practice.sh 10-recursividad-flood-fill
