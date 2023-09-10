/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 11:33:50 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/08 19:48:50 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_arguments(t_global *token)
{
	int			arg_index;
	t_global	*tmp;

	arg_index = 0;
	tmp = token;
	while (tmp && tmp->type != PIPE_LINE)
	{
		if (tmp->type == WORD || tmp->type == ENV)
			arg_index++;
		else if (tmp->type == DQUOTE || tmp->type == SQUOTE
			|| tmp->type == REDIR_IN || tmp->type == REDIR_OUT
			|| tmp->type == DREDIR_OUT || tmp->type == HERE_DOC)
		{
			if (tmp->type == DQUOTE || tmp->type == SQUOTE)
				tmp = skip_tokens_until(tmp->next_token, tmp->type);
			else
				tmp = skip_tokens_until(tmp->next_token, WORD);
			arg_index++;
		}
		if (tmp)
			tmp = tmp->next_token;
	}
	return (arg_index);
}

t_node	*build_command_node(t_global **token, t_environment *env)
{
	t_node	*command_node;

	command_node = build_command_tree(token, env);
	return (command_node);
}

t_node	*create_pipe_node(t_node *left_child, t_node *right_child)
{
	t_node	*pipe_node;

	pipe_node = ft_calloc(1, sizeof(t_node));
	if (!pipe_node)
	{
		free_asn_node(left_child);
		free_asn_node(right_child);
		return (NULL);
	}
	pipe_node->t_node_type = PIPE;
	pipe_node->content.pipe.left = left_child;
	pipe_node->content.pipe.right = right_child;
	if (!pipe_node->content.pipe.left || !pipe_node->content.pipe.right)
	{
		free_asn_node(pipe_node);
		return (NULL);
	}
	return (pipe_node);
}
