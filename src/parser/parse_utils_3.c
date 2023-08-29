/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 19:01:51 by aet-tass          #+#    #+#             */
/*   Updated: 2023/08/29 19:33:52 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	include "../includes/minishell.h"

t_global *skip_tokens_until(t_global *token, int target_type)
{
    t_global *tmp = token;
    while (tmp && tmp->type != target_type)
    {
        tmp = tmp->next_token;
    }
    return tmp;
}

int count_quoted_arguments(t_global *token, int quote_type)
{
    int arg_index = 0;
    t_global *tmp = token;
    tmp = tmp->next_token;

    while (tmp && tmp->type != quote_type)
    {
        tmp = tmp->next_token;
        arg_index++;
    }

    return arg_index;
}

