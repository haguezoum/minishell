/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 22:02:42 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/09 22:02:47 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

t_global	*skip_whitespace(t_global *current_token, int direction)
{
	while (current_token && current_token->type == WHITE_SPACE)
	{
		if (direction)
			current_token = current_token->next_token;
		else
			current_token = current_token->prev_token;
	}
	return (current_token);
}

int	is_operator(enum e_token type)
{
	return (type == PIPE_LINE || type == REDIR_IN || type == REDIR_OUT
		|| type == DREDIR_OUT || type == HERE_DOC);
}
