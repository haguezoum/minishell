CFLAGS =   #-fsanitize=address
CC = gcc -g
RM = rm -rf
SRC = main.c libft_utils.c lexer_utils.c lexer.c check_syntax.c   env.c  get_next_line.c   just_for_print.c parser.c parse_utils.c free.c redir_parse.c init_tree.c build_cmd.c
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

