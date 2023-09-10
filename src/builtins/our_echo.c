/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 17:04:10 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/10 03:04:21 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_option_n(int *hasOptionN, char **args, int *cur_index)
{
	if (args[*cur_index] && args[*cur_index][0] == '-'
		&& check_option(args[*cur_index]))
	{
		*hasOptionN = 1;
		(*cur_index)++;
	}
}

int	hyphen_check(char *s)
{
	int	i;

	i = 0;
	while (i < (int)ft_strlen(s))
	{
		if (s[i] == '-')
			return (1);
		i++;
	}
	return (0);
}

int	process_echo_options(t_cmd *command, int *has_option_n, int cur_index)
{
	while (command->args[cur_index] && hyphen_check(command->args[cur_index])
		&& check_option(command->args[cur_index]))
	{
		(*has_option_n) = 1;
		cur_index++;
	}
	return (cur_index);
}

int	print_echo_argument(t_cmd *command, int first_arg, int cur_index)
{
	if (!first_arg)
		write(STDOUT_FILENO, " ", 1);
	else
		first_arg = 0;
	write(STDOUT_FILENO, command->args[cur_index],
		ft_strlen(command->args[cur_index]));
	cur_index++;
	return (cur_index);
}

void	our_echo(t_cmd *command)
{
	int	cur_index;
	int	first_arg;
	int	has_option_n;

	cur_index = 1;
	first_arg = 1;
	has_option_n = 0;
	cur_index = process_echo_options(command, &has_option_n, cur_index);
	while (command->args[cur_index])
	{
		cur_index = print_echo_argument(command, first_arg, cur_index);
		first_arg = 0;
	}
	if (!has_option_n)
		write(STDOUT_FILENO, "\n", 1);
	g_check.exit_status = EXIT_SUCCESS;
}
