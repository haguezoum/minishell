/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 20:17:33 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/08 20:19:20 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	free_asn_node(t_node *node)
{
	char	**args;
	char	**env;
	int		i;

	if (node)
	{
		if (node->t_node_type == COMMAND)
		{
			args = node->content.command.args;
			env = node->content.command.env;
			i = 0;
			while (args && args[i])
			{
				free(args[i]);
				i++;
			}
			free(args);
			free(env);
			free_redir_list(node->content.command.redirections);
		}
		free(node);
	}
}

void	free_tree(t_node *node)
{
	int	i;

	if (!node)
		return ;
	if (node->t_node_type == COMMAND)
	{
		i = 0;
		while (node->content.command.args[i])
		{
			free(node->content.command.args[i]);
			i++;
		}
		free(node->content.command.args);
		free_redir_list(node->content.command.redirections);
	}
	else if (node->t_node_type == PIPE)
	{
		free_tree(node->content.pipe.right);
		free_tree(node->content.pipe.left);
	}
	free(node);
}

void	free_redir_list(t_rlist *list)
{
	t_relem	*tmp;
	t_relem	*next;

	tmp = list->first;
	while (tmp)
	{
		next = tmp->next;
		if (tmp->argument)
			free(tmp->argument);
		free(tmp);
		tmp = tmp->next;
	}
	free(list);
}

void	free_double_pointer(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}
