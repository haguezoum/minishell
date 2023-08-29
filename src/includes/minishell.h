# ifndef MINISHELL_H
# define MINISHELL_H
#include <unistd.h>
#include <stdlib.h>
# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include "../tokenizer/token.h"
# include <string.h>
# include <limits.h>
# include <unistd.h>
#include <sys/wait.h>
#include <sys/wait.h>



#define PROMPT "minishell$ "




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


typedef struct s_environment {
    char *name;
    char *data;
    struct s_environment *next;
    struct s_environment *prev;
    char **environment_array;
    int count;
} t_environment;


// create_env.c :


t_environment *allocate_environment_element(char *name, char *data);
void extract_name_and_value(char *line, char **name, char **value);
void add_environment_element(t_environment *env, t_environment *elem);
void process_environment_line(char *line, t_environment *env);
t_environment *create_env_vars(char **environment_array);

// process_env.c :

char *store_vars(char *search, t_environment *env);
int get_env_search_len(char *line, int *i);
char *get_env_search(char *line, int i, int env_search_len);
char *find_env_value(char *env_search, char **environment_array);
void append_to_result(char *res, int *j, char *value);

// final_expand.c :

char *expand_dollar(char *line, int *i, char **environment_array);
char *allocate_initial_memory();
char *expand_vars(char *line, char **environment_array);

/*------------------------------------------------------------------------------------*/


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

int	check_redir(enum e_token type);
// initialize.c :

t_tree *init_tree(t_tree *structure_tree);
t_node *init_node(char **cmd_args, char **env, t_rlist *redirections);
t_rlist *initialize_redir_list();
void clean_up(t_rlist *redir_list, char **arguments);


//free.c

void free_asn_node(t_node *node);
void free_tree(t_node *node);
void free_redir_list(t_rlist *list);

// parse_utils_1.c :

void remove_double_quotes(char *input);
char *expand_env_in_quotes(t_global **token, t_environment *env);
char *join_content(char *argument, char *content, int size);
char *parse_quoted_argument(t_global **token, t_environment *env);

// parse_utils_2.c :

int is_env_var(t_global *token);
void skip_to_word_or_env(t_global **token);
char *get_env_var_value(char *var_name, t_environment *env);
int is_whitespace_tokene(t_global *token);
void skip_whitespace_tokene(t_global **token);

// parse_utils_3.c :

t_global *skip_tokens_until(t_global *token, int target_type);
int count_quoted_arguments(t_global *token, int quote_type);

// build_cmd.c :

int count_arguments(t_global *token);
char **build_arguments_array(int size);
int parse_arguments(t_global **token, t_environment *env, t_rlist *redir_list, char **arguments);
t_node *create_new_node(char **arguments, char **environment_array, t_rlist *redir_list);
t_node *build_command_tree(t_global **token, t_environment *env);



// parse_cmd.c :

void handle_regular_argument(t_global **token, char **arguments, int *i, int *ignore_arguments);
void handle_quoted_argument(t_global **token, t_environment *env, char **arguments, int *i, int *ignore_arguments);
void handle_env_argument(t_global **token, t_environment *env, char **arguments, int *i, int *ignore_arguments);
void handle_other_token(t_global **token, t_environment *env, t_rlist *redir, int *ignore_arguments);
int parse_command_arguments(t_global **token, t_environment *env, t_rlist *redir, char **arguments);


// parse_redir.c :

t_rlist *parse_redir(t_global **token, t_environment *env, t_rlist *redir_list) ;
// final_parse.c :

t_node *build_command_node(t_global **token, t_environment *env);
t_node *create_pipe_node(t_node *left_child, t_node *right_child);
t_global *skip_tokens_until_pipe_line(t_global *token);
t_node *add_command_to_tree(t_node *left_child, t_node *right_child);
int final_parse(t_tree **tree, t_global *token, t_environment *env);






// void print_node(t_node *cmd, int depth);
// void print_tree(t_node *ptr, char **env, int depth);

int our_cd(t_cmd *command, char ***environment);
int our_pwd(t_cmd *command);
int our_env(t_cmd *command, t_environment *env);
int our_unset(t_cmd *cmd, t_environment *env);
void our_echo(t_cmd *command, t_global *tokenList, char **environment);
void exec_cmd(t_node *ptr, t_environment *evn_vars, t_global *tokenList);
int execute_tree(t_node *ptr, t_environment *evn_vars, t_global *token_list);
void our_exit(t_cmd *command);
int our_export(char *command, t_environment *env);
// executipn part

int redirout(char *argument);
int redirin(char *argument);
int derdir(char *argument);
int herdoc(char *match, t_environment *env);
void execute_left_child(t_node *left_node, int pipe_fd[2], t_environment *evn_vars, t_global *token_list);
void execute_right_child(t_node *right_node, int pipe_fd[2], t_environment *evn_vars, t_global *token_list);
int check_builtin_cmd(t_cmd *ptr);
void excute_builtin(t_cmd *ptr, t_environment *env, t_global *token_list);
void execute_external_command(t_node *ptr, t_environment *evn_vars);
char* check_cmand_exist_in_dir(t_node *ptr);

#endif