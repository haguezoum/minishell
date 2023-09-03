/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 08:02:04 by aet-tass          #+#    #+#             */
/*   Updated: 2023/08/29 19:19:55 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_node	*build_command_node(t_global **token, t_environment *env)
{
	t_node	*command_node;

	command_node = NULL;
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
	pipe_node->node_type = PIPE;
	pipe_node->content.pipe.left = left_child;
	pipe_node->content.pipe.right = right_child;
	if (!pipe_node->content.pipe.left || !pipe_node->content.pipe.right)
	{
		free_asn_node(pipe_node);
		return (NULL);
	}
	return (pipe_node);
}
t_global	*skip_tokens_until_pipe_line(t_global *token)
{
	while (token && token->type != PIPE_LINE)
	{
		token = token->next_token;
	}
	if (token)
	{
		token = token->next_token;
	}
	return (token);
}
t_node	*add_command_to_tree(t_node *left_child, t_node *right_child)
{
	t_node	*pipe_node;

	pipe_node = create_pipe_node(left_child, right_child);
	if (pipe_node)
	{
		return (pipe_node);
	}
	return (NULL);
}

int	final_parse(t_tree **tree, t_global *token, t_environment *env)
{
	t_node	*command_node;
	t_node	*updated_pipe;
	t_node	*pipe_node;

	while (token)
	{
		command_node = build_command_node(&token, env);
		if (!command_node)
			return (EXIT_FAILURE);
		if (!(*tree)->top)
		{
			(*tree)->top = command_node;
		}
		else
		{
			if ((*tree)->top->node_type == PIPE)
			{
				updated_pipe = add_command_to_tree(command_node, (*tree)->top);
				if (updated_pipe)
					(*tree)->top = updated_pipe;
			}
			else
			{
				pipe_node = add_command_to_tree(command_node, (*tree)->top);
				if (pipe_node)
					(*tree)->top = pipe_node;
			}
		}
		token = skip_tokens_until_pipe_line(token);
	}
	return (EXIT_SUCCESS);
}
