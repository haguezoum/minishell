/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 18:13:50 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/03 23:44:57 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_valid_env_var_name(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (str[0] && ft_isalnum(str[0]));
}

int	update_env_var(t_environment *env, char *name, char *data)
{
	t_environment	*tmp;
	t_environment	*new_var;
	t_environment	*tail;

	tmp = env->next;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0)
		{
			free(tmp->data);
			tmp->data = ft_strdup(data);
			return (1);
		}
		tmp = tmp->next;
	}
	new_var = ft_calloc(1, sizeof(t_environment));
	if (!new_var)
		return (0);
	new_var->name = ft_strdup(name);
	new_var->data = ft_strdup(data);
	tail = env->next;
	if (!tail)
	{
		env->next = new_var;
		new_var->prev = env;
	}
	else
	{
		while (tail->next)
			tail = tail->next;
		tail->next = new_var;
		new_var->prev = tail;
	}
	env->count++;
	return (1);
}

char	*get_key(char *arg)
{
	char	*name;
	char	*equal_sign;

	name = NULL;
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		name = ft_strndup(arg, equal_sign - arg);
	}
	else
	{
		name = ft_strdup(arg);
	}
	return (name);
}

char	*get_value(char *arg)
{
	char	*data;
	char	*equal_sign;

	data = NULL;
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		data = ft_strdup(equal_sign + 1);
	}
	return (data);
}

void	free_memory(char *name, char *data, int quote)
{
	if (quote == 0)
	{
		free(data);
		free(name);
	}
	else if (quote == -1)
	{
		free(name);
	}
}
