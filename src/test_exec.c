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

void execute(t_node *ptr, char **evn_vars)
{
    char *str = check_cmand_exist_in_dir(ptr);
   
   if(str)
    {
      if(ptr->node_type == COMMAND)
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
    else
    {
        printf("bash: %s: command not found\n", ptr->content.command.args[0]);
    }
}
