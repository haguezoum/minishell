CFLAGS =

CC = gcc

RM = rm -rf

SRC = lexer.c

OBJ = ${SRC:.c=.o}
all : NAME

NAME:  compile minishell

compile:
	@make -C libft

minishell : $(OBJ) $(compile)
	$(CC) $(CFLAGS) $(OBJ) libft/libft.a -o minishell


clean:
	@make clean -C libft
	@$(RM) $(OBJ)

fclean : clean
	@make fclean -C libft
	@rm -f minishell

re : fclean all
.PHONY:		all clean fclean re
