/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 08:01:03 by aet-tass          #+#    #+#             */
/*   Updated: 2023/08/29 19:25:53 by aet-tass         ###   ########.fr       */
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
		else if (tmp->type == DQUOTE || tmp->type == SQUOTE ||
					tmp->type == REDIR_IN || tmp->type == REDIR_OUT ||
					tmp->type == DREDIR_OUT || tmp->type == HERE_DOC)
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

t_node	*build_command_tree(t_global **token, t_environment *env)
{
	t_node	*new_node;
	t_rlist	*redir_list;
	char	**arguments;
	int		arg_index;

	new_node = NULL;
	redir_list = NULL;
	arguments = NULL;
	arg_index = 0;
	redir_list = initialize_redir_list();
	if (!redir_list)
		return (NULL);
	arg_index = count_arguments(*token);
	arguments = build_arguments_array(arg_index);
	if (!arguments)
	{
		free_redir_list(redir_list);
		return (NULL);
	}
	if (parse_arguments(token, env, redir_list, arguments) == EXIT_FAILURE)
	{
		clean_up(redir_list, arguments);
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
