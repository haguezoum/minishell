CFLAGS =   # -Wall -Wextra -Werror
CC = gcc -g
RM = rm -rf
SRC = main.c libft_utils.c tokenizer/lexer_utils.c tokenizer/tokenizer.c\
 tokenizer/process_elements.c tokenizer/handle_elements.c\
 check_syntax.c  expand/create_env.c\
  expand/process_env.c expand/final_expand.c just_for_print.c\
    free.c    test_exec.c \
 our_cd.c our_pwd.c our_env.c our_unset.c our_echo.c \
   our_exit.c our_export.c\
  parser/build_cmd.c parser/final_parser.c parser/initialize.c parser/parse_cmd.c\
  parser/parse_utils_1.c  parser/parse_utils_2.c  parser/parse_utils_3.c parser/redir_parse.c
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

