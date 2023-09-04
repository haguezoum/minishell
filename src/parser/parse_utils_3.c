/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:01:51 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/02 17:13:19 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_global	*skip_tokens_until(t_global *token, int target_type)
{
	t_global	*tmp;

	tmp = token;
	while (tmp && tmp->type != target_type)
	{
		tmp = tmp->next_token;
	}
	return (tmp);
}

int	count_quoted_arguments(t_global *token, int quote_type)
{
	int			arg_index;
	t_global	*tmp;

	arg_index = 0;
	tmp = token;
	tmp = tmp->next_token;
	while (tmp && tmp->type != quote_type)
	{
		tmp = tmp->next_token;
		arg_index++;
	}
	return (arg_index);
}

int	check_redir(enum e_token type)
{
	return (type == REDIR_IN || type == REDIR_OUT || type == DREDIR_OUT
		|| type == HERE_DOC);
}
