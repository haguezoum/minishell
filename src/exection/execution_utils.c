/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 08:54:06 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/10 03:51:55 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_left_child(t_node *left_node, int pipe_fd[2],
		t_environment *evn_vars, t_global *token_list)
{
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	execute_tree(left_node, evn_vars, token_list);
	close(pipe_fd[1]);
	exit(0);
}

void	execute_right_child(t_node *right_node, int pipe_fd[2],
		t_environment *evn_vars, t_global *token_list)
{
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	execute_tree(right_node, evn_vars, token_list);
	close(pipe_fd[0]);
	exit(0);
}

void	excute_builtin(t_cmd *ptr, t_environment *env, t_global *token_list)
{
	if (ft_strcmp(ptr->args[0], "echo") == 0)
		our_echo(ptr);
	else if (ft_strcmp(ptr->args[0], "cd") == 0)
		our_cd(ptr);
	else if (ft_strcmp(ptr->args[0], "pwd") == 0)
		our_pwd(ptr);
	else if (ft_strcmp(ptr->args[0], "export") == 0)
		export(env, token_list);
	else if (ft_strcmp(ptr->args[0], "unset") == 0)
		our_unset(ptr, env);
	else if (ft_strcmp(ptr->args[0], "env") == 0)
		our_env(ptr, env);
	else if (ft_strcmp(ptr->args[0], "exit") == 0)
		our_exit(ptr);
}

void	ft_execve(char *str, t_node *p, t_environment *e, int s)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		perror("fork");
	else if (pid == 0)
		execve(str, p->content.command.args, e->environment_array);
	else
	{
		waitpid(pid, &s, 0);
		if (ft_strcmp(p->content.command.args[0], str) != 0)
			free(str);
	}
}

void	execute_external_command(t_node *ptr, t_environment *evn_vars)
{
	int		status;
	char	*str;

	status = EXIT_SUCCESS;
	if (!ft_strcmp(ptr->content.command.args[0], "./minishell"))
	{
		str = ft_strdup("minishell");
	}
	else
		str = check_cmand_exist_in_dir(ptr);
	if (str && !ft_strcmp(str, "dir"))
	{
		free(str);
		return ;
	}
	if (str)
		ft_execve(str, ptr, evn_vars, status);
	else
	{
		printf("minishell: %s: command not found\n",
			ptr->content.command.args[0]);
		g_check.exit_status = 127;
	}
}
