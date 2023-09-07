/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 17:11:19 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/06 17:11:19 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	our_env(t_cmd *command, t_environment *env)
{
	t_environment	*tmp;

	if (!command->args[1])
	{
		tmp = env->next;
		while (tmp)
		{
			if (tmp->data)
			{
				if (tmp->data[0] != '\0')
					printf("%s=%s\n", tmp->name, tmp->data);
			}
			tmp = tmp->next;
		}
		check.exit_status = EXIT_SUCCESS;
	}
	else
	{
		write(STDERR_FILENO, "minishell: env: Too many arguments.\n", 36);
		if (check.exit_status != -1)
			check.exit_status = 1;
		check.exit_status = EXIT_FAILURE;
	}
	check.exit_status = EXIT_SUCCESS;
}
