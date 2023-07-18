CFLAGS = 
CC = gcc
RM = rm -rf
SRC = main.c libft_utils.c lexer_utils.c lexer.c
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

