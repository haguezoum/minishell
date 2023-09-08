/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 20:23:40 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/08 20:35:22 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	check_pipe_error(t_global *current_token)
{
	t_global	*prev_token;
	t_global	*next_token;

	prev_token = skip_whitespace(current_token->prev_token, 0);
	next_token = skip_whitespace(current_token->next_token, 1);
	if ((!prev_token || !next_token) || (prev_token->type != WORD
			&& next_token->type != WORD && !is_operator(next_token->type)))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	check_redirection_error(t_global *current_token)
{
	t_global	*next_token;

	next_token = skip_whitespace(current_token->next_token, 1);
	if (!next_token || (next_token->type != WORD && next_token->type != ENV))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

t_global	*check_unclosed_quotes(t_global **current_token, enum e_token type)
{
	while (*current_token)
	{
		*current_token = (*current_token)->next_token;
		if (!*current_token || (*current_token)->type == type)
			break ;
	}
	if (!*current_token)
		write(STDERR_FILENO, "minishell: unclosed quotes detected.\n",
			ft_strlen("minishell: unclosed quotes detected.\n"));
	return (*current_token);
}
/*
int	check_command_syntax(t_lexer *lexer)
{
	t_global	*current_token;
	t_global	*prev_word;
	int			has_operator;
	int			has_command;
	t_global	*next_token;
	t_global	*quote;

	current_token = lexer->head;
	prev_word = NULL;
	has_operator = 0;
	has_command = 0;
	while (current_token)
	{
		if (current_token->type == PIPE_LINE)
		{
			if (has_operator || !has_command)
				return (syntax_error("unexpected pipe `|'"));
			has_operator = 1;
		}
		else if (is_operator(current_token->type))
		{
			if (!prev_word)
				return (syntax_error("unexpected token before redirection"));
			next_token = skip_whitespace(current_token->next_token, 1);
			if (!next_token || !is_valid_word(next_token->type))
				return (syntax_error("unexpected token after redirection"));
			has_operator = 1;
			current_token = next_token;
		}
		else if (current_token->type == NEW_LINE)
		{
			if (!prev_word && !has_operator)
				return (syntax_error("unexpected token `newline'"));
			has_operator = 0;
			has_command = 0;
		}
		else if (current_token->type == DQUOTE || current_token->type == SQUOTE)
		{
			quote = check_unclosed_quotes(&current_token, current_token->type);
			if (!quote)
				return (EXIT_FAILURE);
		}
		else if (current_token->type == ESCAPE)
		{
			if (!current_token->next_token)
				return (syntax_error("unexpected token `escape'"));
			current_token = current_token->next_token;
		}
		else if (!is_valid_char(current_token->type))
		{
			return (syntax_error("unexpected token"));
		}
		if (is_valid_word(current_token->type))
		{
			prev_word = current_token;
			has_operator = 0;
			has_command = 1;
		}
		current_token = current_token->next_token;
	}
	if (has_operator || !has_command)
		return (syntax_error("unexpected end of command"));
	return (EXIT_SUCCESS);
}
*/


