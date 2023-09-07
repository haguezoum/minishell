/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 08:54:06 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/06 17:03:41 by aet-tass         ###   ########.fr       */
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

int	check_builtin_cmd(t_cmd *ptr)
{
	if (ft_strcmp(ptr->args[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(ptr->args[0], "cd") == 0)
		return (1);
	else if (ft_strcmp(ptr->args[0], "pwd") == 0)
		return (1);
	else if (ft_strcmp(ptr->args[0], "export") == 0)
		return (1);
	else if (ft_strcmp(ptr->args[0], "unset") == 0)
		return (1);
	else if (ft_strcmp(ptr->args[0], "env") == 0)
		return (1);
	else if (ft_strcmp(ptr->args[0], "exit") == 0)
		return (1);
	return (0);
}

void	excute_builtin(t_cmd *ptr, t_environment *env, t_global *token_list)
{
	if (ft_strcmp(ptr->args[0], "echo") == 0)
		our_echo(ptr, token_list, env->environment_array);
	else if (ft_strcmp(ptr->args[0], "cd") == 0)
		our_cd(ptr, env->environment_array);
	else if (ft_strcmp(ptr->args[0], "pwd") == 0)
		our_pwd(ptr);
	else if (ft_strcmp(ptr->args[0], "export") == 0)
		export(ptr, env, token_list);
	else if (ft_strcmp(ptr->args[0], "unset") == 0)
		our_unset(ptr, env);
	else if (ft_strcmp(ptr->args[0], "env") == 0)
		our_env(ptr, env);
	else if (ft_strcmp(ptr->args[0], "exit") == 0)
		our_exit(ptr);
}

void	execute_external_command(t_node *ptr, t_environment *evn_vars)
{
	pid_t	pid;
	int		status;
	char	*str;

	str = check_cmand_exist_in_dir(ptr);
	if (str && strcmp(str, getenv("PWD")) && strcmp(str, getenv("HOME")))
	{
		pid = fork();
		if (pid == 0)
		{
			execve(str, ptr->content.command.args, evn_vars->environment_array);
		}
		else if (pid < 0)
			perror("fork");
		else
			waitpid(pid, &status, 0);
		if (ft_strcmp(ptr->content.command.args[0], str) != 0)
			free(str);
	}
	else
	{
		printf("minishell: %s: command not found\n",
			ptr->content.command.args[0]);
		check.exit_status = 127;
	}
}
