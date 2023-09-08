/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_elements.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 23:04:22 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/08 16:03:10 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_quotes(t_lexer *lexer, char *line, int *i, enum e_state *state)
{
	if (line[*i] == '\'')
	{
		handle_single_quotes(lexer, line, *i, state);
	}
	else if (line[*i] == '\"')
	{
		handle_double_quotes(lexer, line, *i, state);
	}
	(*i)++;
}

int	process_dollar_sign(t_lexer *lexer, char *line, int i, enum e_state *state)
{
	enum e_state	dollar_state;
	int				end;

	dollar_state = *state;
	if (ft_strchr(" \t\v\r\f\n=<>|", line[i + 1]))
	{
		add_token(lexer, new_token(line + i, 1, WORD, dollar_state));
		return (i + 1);
	}
	else
	{
		end = tokenize_env_variable(lexer, line, i, state);
		*state = dollar_state;
		return (end);
	}
}

void	process_word_tokenization(t_lexer *lexer, char *line, int *i,
		enum e_state *state)
{
	int	j;

	j = *i;
	while (line[*i] && !ft_strchr(" \t\v\r\f\n\'\"<>|$", line[*i]))
	{
		if (*state == DEFAULT && (line[*i] == '\'' || line[*i] == '\"'))
		{
			if (line[*i] == '\"')
				*state = IN_DOUBLE_QUOTES;
			else if (line[*i] == '\'')
				*state = IN_SINGLE_QUOTES;
			add_token(lexer, new_token(line + j, *i - j, WORD, *state));
			j = *i + 1;
		}
		(*i)++;
	}
	if (*state == DEFAULT)
		add_token(lexer, new_token(line + j, *i - j, WORD, DEFAULT));
	else
		add_token(lexer, new_token(line + j, *i - j, WORD, *state));
}

void	process_redirection(t_lexer *lexer, char *line, int *i,
		enum e_state *state)
{
	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		add_token(lexer, new_token(line + *i, 2, HERE_DOC, *state));
		(*i) += 2;
	}
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		add_token(lexer, new_token(line + *i, 2, DREDIR_OUT, *state));
		(*i) += 2;
	}
}
