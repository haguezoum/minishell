CFLAGS =   #-fsanitize=address -g # -Wall -Wextra -Werror
RLFLGS    = -L/Users/haguezou/.brew/opt/readline/lib -lreadline
RLOFLGS    = -I/Users/haguezou/.brew/opt/readline/include
CC = cc
RM = rm -rf
SRC = main.c  tokenizer/lexer_utils.c tokenizer/tokenizer.c\
 tokenizer/process_elements.c tokenizer/handle_elements.c\
 check_syntax.c  expand/create_env.c\
  expand/process_env.c expand/final_expand.c just_for_print.c\
    free.c \
  builtins/our_cd.c builtins/our_pwd.c builtins/our_env.c builtins/our_unset.c builtins/our_echo.c \
   builtins/our_exit.c builtins/our_export.c builtins/export_utils.c builtins/cd_utils.c\
  parser/build_cmd.c parser/final_parser.c parser/initialize.c parser/parse_cmd.c\
  parser/parse_utils_1.c  parser/parse_utils_2.c  parser/parse_utils_3.c parser/redir_parse.c \
  exection/derdir_inout.c exection/redir_inout.c exection/execution.c  exection/execution_utils.c exection/builtin_utils.c \
  exection/signales.c  utils/libft_utils_1.c utils/libft_utils_2.c utils/libft_utils_3.c utils/libft_utils_4.c

OBJ = ${SRC:.c=.o}
LIBS = -lreadline

all: minishell

minishell: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(RLFLGS) -o minishell

%.o: %.c
	$(CC) $(CFLAGS) $(RLOFLGS) -c $< -o $@

clean:
	@$(RM) $(OBJ)

fclean: clean
	@$(RM) minishell

re: fclean all

.PHONY: all clean fclean re

