/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_elements.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 23:06:57 by aet-tass          #+#    #+#             */
/*   Updated: 2023/08/28 23:27:20 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_single_quotes(t_lexer *lexer, char *line, int i,
		enum e_state *state)
{
	add_token(lexer, new_token(line + i, 1, SQUOTE, *state));
	if (*state == DEFAULT)
		*state = IN_SINGLE_QUOTES;
	else
		*state = DEFAULT;
}

void	handle_double_quotes(t_lexer *lexer, char *line, int i,
		enum e_state *state)
{
	add_token(lexer, new_token(line + i, 1, DQUOTE, *state));
	if (*state == DEFAULT)
		*state = IN_DOUBLE_QUOTES;
	else
		*state = DEFAULT;
}

void	tokenize_word(t_lexer *lexer, char *line, int i, enum e_state *state)
{
	int	j;

	j = i;
	while (line[i] && !ft_strchr("\'\"<>|$ \t\v\r\f\n", line[i]))
		i++;
	add_token(lexer, new_token(line + j, i - j, WORD, *state));
}

int	tokenize_env_variable(t_lexer *lexer, char *line, int i,
		enum e_state *state)
{
	int	k;

	k = i + 1;
	if ((line[k] == '?' || (line[k] >= '0' && line[k] <= '9')))
		k++;
	else
	{
		while (ft_isalnum(line[k]) && line[k] != '\n' && line[k] != '\0')
			k++;
	}
	add_token(lexer, new_token(line + i, k - i, ENV, *state));
	return (k);
}

int	tokenize_redirection(t_lexer *lexer, char *line, int i, enum e_state *state)
{
	int	k;

	k = i + 1;
	if (line[i] == '<')
	{
		if (line[k] == '<')
		{
			add_token(lexer, new_token(line + i, 2, HERE_DOC, *state));
			k++;
		}
		else
			add_token(lexer, new_token(line + i, 1, REDIR_IN, *state));
	}
	else if (line[i] == '>')
	{
		if (line[k] == '>')
		{
			add_token(lexer, new_token(line + i, 2, DREDIR_OUT, *state));
			k++;
		}
		else
			add_token(lexer, new_token(line + i, 1, REDIR_OUT, *state));
	}
	return (k);
}
