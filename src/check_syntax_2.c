/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 20:23:40 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/09 00:18:58 by aet-tass         ###   ########.fr       */
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



static int check_escape(t_global **current_token)
{
    if (!(*current_token)->next_token)
        return syntax_error("unexpected token `escape'");
    *current_token = (*current_token)->next_token;
    return EXIT_SUCCESS;
}

static int check_valid_token(t_global *current_token, int *has_operator, int *has_command, t_global **prev_word)
{
    if (is_valid_word(current_token->type))
    {
        *prev_word = current_token;
        *has_operator = 0;
        *has_command = 1;
    }
    else if (current_token->type == DQUOTE || current_token->type == SQUOTE)
    {
        if (check_quotes_and_escape(&current_token) != EXIT_SUCCESS)
            return EXIT_FAILURE;
    }
    else if (current_token->type == ESCAPE)
    {
        if (check_escape(&current_token) != EXIT_SUCCESS)
            return EXIT_FAILURE;
    }
    else if (!is_valid_char(current_token->type))
    {
        return syntax_error("unexpected token");
    }

    return EXIT_SUCCESS;
}


static int process_token(t_global *current_token, int *has_operator, int *has_command, t_global **prev_word)
{
    if (current_token->type == PIPE_LINE)
    {
        if (*has_operator || !*has_command)
            return syntax_error("unexpected pipe `|'");

        *has_operator = 1;
    }
    else if (current_token->type == NEW_LINE)
    {
        if (!*prev_word && !*has_operator)
            return syntax_error("unexpected token `newline'");

        *has_operator = 0;
        *has_command = 0;
    }
    else
    {
        if (check_valid_token(current_token, has_operator, has_command, prev_word) != EXIT_SUCCESS)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static int check_end_of_command(int has_operator, int has_command)
{
    if (has_operator || !has_command)
        return syntax_error("unexpected end of command");

    return EXIT_SUCCESS;
}

int check_command_syntax(t_lexer *lexer)
{
    t_global *current_token = lexer->head;
    int has_operator = 0;
    int has_command = 0;
    t_global *prev_word = NULL;

    while (current_token)
    {
        if (process_token(current_token, &has_operator, &has_command, &prev_word) != EXIT_SUCCESS)
            return EXIT_FAILURE;

        current_token = current_token->next_token;
    }

    return check_end_of_command(has_operator, has_command);
}
