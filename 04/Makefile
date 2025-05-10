# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mvigara- <mvigara-@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/09 16:44:49 by mvigara-          #+#    #+#              #
#    Updated: 2025/05/09 16:44:59 by mvigara-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

# Directories
MICROSHELL_DIR = microshell
GUIDE_DIR = microshell
RENDU_DIR = rendu/microshell

# Files
MICROSHELL = $(RENDU_DIR)/microshell
GUIDE_UTILS = $(GUIDE_DIR)/guide_utils.c

# Compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: check_dirs guide

check_dirs:
	@mkdir -p $(RENDU_DIR)

# Targets
guide: $(GUIDE_DIR)/microshell_guide
	@echo "$(GREEN)Microshell guide compiled successfully.$(RESET)"

$(GUIDE_DIR)/microshell_guide: $(GUIDE_DIR)/microshell_guide.c $(GUIDE_UTILS) $(GUIDE_DIR)/guide_utils.h
	@$(CC) $(CFLAGS) -o $@ $< $(GUIDE_UTILS)
	@chmod +x $(GUIDE_DIR)/microshell_guide

test: $(MICROSHELL)
	@echo "$(GREEN)Running tests for microshell...$(RESET)"
	@cd $(MICROSHELL_DIR)/grademe && ./test.sh

$(MICROSHELL):
	@if [ ! -f $(RENDU_DIR)/microshell.c ]; then \
		echo "$(RED)Error: microshell.c not found in $(RENDU_DIR)$(RESET)"; \
		echo "$(RED)Please create your implementation in $(RENDU_DIR)/microshell.c$(RESET)"; \
		exit 1; \
	fi
	@$(CC) $(CFLAGS) -o $@ $(RENDU_DIR)/microshell.c
	@echo "$(GREEN)Microshell compiled successfully.$(RESET)"

clean:
	@rm -f $(GUIDE_DIR)/microshell_guide
	@rm -f $(MICROSHELL)
	@echo "$(GREEN)Cleaned object files and executables.$(RESET)"

re: clean all

.PHONY: all clean re check_dirs guide test
