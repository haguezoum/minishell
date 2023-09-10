/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 23:20:02 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/10 03:44:17 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_all_variables(t_environment *env)
{
	t_environment	*tmp;

	tmp = env->next;
	while (tmp)
	{
		if (tmp->data)
		{
			printf("%s=%s\n", tmp->name, tmp->data);
		}
		else
		{
			printf("%s", tmp->name);
		}
		tmp = tmp->next;
	}
}

void	print_export_error(const char *arg)
{
	char	*error_msg;
	char	*not_valid_msg;

	error_msg = "minishell: export: ";
	not_valid_msg = ": not a valid identifier\n";
	write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, not_valid_msg, ft_strlen(not_valid_msg));
}

void	handle_invalid_env_var(char *arg, int *exit_status, char *data)
{
	print_export_error(arg);
	*exit_status = EXIT_FAILURE;
	if (data)
		free(data);
}

void	handle_valid_env_var(char *name, char *data, t_environment *env,
		int *exit_status)
{
	if (data)
	{
		if (!update_env_var(env, name, data))
		{
			perror("minishell: export");
			*exit_status = EXIT_FAILURE;
			return ;
		}
	}
	else
	{
		data = ft_strdup("");
		if (!update_env_var(env, name, data))
		{
			perror("minishell: export");
			*exit_status = EXIT_FAILURE;
		}
		free(data);
	}
}

int	our_export(char *command, t_environment *env, int quote)
{
	int		exit_status;
	char	*arg;
	char	*name;
	char	*data;

	exit_status = EXIT_SUCCESS;
	arg = command;
	name = get_key(arg);
	data = get_value(arg);
	if (!command)
		return (handle_export_without_arguments(env, exit_status));
	if (name && !is_valid_env_var_name(name))
	{
		free(name);
		handle_invalid_env_var(arg, &exit_status, data);
		return (exit_status);
	}
	else
	{
		handle_valid_env_var(name, data, env, &exit_status);
		free_memory(name, data, quote);
	}
	free_memory(name, data, 0);
	return (exit_status);
}
