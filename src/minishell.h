# ifndef MINISHELL_H
# define MINISHELL_H
#include <unistd.h>
#include <stdlib.h>
# include "libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include "get_next_line.h"
/*
 * the ANSI escape sequence \x1B[37m is used to set the color to white for "minishell",
 *  \x1B[31m sets the color to red for "_gpt",
 *   \x1B[33m sets the color to yellow for "~>",
 *    and \x1B[0m resets the color to the default.
 */

#define PROMPT "\x1B[37mminishell\x1B[31m_gpt\x1B[0m\x1B[33m~>\x1B[0m "
#define MAX_ARGS 100
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

// Define a new struct to store exit status
typedef struct s_exit {
    int exit_status;
} t_check;

t_check check;

char	*ft_strndup(char *s1, int n);
int ft_strcmp(const char *s1, const char *s2);
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

/**************************
 * Functions of the lexer 
 **************************/


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



/************************************
 * Functions of the Syntax 
 ************************************/



// check_syntax.c

t_global *skip_whitespace(t_global *current_token, int direction);
int is_operator(enum e_token type);
int is_valid_word(enum e_token type);
int is_valid_char(enum e_token type);
int syntax_error(const char *msg);
int check_pipe_error(t_global *current_token);
int check_redirection_error(t_global *current_token);
t_global *check_unclosed_quotes(t_global **current_token, enum e_token type);
int check_command_syntax(t_lexer *lexer);




typedef struct s_environment {
    char *name;
    char *data;
    struct s_environment *next;
    struct s_environment *prev;
    char **environment_array;
    int count;
} t_environment;

t_environment *create_env_vars(char **tab);
char *store_vars(char *key, t_environment *env);
char *expand_vars(char *line, char **env);




/********************************-----------------------------------------***************/



// Forward declarations
typedef struct s_node t_node;
typedef struct s_cmd t_cmd;
typedef struct s_relem t_relem;
//typedef struct s_list t_list;

typedef struct s_rlist {
    struct s_relem *first;
    struct s_relem *last;
    int total;
} t_rlist;
// Define the struct for the AST node
struct s_node {
    enum n_type {
        COMMAND,
        PIPE,
    } node_type;
    union u_node_content {
        struct s_pipe {
            t_node *left;
            t_node *right;
        } pipe;
        struct s_cmd {
            char **args;
            char **env;
            struct fd_type {
                int input;
                int output;
            } fd;
	 t_rlist *redirections; 
        } command;
    } content;
};

// Struct to store a single redirection element
struct s_relem {
    char *argument;
    enum e_token type;
    struct s_relem *next;
};
typedef struct s_relem t_relemt;
typedef struct s_node t_tree_node;

typedef struct s_tree {
    t_tree_node *top;
} t_tree;





t_tree *init_tree(t_tree *structure);
t_node *init_node(char **cmd_args, char **env, t_rlist *redirections);
int parse_command_arguments(t_global **token, t_environment *env, t_rlist *redir, char **arguments);
t_node *build_command_tree(t_global **token, t_environment *env);
void free_asn_node(t_node *node); 
void free_tree(t_node *node);
void free_redir_list(t_rlist *list);
t_rlist *parse_redir(t_global **token, t_environment *env, t_rlist *redir_list) ;
void print_node(t_node *cmd, int depth);
void print_tree(t_node *ptr, char **env, int depth);
int final_parse(t_tree **tree, t_global *token, t_environment *env);



#endif
