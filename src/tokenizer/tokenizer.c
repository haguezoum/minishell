/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 22:40:51 by aet-tass          #+#    #+#             */
/*   Updated: 2023/08/28 23:37:10 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\r' || c == '\f'
		|| c == '\n');
}
int	tokenize_regular_word(t_lexer *lexer, char *line, int start, int end,
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
int	tokenize_single_character(t_lexer *lexer, char *line, int index,
		enum e_state *state)
{
	add_token(lexer, new_token(line + index, 1, line[index], *state));
	return (index + 1);
}

int	tokenize_lexeme(t_lexer *lexer, char *line, int i, enum e_state *state)
{
	if (line[i] == '\'')
	{
		return (process_dollar_sign(lexer, line, i, state));
	}
	else if (line[i] == '\"')
	{
		return (process_dollar_sign(lexer, line, i, state));
	}
	else if (line[i] == '$')
	{
		return (process_dollar_sign(lexer, line, i, state));
	}
	else if (ft_strchr("<>|", line[i]))
	{
		return (tokenize_single_character(lexer, line, i, state));
	}
	else if (is_whitespace(line[i]))
	{
		return (tokenize_single_character(lexer, line, i, state));
	}
	else
	{
		return (process_regular_word_or_quote(lexer, line, i, state));
	}
}

t_lexer	*tokenizer(char *line)
{
	t_lexer			*lexer;
	enum e_state	state;
	int				i;

	lexer = init_lexer(lexer);
	if (lexer)
	{
		state = DEFAULT;
		i = 0;
		while (line[i])
		{
			i = tokenize_lexeme(lexer, line, i, &state);
		}
	}
	return (lexer);
}
