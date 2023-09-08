/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 00:18:23 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/09 00:26:01 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

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

int check_pipe_line(t_global *current_token, int *has_operator, int *has_command)
{
    if (*has_operator || !*has_command)
        return syntax_error("unexpected pipe `|'");

    *has_operator = 1;
    return EXIT_SUCCESS;
}

int check_redirection(t_global *current_token, int *has_operator, t_global **prev_word)
{
    if (!*prev_word)
        return syntax_error("unexpected token before redirection");

    t_global *next_token = skip_whitespace(current_token->next_token, 1);
    if (!next_token || !is_valid_word(next_token->type))
        return syntax_error("unexpected token after redirection");

    *has_operator = 1;
    *prev_word = next_token;

    return EXIT_SUCCESS;
}

int check_new_line(t_global *current_token, int *has_operator, int *has_command, t_global **prev_word)
{
    if (!*prev_word && !*has_operator)
        return syntax_error("unexpected token `newline'");

    *has_operator = 0;
    *has_command = 0;

    return EXIT_SUCCESS;
}

int check_quotes_and_escape(t_global **current_token)
{
    t_global *quote = check_unclosed_quotes(current_token, (*current_token)->type);
    if (!quote)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
