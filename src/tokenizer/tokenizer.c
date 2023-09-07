/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 22:40:51 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/06 21:40:58 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\r' || c == '\f'
		|| c == '\n');
}

void	handle_special_characters(t_lexer *lexer, char *line, int *i,
		enum e_state *state)
{
	if (ft_strchr("<>|", line[*i]))
	{
		add_token(lexer, new_token(line + *i, 1, line[*i], *state));
		(*i)++;
	}
	else if (is_whitespace(line[*i]))
	{
		add_token(lexer, new_token(line + *i, 1, WHITE_SPACE, *state));
		(*i)++;
	}
}

int	tokenize_lexeme(t_lexer *lexer, char *line, int i, enum e_state *state)
{
	if (line[i] == '\'' || line[i] == '\"')
		process_quotes(lexer, line, &i, state);
	else if (line[i] == '$')
	{
		if (ft_strchr(" \t\v\r\f\n=<>|", line[i + 1]))
		{
			add_token(lexer, new_token(line + i, 1, WORD, *state));
			i++;
		}
		else
			i = process_dollar_sign(lexer, line, i, state);
	}
	else if (ft_strchr("<>|", line[i]) || is_whitespace(line[i]))
		handle_special_characters(lexer, line, &i, state);
	else
		process_word_tokenization(lexer, line, &i, state);
	process_redirection(lexer, line, &i, state);
	if ((*state == IN_DOUBLE_QUOTES && line[i] == '\"')
		|| (*state == IN_SINGLE_QUOTES && line[i] == '\''))
	{
		*state = DEFAULT;
		add_token(lexer, new_token(line + i, 1, line[i], *state));
		i++;
	}
	return (i);
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
