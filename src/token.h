# ifndef TOKEN_H
# define TOKEN_H

#include "minishell.h"

enum e_token {
    WORD,
    WHITE_SPACE = ' ',
    NEW_LINE = '\n',
    QUOTE = '\'',
    DOUBLE_QUOTE = '\"',
    ESCAPE = '\\',
    ENV = '$',
    PIPE_LINE = '|',
    REDIR_IN = '<',
    REDIR_OUT = '>',
    HERE_DOC,
    DREDIR_OUT
};


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

// lexer_utils.c :

t_lexer *init_lexer(t_lexer *lexer);
t_global *new_token(char *content, int size, enum e_token type, enum e_state token_state);
void add_token(t_lexer *lexer, t_global *token);
void free_lexer(t_lexer *lexer);
void print_list(t_lexer *lexer);

// lexer.c :

void handle_single_quotes(t_lexer *lexer, char *line, int i, enum e_state *state);
void handle_double_quotes(t_lexer *lexer, char *line, int i, enum e_state *state);
void tokenize_word(t_lexer *lexer, char *line, int i, enum e_state *state);
int tokenize_env_variable(t_lexer *lexer, char *line, int i, enum e_state *state);
int tokenize_redirection(t_lexer *lexer, char *line, int i, enum e_state *state);
int is_whitespace(char c);
int tokenize_lexeme(t_lexer *lexer, char *line, int i, enum e_state *state);
t_lexer *lexer(char *line);


#endif
