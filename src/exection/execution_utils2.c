/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 10:53:21 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/10 02:47:24 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*check_command_in_path(char **path, t_node *ptr)
{
	int		i;
	char	*tmp;
	char	*tst;

	i = 0;
	while (path[i])
	{
		tst = ft_strjoin2(path[i], "/");
		tmp = ft_strjoin2(tst, ptr->content.command.args[0]);
		free(tst);
		if (access(tmp, F_OK) == 0)
			return (tmp);
		free(tmp);
		i++;
	}
	return (NULL);
}

char	*check_cmand_exist_in_dir(t_node *ptr)
{
	char	*str;
	char	**path;
	char	*tmp;
	DIR		*dir;

	if ((dir = opendir (ptr->content.command.args[0])) != NULL) {
        printf("minishell: %s: is a directory\n", ptr->content.command.args[0]);
		g_check.exit_status = 126;
		closedir(dir);
		return (ft_strdup("dir"));
    }
	str = getenv("PATH");
	path = ft_split(str, ':');
	if (!path)
		return (NULL);
	if (access(ptr->content.command.args[0], F_OK) == 0)
	{
		free_double_pointer(path);
		return (ptr->content.command.args[0]);
	}
	tmp = check_command_in_path(path, ptr);
	free_double_pointer(path);
	return (tmp);
}

