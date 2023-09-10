/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 00:50:46 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/10 03:11:13 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	redirection(t_relem *tmp, t_environment *evn_vars)
{
	if (tmp->type == REDIR_OUT)
	{
		if (redirout(tmp->argument))
			return (1);
	}
	else if (tmp->type == REDIR_IN)
	{
		if (redirin(tmp->argument))
			return (1);
	}
	else if (tmp->type == DREDIR_OUT)
	{
		if (derdir(tmp->argument))
			return (1);
	}
	else if (tmp->type == HERE_DOC)
	{
		if (herdoc(tmp->argument, evn_vars))
			return (1);
	}
	return (0);
}

void	exec_cmd(t_node *ptr, t_environment *evn_vars, t_global *token_list)
{
	int		stdin;
	int		stdout;
	t_relem	*tmp;

	stdin = dup(0);
	stdout = dup(1);
	if (ptr->content.command.redirections)
	{
		tmp = ptr->content.command.redirections->first;
		while (tmp)
		{
			redirection(tmp, evn_vars);
			tmp = tmp->next;
		}
	}
	if (check_builtin_cmd(&(ptr->content.command)))
	{
		excute_builtin(&(ptr->content.command), evn_vars, token_list);
	}
	else
		execute_external_command(ptr, evn_vars);
	dup2(stdin, STDIN_FILENO);
	dup2(stdout, STDOUT_FILENO);
	close(stdin);
	close(stdout);
}

void	run_pipe(t_node *ptr, t_environment *evn_vars, t_global *token_list)
{
	pid_t	l_pid;
	pid_t	r_pid;
	int		status;
	int		fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return ;
	}
	l_pid = fork();
	if (l_pid == 0)
		execute_left_child(ptr->content.pipe.right, fd, evn_vars, token_list);
	r_pid = fork();
	if (r_pid == 0)
		execute_right_child(ptr->content.pipe.left, fd, evn_vars, token_list);
	close(fd[0]);
	close(fd[1]);
	waitpid(l_pid, &status, 0);
	waitpid(r_pid, &status, 0);
}

int	execute_tree(t_node *ptr, t_environment *evn_vars, t_global *token_list)
{
	if (ptr->t_node_type == COMMAND)
	{
		exec_cmd(ptr, evn_vars, token_list);
	}
	else
	{
		run_pipe(ptr, evn_vars, token_list);
	}
	return (0);
}
