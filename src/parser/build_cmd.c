/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 08:01:03 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/07 11:33:42 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**build_arguments_array(int size)
{
	char	**arguments;

	arguments = calloc(sizeof(char *), (size + 1));
	return (arguments);
}

int	parse_arguments(t_global **token, t_environment *env, t_rlist *redir_list,
		char **arguments)
{
	return (parse_command_arguments(token, env, redir_list, arguments));
}

t_node	*create_new_node(char **arguments, char **environment_array,
		t_rlist *redir_list)
{
	return (init_node(arguments, environment_array, redir_list));
}

char	**build_arguments(t_global **token, t_environment *env,
		t_rlist *redir_list)
{
	int		arg_index;
	char	**arguments;

	arg_index = count_arguments(*token);
	arguments = build_arguments_array(arg_index);
	if (!arguments)
		return (NULL);
	if (parse_arguments(token, env, redir_list, arguments) == EXIT_FAILURE)
	{
		free(arguments);
		return (NULL);
	}
	return (arguments);
}

t_node	*build_command_tree(t_global **token, t_environment *env)
{
	t_node	*new_node;
	t_rlist	*redir_list;
	char	**arguments;

	new_node = NULL;
	redir_list = NULL;
	arguments = NULL;
	redir_list = initialize_redir_list();
	if (!redir_list)
		return (NULL);
	arguments = build_arguments(token, env, redir_list);
	if (!arguments)
	{
		free_redir_list(redir_list);
		return (NULL);
	}
	new_node = create_new_node(arguments, env->environment_array, redir_list);
	if (!new_node)
	{
		clean_up(redir_list, arguments);
		return (NULL);
	}
	return (new_node);
}
