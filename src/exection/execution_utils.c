/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 08:54:06 by haguezou          #+#    #+#             */
/*   Updated: 2023/08/29 09:39:37 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void execute_left_child(t_node *left_node, int pipe_fd[2], t_environment *evn_vars, t_global *token_list)
{
    close(pipe_fd[0]);
    dup2(pipe_fd[1], STDOUT_FILENO);
    execute_tree(left_node, evn_vars, token_list);
    close(pipe_fd[1]);
    exit(0);
}

void execute_right_child(t_node *right_node, int pipe_fd[2], t_environment *evn_vars, t_global *token_list)
{
    close(pipe_fd[1]);
    dup2(pipe_fd[0], STDIN_FILENO);
    execute_tree(right_node, evn_vars, token_list);
    close(pipe_fd[0]);
    exit(0);
}

int check_builtin_cmd(t_cmd *ptr) //check if the given command is builtin or not
{
        if (ft_strcmp(ptr->args[0], "echo") == 0)
            return 1;
        else if (ft_strcmp(ptr->args[0], "cd") == 0)
            return 1;
        else if (ft_strcmp(ptr->args[0], "pwd") == 0)
            return 1;
        else if (ft_strcmp(ptr->args[0], "export") == 0)
            return 1;
        else if (ft_strcmp(ptr->args[0], "unset") == 0)
            return 1;
        else if (ft_strcmp(ptr->args[0], "env") == 0)
            return 1;
        else if (ft_strcmp(ptr->args[0], "exit") == 0)
            return 1;
    return 0;
}

void excute_builtin(t_cmd *ptr, t_environment *env, t_global *token_list) //should pass the whole structer t_environment *evn_vars
{
        if (ft_strcmp(ptr->args[0], "echo") == 0)
            our_echo(ptr, token_list ,env->environment_array);
        else if (ft_strcmp(ptr->args[0], "cd") == 0)
            our_cd(ptr, &env->environment_array);
        else if (ft_strcmp(ptr->args[0], "pwd") == 0)
            our_pwd(ptr);
        else if (ft_strcmp(ptr->args[0], "export") == 0)
        {
            if (!ptr->args[1]) {
                t_environment *tmp = env->next;
                while (tmp) 
                {
                    if (tmp->data) 
                    {
                        printf("declare -x %s=\"%s\"\n", tmp->name, tmp->data);
                    }
                    else
                        printf("%s\n", tmp->name);
                    tmp = tmp->next;
                }
                return ;
            }
            // printf("ptr->args[1] : %s\n", ptr->args[1]);
            // printf("ptr->args[2] : %s\n", ptr->args[2]);
            if(!ptr->args[2])
            {
                char *es = ft_strchr(ptr->args[1], '=');
                if (!es)
                {
                    printf("es : %s\n", es);
                    our_export(ptr->args[1], env);
                    return ;
                }
                char *arg2 = ft_strdup(es + 1);
                char *arg1 = ft_substr(ptr->args[1], 0, ft_strlen(ptr->args[1]) - ft_strlen(es));
                char *str = ft_strjoin(arg1, "=");
                str = ft_strjoin(str, arg2);
                our_export(str, env);
            }
            else
            {
                char *str = strcat(ptr->args[1], ptr->args[2]);
                our_export(str, env);
            }

        }
        else if (ft_strcmp(ptr->args[0], "unset") == 0)
            our_unset(ptr, env);
        else if (ft_strcmp(ptr->args[0], "env") == 0)
            our_env(ptr, env);
        else if (ft_strcmp(ptr->args[0], "exit") == 0)
            our_exit(ptr);
}

void execute_external_command(t_node *ptr, t_environment *evn_vars)
{
    pid_t pid;
    int status;
    
    char *str = check_cmand_exist_in_dir(ptr);
        if(str)
        {
            pid = fork();
            if (pid == 0)
                execve(str, ptr->content.command.args, evn_vars->environment_array);
            else if (pid < 0)
                perror("fork");
            else
                waitpid(pid, &status, 0);
        }
        else 
            printf("minishell: %s: command not found\n", ptr->content.command.args[0]);
}
