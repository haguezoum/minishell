# ifndef MINISHELL_H
# define MINISHELL_H
#include <unistd.h>
#include <stdlib.h>
# include "libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include "token.h"


/*
 * the ANSI escape sequence \x1B[37m is used to set the color to white for "minishell",
 *  \x1B[31m sets the color to red for "_gpt",
 *   \x1B[33m sets the color to yellow for "~>",
 *    and \x1B[0m resets the color to the default.
 */

#define PROMPT "\x1B[37mminishell\x1B[31m_gpt\x1B[0m\x1B[33m~>\x1B[0m "

int ft_strcmp(const char *s1, const char *s2);
/*
enum e_token {
    WORD = -1,
    WHITE_SPACE = ' ',
    NEW_LINE = '\n',
    QUOTE = '\'',
    DOUBLE_QUOTE = '\"',
    ESCAPE = '\\',
    ENV = '$',
    PIPE = '|',
    REDIR_IN = '<',
    REDIR_OUT = '>',
    HERE_DOC,
    DREDIR_OUT
};

*/
enum e_state {
    IN_DOUBLE_QUOTES,
    IN_SINGLE_QUOTES,
    DEFAULT
};

typedef struct s_global {
    char *content;
    int size;
    enum e_token type;
    enum e_state token_state;
    struct s_global *next_token;
    struct s_global *prev_token;
} t_global;

typedef struct s_lexer {
    t_global *head;
    t_global *last;
    int count;
} t_lexer;

void free_lexer(t_lexer *lexer);
void add_token(t_lexer *lexer, t_global *token);
t_global *new_token(char *content, int size, enum e_token type, enum e_state token_state);
t_lexer *init_lexer(t_lexer *lexer);
t_lexer *lexer(char *line);
void print_list(t_lexer *lexer);
void print_list(t_lexer *lexer);


#endif
