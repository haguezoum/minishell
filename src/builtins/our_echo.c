/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 17:04:10 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/03 23:12:39 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_option(char *arg)
{
	int	next_index;

	next_index = 1;
	while (arg[next_index] == 'n')
	{
		next_index++;
	}
	return (arg[next_index] == '\0');
}

void	concatenate_arguments(char **args, int cur_index,
		int args_to_concatenate)
{
	char	*temp;

	while (++args_to_concatenate > 0 && args[cur_index + args_to_concatenate])
	{
		temp = ft_strjoin(args[cur_index], args[cur_index
				+ args_to_concatenate]);
		free(args[cur_index]);
		args[cur_index] = temp;
	}
}

void	process_arguments(t_cmd *command, t_global *tokenList,
		char **environment)
{
	int	cur_index;
	int	first_arg;

	cur_index = 1;
	first_arg = 1;
	while (command->args[cur_index])
	{
		if (!first_arg)
		{
			write(STDOUT_FILENO, " ", 1);
		}
		else
		{
			first_arg = 0;
		}
		write(STDOUT_FILENO, command->args[cur_index],
			ft_strlen(command->args[cur_index]));
		cur_index++;
	}
}

void	handle_option_n(int *hasOptionN, char **args, int *cur_index)
{
	if (args[*cur_index] && args[*cur_index][0] == '-'
		&& check_option(args[*cur_index]))
	{
		*hasOptionN = 1;
		(*cur_index)++;
	}
}

void	our_echo(t_cmd *command, t_global *tokenList, char **environment)
{
	int	cur_index;
	int	first_arg;
	int	has_option_n;

	cur_index = 1;
	first_arg = 1;
	has_option_n = 0;
	while (command->args[cur_index] && check_option(command->args[cur_index]))
	{
		has_option_n = 1;
		cur_index++;
	}
	while (command->args[cur_index])
	{
		if (!first_arg)
			write(STDOUT_FILENO, " ", 1);
		else
			first_arg = 0;
		write(STDOUT_FILENO, command->args[cur_index],
			strlen(command->args[cur_index]));
		cur_index++;
	}
	if (!has_option_n)
		write(STDOUT_FILENO, "\n", 1);
	check.exit_status = EXIT_SUCCESS;
}
