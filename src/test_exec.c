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
int check_builtin_cmd(t_cmd *ptr) //check if the given command is builtin or not 
{
        if (ft_strcmp(ptr->args[0], "echo") == 0)
        {
            return 1;
        }
        else if (ft_strcmp(ptr->args[0], "cd") == 0)
        {
            return 1;
        }
        else if (ft_strcmp(ptr->args[0], "pwd") == 0)
        {
            return 1;
        }
        else if (ft_strcmp(ptr->args[0], "export") == 0)
        {
            return 1;
        }
        else if (ft_strcmp(ptr->args[0], "unset") == 0)
        {
            return 1;
        }
        else if (ft_strcmp(ptr->args[0], "env") == 0)
        {
            return 1;
        }
        else if (ft_strcmp(ptr->args[0], "exit") == 0)
        {
            return 1;
        }
    return 0;
}
void excute_builtin(t_cmd *ptr, t_environment *env, t_global *token_list) //should pass the whole structer t_environment *evn_vars
{   
        if (ft_strcmp(ptr->args[0], "echo") == 0)
        {
            our_echo(ptr, token_list ,env->environment_array);
        }
        else if (ft_strcmp(ptr->args[0], "cd") == 0)
        {
            our_cd(ptr, &env->environment_array); // cd command that takes care of the cd $var and the second argument is the structer that contains the env variables
        }
        else if (ft_strcmp(ptr->args[0], "pwd") == 0)
        {
            our_pwd(ptr);
        }
        else if (ft_strcmp(ptr->args[0], "export") == 0)
        {
            // our_export();
        }
        else if (ft_strcmp(ptr->args[0], "unset") == 0)
        {
            our_unset(ptr, &env); // unset command that takes care of the unset $var and the second argument is the structer that contains the env variables
        }
        else if (ft_strcmp(ptr->args[0], "env") == 0)
        {
            our_env(ptr, env->environment_array);
        }
        else if (ft_strcmp(ptr->args[0], "exit") == 0)
        {
            // our_exit();
        }
}
int execute(t_node *ptr, char **evn_vars)
{
    if(ptr->node_type == COMMAND)
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


void excution(t_node *ptr, t_environment *evn_vars, t_global *token_list)
{
    if(check_builtin_cmd(&(ptr->content.command)))
    {
        //printf("builtin\n"); // this is just for testing
        excute_builtin(&(ptr->content.command), evn_vars, token_list); // excute the builtin command
    }
    else
    {
        //printf("not builtin\n"); // this is just for testing
        execute(ptr, evn_vars->environment_array); // excute the command that stored in structer astTree->top
    }
}