/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 17:03:34 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/02 18:40:43 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_environment	*find_env_elem(t_environment *env, char *key)
{
	t_environment	*tmp;

	tmp = env->next;
	while (tmp && strcmp(tmp->name, key) != 0)
		tmp = tmp->next;
	return (tmp);
}

void	delete_env_elem(t_environment *env, t_environment *elem)
{
	if (elem && elem->next)
		elem->next->prev = elem->prev;
	if (elem && elem->prev)
		elem->prev->next = elem->next;
	env->count--;
	free(elem->name);
	free(elem->data);
	free(elem);
}

void	update_env(char *args, t_environment *env_vars)
{
	t_environment	*elem;

	elem = NULL;
	if (args && !(ft_isalpha(args[0]) || args[0] == '_') && strlen(args) > 1)
	{
		printf("minishell: unset: `%s`: not a valid identifier\n", args);
		check.exit_status = EXIT_FAILURE;
	}
	else
	{
		elem = find_env_elem(env_vars, args);
		if (elem)
			delete_env_elem(env_vars, elem);
	}
}

int	our_unset(t_cmd *cmd, t_environment *ev)
{
	int	i;

	i = 1;
	check.exit_status = EXIT_SUCCESS;
	while (cmd->args[i])
	{
		update_env(cmd->args[i], ev);
		i++;
	}
	return (check.exit_status);
}
