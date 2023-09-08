/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:10:19 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/08 18:35:38 by aet-tass         ###   ########.fr       */
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

t_environment	*find_environment_variable(t_environment *env, const char *name)
{
	t_environment	*tmp;

	tmp = env->next;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0)
		{
			return (tmp);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	update_existing_env_var(t_environment *var, const char *data)
{
	free(var->data);
	var->data = ft_strdup(data);
}

int	handle_export_without_arguments(t_environment *env, int exit_status)
{
	print_all_variables(env);
	return (exit_status);
}
