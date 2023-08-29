/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   derdir_inout.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 00:50:20 by haguezou          #+#    #+#             */
/*   Updated: 2023/08/29 03:16:42 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int herdoc(char *match, t_environment *env) 
{
    int exit_status  = EXIT_SUCCESS;
    char *line = NULL;
    char *path;
    int fd;
    
    path = ft_strjoin("/tmp/", match);
    fd =  open(path, O_RDWR | O_CREAT | O_APPEND | O_TRUNC, 0666);
    while (1)
    {
        line = readline(">");
        if (line == NULL || ft_strncmp(line, match, ft_strlen(match)) == 0)
        {
            free(line);
            exit_status = EXIT_FAILURE;
            break;
        }
        else
        {
            char *line_to_write = line;

            if (ft_strchr(line, '$') != NULL) 
            {
                char *expanded_line = expand_vars(line, env->environment_array);
                line_to_write = expanded_line;
            }

            write(fd, line_to_write, ft_strlen(line_to_write));
            write(fd, "\n", 1);

            if (line_to_write != line) 
            {
                free(line_to_write);
            }
        }
    }
    dup2(fd, STDIN_FILENO);
    free(line);
    free(path);
    return (exit_status);
}

int derdir(char *argument)
{
  int exit_status = EXIT_SUCCESS;
  int fd;
   fd = open(argument, O_CREAT | O_WRONLY  | O_APPEND, 0644);
   dup2(fd, 1);
   close(fd);
   
   return  (exit_status);
}