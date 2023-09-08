/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 17:11:19 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/08 19:22:41 by haguezou         ###   ########.fr       */
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
		g_check.exit_status = EXIT_SUCCESS;
	}
	else
	{
		write(STDERR_FILENO, "minishell: env: Too many arguments.\n", 36);
		if (g_check.exit_status != -1)
			g_check.exit_status = 1;
		g_check.exit_status = EXIT_FAILURE;
	}
	g_check.exit_status = EXIT_SUCCESS;
}
