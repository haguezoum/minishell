/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:20:52 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/10 03:40:50 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../tokenizer/token.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# define PROMPT "minishell$ "

typedef struct s_node		t_node;
typedef struct s_cmd		t_cmd;
typedef struct s_relem		t_relem;

typedef struct s_exit
{
	int						exit_status;
}							t_check;

t_check						g_check;

int							ft_strcmp(const char *s1, const char *s2);
char						*ft_strndup(char *s1, int n);

t_global					*skip_whitespace(t_global *current_token,
								int direction);
int							is_operator(enum e_token type);
int							is_valid_word(enum e_token type);
int							is_valid_char(enum e_token type);
int							syntax_error(const char *msg);
int							check_pipe_error(t_global *current_token);
int							check_redirection_error(t_global *current_token);
t_global					*check_unclosed_quotes(t_global **current_token,
								enum e_token type);
int							check_command_syntax(t_lexer *lexer);

typedef struct s_environment
{
	char					*name;
	char					*data;
	struct s_environment	*next;
	struct s_environment	*prev;
	char					**environment_array;
	int						count;
}							t_environment;

t_environment				*allocate_environment_element(char *name,
								char *data);
void						extract_name_and_value(char *line, char **name,
								char **value);
void						add_environment_element(t_environment *env,
								t_environment *elem);
void						process_environment_line(char *line,
								t_environment *env);
t_environment				*create_env_vars(char **environment_array);

char						*store_vars(char *search, t_environment *env);
int							get_env_search_len(char *line, int *i);
char						*get_env_search(char *line, int i,
								int env_search_len);
char						*find_env_value(char *env_search,
								char **environment_array);
void						append_to_result(char *res, int *j, char *value);

char						*expand_dollar(char *line, int *i,
								char **environment_array);
char						*allocate_initial_memory(void);
char						*expand_vars(char *line, char **environment_array);

typedef enum e_type
{
	COMMAND,
	PIPE
}							t_type;

typedef struct s_rlist
{
	struct s_relem			*first;
	struct s_relem			*last;
	int						total;
}							t_rlist;

struct						s_node
{
	t_type					t_node_type;
	union					u_node_content
	{
		struct				s_pipe
		{
			t_node			*left;
			t_node			*right;
		} pipe;
		struct				s_cmd
		{
			char			**args;
			char			**env;
			struct			s_fd_type
			{
				int			input;
				int			output;
			} fd;
			t_rlist			*redirections;
		} command;
	} content;
};

struct						s_relem
{
	char					*argument;
	enum e_token			type;
	struct s_relem			*next;
};

typedef struct s_relem		t_relemt;
typedef struct s_node		t_tree_node;

typedef struct s_tree
{
	t_tree_node				*top;
}							t_tree;

int							check_redir(enum e_token type);

t_tree						*init_tree(t_tree *structure_tree);
t_node						*init_node(char **cmd_args, char **env,
								t_rlist *redirections);
t_rlist						*initialize_redir_list(void);
void						clean_up(t_rlist *redir_list, char **arguments);

void						free_asn_node(t_node *node);
void						free_tree(t_node *node);
void						free_redir_list(t_rlist *list);

int							count_arguments(t_global *token);
t_node						*create_pipe_node(t_node *left_child,
								t_node *right_child);
t_node						*build_command_node(t_global **token,
								t_environment *env);
void						ft_free_split(char **split);
void						expand_all(t_global *_token, t_environment *env);

int							is_env_var(t_global *token);
void						skip_to_word_or_env(t_global **token);
char						*get_env_var_value(char *var_name,
								t_environment *env);
int							is_whitespace_tokene(t_global *token);
void						skip_whitespace_tokene(t_global **token);
t_global					*skip_tokens_until(t_global *token,
								int target_type);
int							count_quoted_arguments(t_global *token,
								int quote_type);
int							count_arguments(t_global *token);
char						**build_arguments_array(int size);
int							parse_arguments(t_global **token,
								t_environment *env, t_rlist *redir_list,
								char **arguments);
t_node						*create_new_node(char **arguments,
								char **environment_array, t_rlist *redir_list);
t_node						*build_command_tree(t_global **token,
								t_environment *env);
void						handle_regular_argument(t_global **token,
								char **arguments, int *i,
								int *ignore_arguments);
void						handle_other_token(t_global **token,
								t_environment *env, t_rlist *redir,
								int *ignore_arguments);
int							parse_command_arguments(t_global **token,
								t_environment *env, t_rlist *redir,
								char **arguments);

t_rlist						*parse_redir(t_global **token, t_environment *env,
								t_rlist *redir_list);

t_node						*build_command_node(t_global **token,
								t_environment *env);
t_node						*create_pipe_node(t_node *left_child,
								t_node *right_child);
t_global					*skip_tokens_until_pipe_line(t_global *token);
t_node						*add_command_to_tree(t_node *left_child,
								t_node *right_child);
int							final_parse(t_tree **tree, t_global *token,
								t_environment *env);

int							is_valid_env_var_char(char c);
int							is_valid_env_var_name(char *str);
int							update_env_var(t_environment *env, char *name,
								char *data);
char						*get_key(char *arg);
char						*get_value(char *arg);
void						free_memory(char *name, char *data, int quote);
int							our_export(char *command, t_environment *env,
								int quote);

int							is_valid_env_var_name(char *str);
t_environment				*find_environment_variable(t_environment *env,
								const char *name);
void						update_existing_env_var(t_environment *var,
								const char *data);
int							handle_export_without_arguments(t_environment *env,
								int exit_status);

int							check_option(char *arg);
void						concatenate_arguments(char **args, int cur_index,
								int args_to_concatenate);
void						process_arguments(t_cmd *command);

int							change_directory(const char *path);
int							our_cd(t_cmd *command);

int							our_pwd(t_cmd *command);
void						our_env(t_cmd *command, t_environment *env);
int							our_unset(t_cmd *cmd, t_environment *env);
void						our_echo(t_cmd *command);
void						our_exit(t_cmd *command);
void						exec_cmd(t_node *ptr, t_environment *evn_vars,
								t_global *tokenList);
int							execute_tree(t_node *ptr, t_environment *evn_vars,
								t_global *token_list);

int							redirout(char *argument);
int							redirin(char *argument);
int							derdir(char *argument);
int							herdoc(char *match, t_environment *env);
void						execute_left_child(t_node *left_node,
								int pipe_fd[2], t_environment *evn_vars,
								t_global *token_list);
void						execute_right_child(t_node *right_node,
								int pipe_fd[2], t_environment *evn_vars,
								t_global *token_list);
int							check_builtin_cmd(t_cmd *ptr);
void						excute_builtin(t_cmd *ptr, t_environment *env,
								t_global *token_list);
void						execute_external_command(t_node *ptr,
								t_environment *evn_vars);
char						*check_cmand_exist_in_dir(t_node *ptr);
void						export(t_environment *env, t_global *token_list);
void						handel_signal(int sig);

char						*check_cmand_exist_in_dir(t_node *ptr);
void						free_double_pointer(char **ptr);
void						*ft_calloc(size_t count, size_t size);
char						*ft_itoa(int n);
char						*ft_strdup(const char *s1);
char						**ft_split(char const *s, char c);
char						*ft_strjoin(char *s1, char *s2);
size_t						ft_strlen(const char *str);
void						ft_bzero(void *string, size_t n);
int							ft_strncmp(const char *s1, const char *s2,
								size_t n);
char						*ft_strchr(const char *str, int c);
void						ft_putstr_fd(char *s, int fd);
char						*ft_substr(char const *s, unsigned int start,
								size_t len);
int							ft_isalnum(int c);
int							ft_isalpha(int ch);
char						*ft_strtrim(char *s1, char const *set);
int							check_equal(char *s);
void						expand_all(t_global *_token, t_environment *env);
char						*ft_strjoin2(char *s1, char *s2);
void						export_names(char **tmp, t_environment *env);
void						print_environment(t_environment *env);
char						*ft_remove_char(char *str, char c);
void						export_utils(t_environment *env, char *arg);
void						print_all_variables(t_environment *env);
int							check_pipe_error(t_global *current_token);
int							check_redirection_error(t_global *current_token);

t_global					*check_unclosed_quotes(t_global **current_token,
								enum e_token type);
int							process_operator(t_global **current_token,
								int *has_operator);
int							process_token(t_global **current_token,
								int *has_operator, int *has_command,
								t_global **prev_word);
int							handle_pipe_line(int *has_operator,
								int *has_command);
int							handle_operator_case(t_global **current_token,
								int *has_operator, t_global **prev_word);
int							is_valid_word(enum e_token type);
int							is_valid_char(enum e_token type);
int							syntax_error(const char *msg);
int							check_pipe_error(t_global *current_token);
int							check_redirection_error(t_global *current_token);
t_global					*skip_whitespace(t_global *current_token,
								int direction);
int							is_operator(enum e_token type);
int							check_command_syntax(t_lexer *lexer);

#endif
