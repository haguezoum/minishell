/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 18:13:02 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/08 19:22:41 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	display_chdir_error(const char *path, char **environment)
{
	write(STDERR_FILENO, "minishell: cd: ", 15);
	write(STDERR_FILENO, path, strlen(path));
	write(STDERR_FILENO, ": No such file or directory\n", 29);
}

int	change_directory(const char *path, char **environment)
{
	if (chdir(path) != 0)
	{
		display_chdir_error(path, environment);
		g_check.exit_status = 1;
		return (1);
	}
	return (0);
}
