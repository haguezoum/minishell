/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 17:02:07 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/07 19:13:09 by aet-tass         ###   ########.fr       */
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

void print_exit_error(const char *error_msg)
{
    write(STDERR_FILENO, error_msg, strlen(error_msg));
}

#include <stdlib.h> // for exit

int get_exit_code(t_cmd *command)
{
    if (command->args[1])
    {
        const char *str = command->args[1];
        if (*str == '-')
            str++;

        while (*str != '\0')
        {
            if (*str < '0' || *str > '9')
                return -1;
            str++;
        }

        int exit_code = atoi(command->args[1]);
        if (exit_code < 0 || exit_code > 255)
            return -1;

        return exit_code;
    }
    else
        return check.exit_status;
}

void our_exit(t_cmd *command)
{
    int exit_code = get_exit_code(command);

    if (exit_code == -1)
    {
        print_exit_error("minishell: exit: numeric argument required\n");
        exit(255);
    }

    check.exit_status = exit_code;

    if (get_nb_args(command->args) > 2)
    {
        print_exit_error("minishell: exit: too many arguments\n");
        exit(check.exit_status);
    }

    exit(check.exit_status);
}
