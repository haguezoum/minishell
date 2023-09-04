/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   derdir_inout.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 00:50:20 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/04 11:52:52 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	herdo_utils(int fd, char *line_to_write, char *match,
		t_environment *env)
{
	char *line;
	line =  NULL;
	
	while (1)
	{
		line = readline(">");
		if (line == NULL || ft_strncmp(line, match, ft_strlen(match)) == 0)
		{
			free(line);
			check.exit_status = EXIT_FAILURE;
			break ;
		}
		else
		{
			line_to_write = line;
			if (ft_strchr(line, '$') != NULL)
				line_to_write = expand_vars(line, env->environment_array);
			write(fd, line_to_write, ft_strlen(line_to_write));
			write(fd, "\n", 1);
			if (line_to_write != line)
				free(line_to_write);
			free(line);
		}
	}
}

int	herdoc(char *match, t_environment *env)
{
	int		fd;
	char	*line_to_write;

	check.exit_status = EXIT_SUCCESS;
	
	fd = open("/tmp/.minishell", O_RDWR | O_CREAT  | O_TRUNC, 0666);
	herdo_utils(fd, line_to_write, match , env);
    close(fd);
    fd =  open("/tmp/.minishell", O_RDONLY);
	dup2(fd, STDIN_FILENO);
	return (1);
}

int	derdir(char *argument)
{
	int	exit_status;
	int	fd;

	exit_status = EXIT_SUCCESS;
	fd = open(argument, O_CREAT | O_WRONLY | O_APPEND, 0644);
	dup2(fd, 1);
	close(fd);
	return (exit_status);
}
