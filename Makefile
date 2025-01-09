CC			= cc
CFLAGS		= -Wall -Wextra -Werror -Iinclude -g
MAKEFLAGS	+= --no-print-directory -s

RM			= rm -rf
AR			= ar src

SRC			= src
OBJ			= obj
SRCS		= $(shell find $(SRC) -name '*.c')
OBJS		= $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
BIN_DIR		= bin
BIN			= $(BIN_DIR)/minishell

LIBFT_DIR	= lib/libft
LIBFT		= $(LIBFT_DIR)/libft.a

GREEN  		= \033[0;32m
YELLOW 		= \033[1;33m
BLUE   		= \033[1;34m
RED    		= \033[0;31m
RESET  		= \033[0m

all:		$(BIN)
			printf "☑️ $(GREEN)Compiled in$(RESET): $(BIN)\n"

$(OBJ)/%.o:	$(SRC)/%.c
			@mkdir -p $(OBJ)
			@mkdir -p $(dir $@)
			@mkdir -p $(BIN_DIR)
			$(CC) $(CFLAGS) -c -o $@ $<

$(BIN):		$(OBJS)
			printf "⚙️  $(BLUE)Building LIB_FT$(RESET)\n"
			make all -C $(LIBFT_DIR)
			printf "⚙️  $(BLUE)Building MINISHELL$(RESET)\n"
			$(CC) $(CFLAGS) -lreadline -o $@ $^ $(LIBFT)

clean:
			$(RM) $(OBJ)
			printf "🧹 $(RED)Cleaning$(RESET): $(OBJ)\n"

fclean:		clean
			printf "🧹 $(RED)Cleaning$(RESET): $(BIN_DIR) vgcore.* a.out\n"
			$(RM) $(BIN_DIR)
			$(RM) vgcore.* a.out
			make fclean -C $(LIBFT_DIR)
			printf "🧹 $(RED)Cleaning$(RESET): build/\n"
			cd build/ && make clean && cd ..

re:			fclean all

debug:		all
			cp $(BIN) a.out

tests:		all
			cd build/ && cmake .. && make && cd ..
			./build/test_minishell

checkleaks:	all
			valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=external.supp ./$(BIN)

gensupp:	all
			valgrind --leak-check=full --show-leak-kinds=all --gen-suppressions=all -s ./$(BIN) 2>> new.supp

norm:
			printf "$(RED)"
			norminette $(SRCS) lib include | grep "Error"
			printf "$(RESET)"

run:		all debug
			printf "🚀 $(YELLOW)Running$(RESET): $(BIN)\n"
			./$(BIN)

.SILENT:	all clean fclean re run
.PHONY:		all clean fclean re run
