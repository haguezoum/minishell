/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 04:54:17 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/06 13:15:08 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_environment	*allocate_environment_element(char *name, char *data)
{
	t_environment	*elem;

	elem = malloc(sizeof(t_environment) * 1);
	if (!elem)
	{
		return (NULL);
	}
	elem->name = name;
	elem->data = data;
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}

void	extract_name_and_value(char *line, char **name, char **value)
{
	char	*index;

	index = ft_strchr(line, '=');
	if (index != NULL)
	{
		*name = ft_substr(line, 0, index - line);
		*value = ft_strdup(index + 1);
	}
	else
	{
		*name = ft_strdup(line);
		*value = NULL;
	}
}

void	add_environment_element(t_environment *env, t_environment *elem)
{
	t_environment	*tail;

	tail = env->next;
	if (!tail)
	{
		env->next = elem;
		elem->prev = env;
	}
	else
	{
		while (tail->next)
		{
			tail = tail->next;
		}
		tail->next = elem;
		elem->prev = tail;
	}
	env->count++;
}

void	process_environment_line(char *line, t_environment *env)
{
	char			*name;
	char			*value;
	t_environment	*elem;

	name = NULL;
	value = NULL;
	extract_name_and_value(line, &name, &value);
	if (name)
	{
		elem = allocate_environment_element(name, value);
		if (elem)
		{
			add_environment_element(env, elem);
		}
		else
		{
			free(name);
			free(value);
		}
	}
}

t_environment	*create_env_vars(char **environment_array)
{
	t_environment	*env;
	int				x;
	int				i;

	env = malloc(sizeof(t_environment) * 1);
	if (!env)
	{
		return (NULL);
	}
	x = 0;
	while (environment_array[x])
		x++;
	env->environment_array = malloc(sizeof(char *) * (x + 1));
	env->count = 0;
	check.exit_status = 0;
	i = 0;
	while (environment_array[i])
	{
		process_environment_line(environment_array[i++], env);
	}
	return (env);
}
