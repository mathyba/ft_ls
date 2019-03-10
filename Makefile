# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: emuckens <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/20 17:52:27 by emuckens          #+#    #+#              #
#    Updated: 2018/12/02 18:41:06 by emuckens         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME := ft_ls

#==============================================================================#
#					 		 	  COMPILATION								   
#==============================================================================#

FLAGS		:= -Wall -Werror -Wextra -g
FSANI		:= -fsanitize=address
CC			:= gcc $(FLAGS)
CCF			:= $(CC) $(FSANI)

#==============================================================================#
#								DIRECTORIES									    
#==============================================================================#

BIN_DIR 	:= bin
HEAD_DIR 	:= includes
SRC_DIR 	:= srcs
EXE_DIR		:= programs
LIB_DIR 	:= libft
LISTDIR_DIR := $(LIB_DIR)/$(PROGRAMS)

#==============================================================================#
#								    FILES									   
#==============================================================================#

_SRC		= main.c options.c dir.c display.c color.c file_supportft.c \
			myenv.c env_variables.c tree.c file_getinfo.c padding.c list.c \
			sort.c column.c scan_utils.c
_HEAD		= ft_ls.h
_LISTDIR 	= ft_list_dir

SRC 		= $(patsubst %, $(SRC_DIR)/%, $(_SRC))
HEAD		= $(patsubst %, $(HEAD_DIR)/$, $(_HEAD))
BIN			= $(patsubst %.c, $(BIN_DIR)/%.o, $(_SRC))
LIBFT		:= ../../libft/libft.a
LISTDIR		:= $(LIST_DIR_DIR)/$(_LISTDIR)

#==============================================================================#
#							       MISC										    
#==============================================================================#

NB = $(words $(SRC))
INDEX := 0

#==============================================================================#
#							       RULES									    
#==============================================================================#


.PHONY: all clean fclean re debug

all:
	@make -j5 $(NAME)

$(NAME): $(BIN) $(LIBFT)
	@$(CC) -o $(NAME) $(BIN) -lft -L $(LIB_DIR)
	@echo "\r>>> \033[01;32m$(NAME) READY              \033[0m"

$(LIBFT):
	@make -s -C $(LIB_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(HEAD) Makefile | $(BIN_DIR)
	@$(eval PERCENT=$(shell echo $$(($(INDEX)*100/($(NB))))))
	@$(CC) -I $(HEAD_DIR) -I $(LIB_DIR)/$(HEAD_DIR) -o $@ -c $<
	@printf "\033[22;35mgenerating binary.... [ %d%% ]\033[0m\r" $(PERCENT) 
	@$(eval INDEX=$(shell echo $$(($(INDEX)+1))))

clean:
	@echo "cleaning $(NAME) binaries..."
	@rm -f $(BIN)
	@rm -rf $(BIN_DIR)
	@make clean -C $(LIB_DIR)
	@echo "\033[01;34mBinaries removed from all folders.\n\033[0m"

fclean: clean
	@echo "cleaning $(NAME) executable..."
	@rm -f $(NAME)
	@echo "cleaning $(LIBFT) executable..."
	@rm -f $(LIBFT)
	@echo "\033[01;34mAll directories now clean.\n\033[0m"

re:
	@make -s fclean
	@make -s all

debug: $(BIN) $(LIBFT)
	@echo "\033[01;31mbuilding $(NAME) in DEBUG MODE...\033[0m"
	@$(CCF) -o $(NAME) $(BIN) -lft -L $(LIB_DIR)
	@echo "\033[01;32m$(NAME) READY\033[0m"
