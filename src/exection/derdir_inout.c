/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   derdir_inout.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 00:50:20 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/03 20:37:36 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	herdoc(char *match, t_environment *env)
{
	char	*line;
	int		fd;
	char	*line_to_write;
	char	*expanded_line;

	check.exit_status = EXIT_SUCCESS;
	line = NULL;
    unlink("/tmp/.minishell");
	fd = open("/tmp/.minishell", O_RDWR | O_CREAT  | O_TRUNC, 0666);
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
    close(fd);
    fd =  open("/tmp/.minishell", O_RDONLY);
	dup2(fd, STDIN_FILENO);
	return (1);
}

int	derdir(char *argument)
{
	int exit_status = EXIT_SUCCESS;
	int fd;
	fd = open(argument, O_CREAT | O_WRONLY | O_APPEND, 0644);
	dup2(fd, 1);
	close(fd);

	return (exit_status);
}