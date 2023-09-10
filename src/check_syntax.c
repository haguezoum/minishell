/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 03:32:24 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/10 03:49:08 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

int	handle_new_line(int *has_operator, int *has_command, t_global **prev_word)
{
	if (!*prev_word && !*has_operator)
		return (syntax_error("unexpected token `newline'"));
	*has_operator = 0;
	*has_command = 0;
	return (EXIT_SUCCESS);
}

int	handle_other_cases(t_global **current_token)
{
	t_global	*quote;

	if ((*current_token)->type == DQUOTE || (*current_token)->type == SQUOTE)
	{
		quote = check_unclosed_quotes(current_token, (*current_token)->type);
		if (!quote)
			return (EXIT_FAILURE);
	}
	else if ((*current_token)->type == ESCAPE)
	{
		if (!(*current_token)->next_token)
			return (syntax_error("unexpected token `escape'"));
		*current_token = (*current_token)->next_token;
	}
	else if (!is_valid_char((*current_token)->type))
		return (syntax_error("unexpected token"));
	return (EXIT_SUCCESS);
}

int	handle_token_case(t_global **current_token, int *has_operator,
		int *has_command, t_global **prev_word)
{
	int	result;

	if ((*current_token)->type == PIPE_LINE)
		result = handle_pipe_line(has_operator, has_command);
	else if (is_operator((*current_token)->type))
		result = handle_operator_case(current_token, has_operator, prev_word);
	else if ((*current_token)->type == NEW_LINE)
		result = handle_new_line(has_operator, has_command, prev_word);
	else
		result = handle_other_cases(current_token);
	if (result != EXIT_SUCCESS)
		return (result);
	return (EXIT_SUCCESS);
}

int	parse_token(t_global **current_token, int *has_operator, int *has_command,
		t_global **prev_word)
{
	int	result;

	result = handle_token_case(current_token, has_operator, has_command, \
			prev_word);
	if (result != EXIT_SUCCESS)
		return (result);
	result = process_token(current_token, has_operator, has_command, prev_word);
	if (result != EXIT_SUCCESS)
		return (result);
	return (EXIT_SUCCESS);
}

int	check_command_syntax(t_lexer *lexer)
{
	t_global	*current_token;
	t_global	*prev_word;
	int			has_operator;
	int			has_command;
	int			result;

	current_token = lexer->head;
	prev_word = NULL;
	has_operator = 0;
	has_command = 0;
	while (current_token)
	{
		result = parse_token(&current_token, &has_operator, &has_command, \
				&prev_word);
		if (result != EXIT_SUCCESS)
			return (result);
	}
	if (has_operator || !has_command)
		return (syntax_error("unexpected end of command"));
	return (EXIT_SUCCESS);
}
