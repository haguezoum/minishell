/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_elements.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 23:04:22 by aet-tass          #+#    #+#             */
/*   Updated: 2023/08/28 23:28:45 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	process_regular_word(t_lexer *lexer, char *line, int start, int end,
		enum e_state *state)
{
	if (*state != DEFAULT)
	{
		add_token(lexer, new_token(line + start, end - start, WORD, *state));
		*state = DEFAULT;
	}
	add_token(lexer, new_token(line + start, end - start, WORD, *state));
	return (end);
}

int	process_single_quote(t_lexer *lexer, char *line, int i, enum e_state *state)
{
	handle_single_quotes(lexer, line, i, state);
	return (i + 1);
}

int	process_double_quote(t_lexer *lexer, char *line, int i, enum e_state *state)
{
	handle_double_quotes(lexer, line, i, state);
	return (i + 1);
}

int	process_dollar_sign(t_lexer *lexer, char *line, int i, enum e_state *state)
{
	if (ft_strchr(" \t\v\r\f\n=<>|", line[i + 1]))
	{
		add_token(lexer, new_token(line + i, 1, WORD, *state));
		return (i + 1);
	}
	else
	{
		return (tokenize_env_variable(lexer, line, i, state));
	}
}

int	process_regular_word_or_quote(t_lexer *lexer, char *line, int i,
		enum e_state *state)
{
	int	start;

	start = i;
	while (line[i] && !ft_strchr(" \t\v\r\f\n\'\"<>|$", line[i]))
	{
		if ((*state == DEFAULT) && (line[i] == '\'' || line[i] == '\"'))
		{
			if (line[i] == '\"')
			{
				*state = IN_DOUBLE_QUOTES;
			}
			else if (line[i] == '\'')
			{
				*state = IN_SINGLE_QUOTES;
			}
			return (tokenize_regular_word(lexer, line, start, i, state));
		}
		i++;
	}
	return (process_regular_word(lexer, line, start, i, state));
}
