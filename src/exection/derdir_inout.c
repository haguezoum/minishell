/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   derdir_inout.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 00:50:20 by haguezou          #+#    #+#             */
/*   Updated: 2023/08/30 19:31:35 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	herdoc(char *match, t_environment *env)
{
	int		exit_status;
	char	*line;
	char	*path;
	int		fd;
	char	*line_to_write;
	char	*expanded_line;

	exit_status = EXIT_SUCCESS;
	line = NULL;
	path = ft_strjoin("/tmp/", match);
	fd = open(path, O_RDWR | O_CREAT | O_APPEND | O_TRUNC, 0666);
	while (1)
	{
		line = readline(">");
		if (line == NULL || ft_strncmp(line, match, ft_strlen(match)) == 0)
		{
			free(line);
			exit_status = EXIT_FAILURE;
			break ;
		}
		else
		{
			line_to_write = line;
			if (ft_strchr(line, '$') != NULL)
			{
				expanded_line = expand_vars(line, env->environment_array);
				line_to_write = expanded_line;
			}
			write(fd, line_to_write, ft_strlen(line_to_write));
			write(fd, "\n", 1);
			if (line_to_write != line)
				free(line_to_write);
		}
	}
	dup2(fd, STDIN_FILENO);
	free(line);
	free(path);
	return (exit_status);
}

int implement_herdocument(char *match, t_environment *env)
{
    int fd;
    char *input;
	(void)env;
	
    unlink("/tmp/herdoc_file");
    fd = open("/tmp/herdoc_file", O_CREAT | O_WRONLY, 0777);
    if (fd == -1) {
        perror("Error opening file");
        return -1;
    }

    while (1) {
        input = readline(">");
        if (input == NULL) {
            perror("Error reading input");
            break;
        }

        if (strcmp(input, match) == 0) {
            free(input);
            break;
        }

        write(fd, input, strlen(input));
        write(fd, "\n", 1);
        free(input);
    }

    close(fd);
    return 0;
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