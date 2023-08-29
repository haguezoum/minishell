CFLAGS =   # -Wall -Wextra -Werror
CC = gcc
RM = rm -rf
SRC = main.c libft_utils.c tokenizer/lexer_utils.c tokenizer/tokenizer.c tokenizer/process_elements.c tokenizer/handle_elements.c\
 check_syntax.c parser.c env.c just_for_print.c  free.c redir_parse.c init_tree.c parse_utils.c \
 our_cd.c our_pwd.c our_env.c our_unset.c our_echo.c build_cmd.c our_exit.c our_export.c \
 exection/derdir_inout.c exection/redir_inout.c exection/execution.c  exection/execution_utils.c exection/builtin_utils.c
OBJ = ${SRC:.c=.o}
LIBS = -lreadline

all: minishell

minishell: $(OBJ)
	@make -C libft
	$(CC) $(CFLAGS) $(OBJ) libft/libft.a $(LIBS) -o minishell

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make clean -C libft
	@$(RM) $(OBJ)

fclean: clean
	@make fclean -C libft
	@$(RM) minishell

re: fclean all

.PHONY: all clean fclean re

