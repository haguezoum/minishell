/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 23:45:25 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/02 17:03:45 by aet-tass         ###   ########.fr       */
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
		check.exit_status = EXIT_FAILURE;
		return (-1);
	}
	if (getcwd(directory_path, PATH_MAX) == NULL)
	{
		perror("minishell: pwd: ");
		free(directory_path);
		check.exit_status = EXIT_FAILURE;
		return (-1);
	}
	printf("%s\n", directory_path);
	free(directory_path);
	check.exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
