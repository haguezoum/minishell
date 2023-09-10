/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 23:45:25 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/08 19:22:41 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	our_pwd(t_cmd *command)
{
	char	*directory_path;

	(void)command;
	directory_path = (char *)malloc(PATH_MAX);
	if (!directory_path)
	{
		perror("minishell: pwd: malloc");
		g_check.exit_status = EXIT_FAILURE;
		return (-1);
	}
	if (getcwd(directory_path, PATH_MAX) == NULL)
	{
		perror("minishell: pwd: ");
		free(directory_path);
		g_check.exit_status = EXIT_FAILURE;
		return (-1);
	}
	printf("%s\n", directory_path);
	free(directory_path);
	g_check.exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
