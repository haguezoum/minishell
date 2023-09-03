/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 17:02:07 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/03 23:07:30 by aet-tass         ###   ########.fr       */
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

void	our_exit(t_cmd *command)
{
	int			exit_code;
	const char	*str;
	char		*error_msg;

	if (command->args[1])
	{
		str = command->args[1];
		if (*str == '-')
		{
			str++;
		}
		while (*str != '\0')
		{
			if (*str < '0' || *str > '9')
			{
				error_msg = "minishell: exit: numeric argument required\n";
				write(STDERR_FILENO, error_msg, strlen(error_msg));
				check.exit_status = 2;
				exit(check.exit_status);
			}
			str++;
		}
		exit_code = atoi(command->args[1]);
		if (exit_code < 0 || exit_code > 255)
		{
			error_msg = "minishell: exit: numeric argument required\n";
			write(STDERR_FILENO, error_msg, strlen(error_msg));
			check.exit_status = 255;
			exit(check.exit_status);
		}
	}
	else
		exit_code = check.exit_status;
	if (get_nb_args(command->args) > 2)
	{
		error_msg = "minishell: exit: too many arguments\n";
		write(STDERR_FILENO, error_msg, strlen(error_msg));
		check.exit_status = 1;
		exit(check.exit_status);
	}
	check.exit_status = exit_code;
	exit(check.exit_status);
}
