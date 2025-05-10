NAME = mini_serv
CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = ./rendu
EXAMPLES_DIR = ./examples

all: $(NAME)

$(NAME): 
	@if [ -f "$(SRC_DIR)/mini_serv.c" ]; then \
		$(CC) $(CFLAGS) $(SRC_DIR)/mini_serv.c -o $(NAME); \
		echo "Compilation successful!"; \
	else \
		echo "Error: mini_serv.c not found in $(SRC_DIR)"; \
		echo "Please copy your solution to the rendu directory first"; \
	fi

example:
	@echo "Compiling example solution..."
	@$(CC) $(CFLAGS) $(EXAMPLES_DIR)/mini_serv_solution.c -o mini_serv_example
	@echo "Example compiled as mini_serv_example"

template:
	@echo "Compiling template..."
	@$(CC) $(CFLAGS) $(EXAMPLES_DIR)/mini_serv_template.c -o mini_serv_template
	@echo "Template compiled as mini_serv_template"

clean:
	@rm -f $(NAME) mini_serv_example mini_serv_template
	@echo "Clean done!"

setup:
	@bash setup.sh

test: all
	@cd tester && bash test.sh

.PHONY: all clean example template setup test
