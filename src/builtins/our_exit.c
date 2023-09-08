/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 17:02:07 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/08 19:22:41 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_nb_args(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		i++;
	}
	return (i);
}

void	print_exit_error(const char *error_msg)
{
	write(STDERR_FILENO, error_msg, strlen(error_msg));
}

int	get_exit_code(t_cmd *command)
{
	const char	*str;
	int			exit_code;

	if (command->args[1])
	{
		str = command->args[1];
		if (*str == '-')
			str++;
		while (*str != '\0')
		{
			if (*str < '0' || *str > '9')
				return (-1);
			str++;
		}
		exit_code = atoi(command->args[1]);
		if (exit_code < 0 || exit_code > 255)
			return (-1);
		return (exit_code);
	}
	else
		return (g_check.exit_status);
}

void	our_exit(t_cmd *command)
{
	int	exit_code;

	exit_code = get_exit_code(command);
	if (exit_code == -1)
	{
		print_exit_error("minishell: exit: numeric argument required\n");
		exit(255);
	}
	g_check.exit_status = exit_code;
	if (get_nb_args(command->args) > 2)
	{
		print_exit_error("minishell: exit: too many arguments\n");
		exit(g_check.exit_status);
	}
	exit(g_check.exit_status);
}
