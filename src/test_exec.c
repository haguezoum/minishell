#include "minishell.h"

/**
 * Searches for a given command in the directories listed in the PATH environment variable.
 * 
 * @return 0 if the command is found in at least one directory, otherwise returns -1.
 */
char* check_cmand_exist_in_dir(t_node *ptr)
{
    char *str = getenv("PATH");
    int i = 0;
    char **path = ft_split(str, ':');
    while (path[i])
    {
        if(access(ft_strjoin(ft_strjoin(path[i],"/"), ptr->content.command.args[0]), F_OK) == 0)
        {
           return ft_strjoin(ft_strjoin(path[i],"/"), ptr->content.command.args[0]);
        }
       i++;
    }
   
    return NULL;
}

int check_builtin_cmd(t_node *ptr, char **evn_vars)
{
    if (ptr->node_type == COMMAND)
    {
        if (ft_strcmp(ptr->content.command.args[0], "echo") == 0)
        {
            // ft_echo(ptr->content.command.args);
            return 1;
        }
        else if (ft_strcmp(ptr->content.command.args[0], "cd") == 0)
        {
            // our_cd(ptr->content.command.args, evn_vars); // cd command that takes care of the cd - and cd ~
            our_cd(&(ptr->content.command), &evn_vars);

            return 1;
        }
        else if (ft_strcmp(ptr->content.command.args[0], "pwd") == 0)
        {
            our_pwd(&(ptr->content.command));
            return 1;
        }
        else if (ft_strcmp(ptr->content.command.args[0], "export") == 0)
        {
            // ft_export(ptr->content.command.args, evn_vars);
            return 1;
        }
        else if (ft_strcmp(ptr->content.command.args[0], "unset") == 0)
        {
            our_unset(&(ptr->content.command.args), evn_vars);
            return 1;
        }
        else if (ft_strcmp(ptr->content.command.args[0], "env") == 0)
        {
            our_env(&(ptr->content.command), evn_vars);
            return 1;
        }
        else if (ft_strcmp(ptr->content.command.args[0], "exit") == 0)
        {
            // ft_exit(ptr->content.command.args);
            return 1;
        }
    }
    return 0;
}
void execute(t_node *ptr, char **evn_vars)
{
    if(ptr->node_type == COMMAND && !check_builtin_cmd(ptr, evn_vars))
    {
        char *str = check_cmand_exist_in_dir(ptr); // check if the command exists in the directories listed in the PATH environment variable
        if(str) // if the command exists in at least one directory
        {
            pid_t pid;
            int status;
            pid = fork();
            if (pid == 0)
            {
                if (execve(str, ptr->content.command.args, evn_vars) == -1)
                {
                    perror("execv");
                    exit(1);
                }
            }
            else if (pid < 0)
            {
                perror("fork");
                exit(1);
            }
            else
            {
                waitpid(pid, &status, 0);
            }
        }
        else
        {
            printf("bash: %s: command not found\n", ptr->content.command.args[0]);
        }
    }
    else if(ptr->node_type == PIPE)
    {
        int fd[2];
        pid_t pid;
        int status;
        pipe(fd);
        pid = fork();
        if (pid == 0)
        {
            // child proccess
            dup2(fd[1], 1); // redirect stdout to the write end of the pipe
            close(fd[0]); // close the read end of the pipe
            close(fd[1]); // close the write end of the pipe
            execute(ptr->content.pipe.left, evn_vars); // execute the left side of the pipe
            exit(0); // exit the child process
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(1);
        }
        else
        {
            waitpid(pid, &status, 0); // wait for the child process to finish
            dup2(fd[0], 0); // redirect stdin to the read end of the pipe
            close(fd[0]); // close the read end of the pipe
            close(fd[1]); // close the write end of the pipe
            execute(ptr->content.pipe.right, evn_vars); //execute the right side of the pipe
        }
    }
}
