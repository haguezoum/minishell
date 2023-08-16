# ifndef MINISHELL_H
# define MINISHELL_H
#include <unistd.h>
#include <stdlib.h>
# include "libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include "get_next_line.h"
# include "token.h"
# include <string.h>
# include <unistd.h>
#include <sys/wait.h>
#include <sys/wait.h>

#define PROMPT "\x1B[37mminishell\x1B[31m_gpt\x1B[0m\x1B[33m~>\x1B[0m "




typedef struct s_node t_node;
typedef struct s_cmd t_cmd;
typedef struct s_relem t_relem;

// Define a new struct to store exit status
typedef struct s_exit {
    int exit_status;
} t_check;

t_check check;




int ft_strcmp(const char *s1, const char *s2);
char	*ft_strndup(char *s1, int n);
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


/*
typedef struct s_environment {
    char *name;
    char *data;
    struct s_environment *next;
    struct s_environment *prev;
    char **environment_array;
    int count;
} t_environment;
*/

// t_environment: Represents the environment linked list and its array representation
typedef struct s_environment {
    char *name;                       // Pointer to the name of the environment variable
    char *data;                       // Pointer to the data (value) of the environment variable
    struct s_environment *next;       // Pointer to the next environment variable in the list
    struct s_environment *prev;       // Pointer to the previous environment variable in the list
    char **environment_array;         // Array representation of the environment variables
    int count;                        // Count of the total number of environment variables
} t_environment;

/*
    Explanation of the struct members:
    - name: Represents the name of the environment variable. It is a pointer to a character array.
    - data: Represents the data (value) of the environment variable. It is a pointer to a character array.
    - next: Represents the pointer to the next environment variable in the linked list.
    - prev: Represents the pointer to the previous environment variable in the linked list.
    - environment_array: Represents the array representation of the environment variables.
    - count: Represents the total count of environment variables stored in the list.
*/

/*
    This struct is designed to store environment variables in a linked list.
    The linked list allows for easy addition, removal, and manipulation of environment variables.
    The environment_array member is used to provide a convenient array representation of the environment variables, which can be useful in certain situations where an array is more suitable than a linked list.
    The count member keeps track of the total number of environment variables stored in the list.
*/

t_environment *create_env_vars(char **tab);
char *store_vars(char *key, t_environment *env);
char *expand_vars(char *line, char **env);



/*------------------------------------------------------------------------------------*/

// Define a struct for a linked list of redirection elements
typedef struct s_rlist {
    struct s_relem *first; // Pointer to the first element in the list
    struct s_relem *last;  // Pointer to the last element in the list
    int total;             // Total number of elements in the list
} t_rlist;

// Define the struct for the AST node
struct s_node {
    enum n_type {           // Enum representing node types
        COMMAND,            // Command node - represents a command in the shell
        PIPE,               // Pipe node - represents a pipe between commands
    } node_type;
    union u_node_content {
        struct s_pipe {     // Struct for a pipe node
            t_node *left; // Pointer to the left node of the pipe
            t_node *right; // Pointer to the right node of the pipe
        } pipe;
        struct s_cmd {      // Struct for a command node
            char **args;     // Array of command arguments
            char **env;      // Array of environment variables
            struct fd_type { // Struct for file descriptors
                int input;    // Input file descriptor
                int output;   // Output file descriptor
            } fd;
            t_rlist *redirections; // Pointer to a list of redirections
        } command;
    } content;
};

// Define a struct for a single redirection element
struct s_relem {
    char *argument;     // Argument of the redirection - represents the file or target
    enum e_token type;  // Type of the redirection - represents the type of redirection
    struct s_relem *next; // Pointer to the next redirection element in the list
};

typedef struct s_relem t_relemt;
typedef struct s_node t_tree_node;

// Typedef for the AST struct
typedef struct s_tree {
    t_tree_node *top;   // Pointer to the top node of the AST
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
int our_cd(t_cmd *command, char ***environment);
int our_pwd(t_cmd *command);
int our_env(t_cmd *command, char **environment);
int our_unset(t_cmd *cmd, t_environment **env);
// void execute(t_node *ptr, char **evn_vars);
void excution(t_node *ptr, t_environment *evn_vars, t_global *tokenList);
void our_echo(t_cmd *command, t_global *tokenList, char **environment);
t_node *build_command_tree(t_global **token, t_environment *env);



#endif
