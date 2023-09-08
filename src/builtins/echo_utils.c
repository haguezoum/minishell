/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:25:19 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/08 18:37:24 by aet-tass         ###   ########.fr       */
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
