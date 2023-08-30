/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_inout.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 01:25:21 by haguezou          #+#    #+#             */
/*   Updated: 2023/08/29 05:17:54 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	redirout(char *argument)
{
	int	exit_status;
	int	fd;

	exit_status = EXIT_SUCCESS;
	fd = open(argument, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		ft_putstr_fd("Error in opening file", 2);
	dup2(fd, 1);
	close(fd);
	return (exit_status);
}

int	redirin(char *argument)
{
	int	exit_status;
	int	fd;
	int	access;

	exit_status = EXIT_SUCCESS;
	access = open(argument, O_RDONLY);
	if (access < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argument, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit_status = EXIT_FAILURE;
		return (exit_status);
	}
	fd = open(argument, O_RDONLY);
	if (fd < 0)
		ft_putstr_fd("Error in opening file", 2);
	dup2(fd, 0);
	close(fd);
	return (exit_status);
}
