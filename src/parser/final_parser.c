/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 08:02:04 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/08 19:48:50 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	update_top_node(t_tree **tree, t_node *command_node)
{
	t_node	*updated_pipe;

	updated_pipe = add_command_to_tree(command_node, (*tree)->top);
	if (updated_pipe)
		(*tree)->top = updated_pipe;
}

void	create_new_pipe_node(t_tree **tree, t_node *command_node)
{
	t_node	*pipe_node;

	pipe_node = add_command_to_tree(command_node, (*tree)->top);
	if (pipe_node)
		(*tree)->top = pipe_node;
}

int	final_parse(t_tree **tree, t_global *token, t_environment *env)
{
	t_node	*command_node;

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
			if ((*tree)->top->t_node_type == PIPE)
				update_top_node(tree, command_node);
			else
				create_new_pipe_node(tree, command_node);
		}
		token = skip_tokens_until_pipe_line(token);
	}
	return (EXIT_SUCCESS);
}
