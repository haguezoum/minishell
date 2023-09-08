/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 08:01:28 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/08 19:48:50 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_rlist	*initialize_redir_list(void)
{
	t_rlist	*redir_list;

	redir_list = ft_calloc(1, sizeof(t_rlist));
	return (redir_list);
}

void	clean_up(t_rlist *redir_list, char **arguments)
{
	free_redir_list(redir_list);
	free(arguments);
}

t_tree	*init_tree(t_tree *structure_tree)
{
	structure_tree = ft_calloc(sizeof(t_tree), 1);
	return (structure_tree);
}

t_node	*init_node(char **cmd_args, char **env, t_rlist *redirections)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->t_node_type = COMMAND;
	node->content.command.args = cmd_args;
	node->content.command.env = env;
	node->content.command.redirections = redirections;
	node->content.command.fd.input = 0;
	node->content.command.fd.output = 1;
	return (node);
}
