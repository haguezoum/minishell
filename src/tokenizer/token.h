/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:41:10 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/07 17:48:51 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "../includes/minishell.h"

enum				e_token
{
	WORD,
	PIPE_LINE = '|',
	ENV = '$',
	SQUOTE = '\'',
	DQUOTE = '\"',
	REDIR_IN = '<',
	REDIR_OUT = '>',
	HERE_DOC,
	ESCAPE = '\\',
	NEW_LINE = '\n',
	WHITE_SPACE = ' ',
	DREDIR_OUT
};

enum				e_state
{
	IN_DOUBLE_QUOTES,
	IN_SINGLE_QUOTES,
	DEFAULT
};

typedef struct s_global
{
	char			*content;
	int				size;
	enum e_token	type;
	enum e_state	token_state;
	struct s_global	*next_token;
	struct s_global	*prev_token;
}					t_global;

typedef struct s_lexer
{
	t_global		*head;
	t_global		*last;
	int				count;
}					t_lexer;

// lexer_utils.c :

t_lexer				*init_lexer(t_lexer *lexer);
t_global			*new_token(char *content, int size, enum e_token type,
						enum e_state token_state);
void				add_token(t_lexer *lexer, t_global *token);
void				free_lexer(t_lexer *lexer);
void				print_list(t_lexer *lexer);

// handle_elements.c :

void				handle_single_quotes(t_lexer *lexer, char *line, int i,
						enum e_state *state);
void				handle_double_quotes(t_lexer *lexer, char *line, int i,
						enum e_state *state);
void				tokenize_word(t_lexer *lexer, char *line, int i,
						enum e_state *state);
int					tokenize_env_variable(t_lexer *lexer, char *line, int i,
						enum e_state *state);
int					tokenize_redirection(t_lexer *lexer, char *line, int i,
						enum e_state *state);

// process_elements.c :

void				process_quotes(t_lexer *lexer, char *line, int *i,
						enum e_state *state);

void	process_word(t_lexer *lexer, char *line, int start, int end,
		enum e_state *state);
int					process_dollar_sign(t_lexer *lexer, char *line, int i,
						enum e_state *state);
void				process_word_tokenization(t_lexer *lexer, char *line,
						int *i, enum e_state *state);
void				process_redirection(t_lexer *lexer, char *line, int *i,
						enum e_state *state);

// tokenizer.c :

int					is_whitespace(char c);
void				handle_special_characters(t_lexer *lexer, char *line,
						int *i, enum e_state *state);
int					tokenize_lexeme(t_lexer *lexer, char *line, int i,
						enum e_state *state);
t_lexer				*tokenizer(char *line);

#endif
