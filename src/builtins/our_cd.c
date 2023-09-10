/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 17:04:36 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/10 03:07:10 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_env_variable(t_environment *env_vars, const char *name,
		const char *value)
{
	t_environment	*current_env_elem;

	current_env_elem = env_vars;
	while (current_env_elem && ft_strcmp(current_env_elem->name, name) != 0)
		current_env_elem = current_env_elem->next;
	if (current_env_elem)
	{
		free(current_env_elem->data);
		current_env_elem->data = ft_strdup(value);
	}
}

char	**convert_env_list_to_array(t_environment *env_vars)
{
	char			**env_array;
	int				i;
	t_environment	*current_env_elem;

	env_array = ft_calloc(env_vars->count + 1, sizeof(char *));
	i = 0;
	if (env_array)
	{
		current_env_elem = env_vars;
		while (current_env_elem)
		{
			env_array[i] = ft_strjoin(current_env_elem->name, "=");
			if (current_env_elem->data)
				env_array[i] = ft_strjoin(env_array[i], current_env_elem->data);
			current_env_elem = current_env_elem->next;
			i++;
		}
		env_array[i] = NULL;
	}
	return (env_array);
}

int	handle_change_directory(char *path)
{
	char	current_working_dir[PATH_MAX];

	if (change_directory(path) != 0)
	{
		return (1);
	}
	getcwd(current_working_dir, PATH_MAX);
	return (0);
}

void	handle_update_env_variables(t_environment *env_vars,
		const char *current_working_dir, const char *old_working_dir)
{
	update_env_variable(env_vars, "PWD", current_working_dir);
	update_env_variable(env_vars, "OLDPWD", old_working_dir);
}

int	our_cd(t_cmd *command)
{
	char	*home_dir;

	if (!command->args[1])
	{
		home_dir = getenv("HOME");
		if (home_dir == NULL)
		{
			write(STDERR_FILENO, "minishell: cd: HOME not set\n", 29);
			return (1);
		}
		handle_change_directory(home_dir);
		return (0);
	}
	else
	{
		return (handle_change_directory(command->args[1]));
	}
	g_check.exit_status = 0;
	return (0);
}
