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
            our_exit(ptr, env->environment_array);
        }
}
void exec_cmd(t_node *ptr, t_environment *evn_vars, t_global *token_list)
{
    
    pid_t pid;
    int status;
    int fd;

    if(check_builtin_cmd(&(ptr->content.command))) // check if the command is builtin or not
    {
        excute_builtin(&(ptr->content.command), evn_vars, token_list);
    }
    else
    {    //excute external command
        int stdin = dup(0);
        int stdout = dup(1);
        char *str = check_cmand_exist_in_dir(ptr); // check if the command exists in the directories listed in the PATH environment variable
        if(str) // if the command exists in at least one directory then fork and execute the command
        {
            pid = fork();
            if (pid == 0)
            {
                if(ptr->content.command.redirections) // if the command has redirections then redirect the input and output
                {
                    t_relem *tmp = ptr->content.command.redirections->first;
                    while (tmp) {
                        if (tmp->type == REDIR_OUT) // if the redirection is output then redirect the output > to the file
                        {
                            fd = open(tmp->argument, O_CREAT | O_WRONLY  | O_TRUNC, 0644);
                            if (fd < 0)
                                dprintf(2, "erorooorororo\n");
                            dup2(fd, 1);
                            close(fd);
                        }
                        else if (tmp->type == REDIR_IN)
                        {
                            int acc = access(tmp->argument, F_OK);
                            if(acc == -1)
                            {
                                printf("bash: %s: No such file or directory\n", tmp->argument);
                                exit(1);
                            }
                            int fd = open(tmp->argument, O_RDONLY);
                            dup2(fd, STDIN_FILENO);
                            close(fd);
                        }
                        else if(tmp->type == DREDIR_OUT)
                        {
                            fd = open(tmp->argument, O_CREAT | O_WRONLY  | O_APPEND, 0644);
                            dup2(fd, 1);
                            close(fd);
                        }
                        else if(tmp->type == HERE_DOC)
                        {
                            char *line = NULL;
                            int fd = open(tmp->argument, O_RDONLY, 0644);
                            // close(fd);
                            dup2(fd, STDIN_FILENO);
                            close(fd);
                        }
                        tmp = tmp->next;
                    }
                }
                execve(str, ptr->content.command.args, evn_vars->environment_array);
            }
            else if (pid < 0)
            { 
                //error in forking
                perror("fork");
                exit(1);
            }
            else
            {
                // parent proccess
                waitpid(pid, &status, 0); // wait for the child process to finish
            }
        }
        else // if the command does not exist in any directory then print error message
        {
            printf("bash: %s: command not found\n", ptr->content.command.args[0]);
            //free str  // free the string that contains the path of the command
        } 
        //free str  // free the string that contains the path of the command
    }
}

void run_pipe(t_node *ptr, t_environment *evn_vars, t_global *token_list)
{
    pid_t l_pid, r_pid;
    int status;
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return;
    }
    l_pid = fork();
    if (l_pid == 0)
    {
        // child proccess
        // printf("Right but into left fork %s\n", ptr->content.pipe.left->content.command.args[0]);
        
        dup2(fd[1], STDOUT_FILENO); // redirect the output of the left side of the pipe to the input of the right side of the pipe
        close(fd[0]); // close the input of the left side of the pipe
        execute_tree(ptr->content.pipe.right, evn_vars, token_list); // execute the left side of the pipe
        close(fd[1]); // close the output of the left side of the pipe
        exit(0);
    }
    r_pid = fork();
    if(r_pid == 0)
    {
        // printf("Right but into left fork%s\n", ptr->content.pipe.right->content.command.args[0]);
        dup2(fd[0], STDIN_FILENO); // redirect the input of the right side of the pipe to the output of the left side of the pipe
        close(fd[1]); // close the output of the right side of the pipe
        execute_tree(ptr->content.pipe.left, evn_vars, token_list); // execute the left side of the pipe
        close(fd[0]); // close the input of the right side of the pipe
        exit(0);
    }
    close(fd[0]); // close the input of the right side of the pipe
    close(fd[1]); // close the output of the right side of the pipe
    waitpid(l_pid, &status, 0); // wait for the child process to finish
    waitpid(r_pid, &status, 0); // wait for the child process to finish
}
int execute_tree(t_node *ptr, t_environment *evn_vars, t_global *token_list) // call the functiom from main to execute the command that stored in structer astTree->top->content.command.args[0]
{
    if(ptr->node_type == COMMAND)
    {
        exec_cmd(ptr, evn_vars, token_list); // excute the command that stored in structer astTree->top->content.command.args[0]
    }
    else
    {
        run_pipe(ptr, evn_vars, token_list); // run the pipe
    }
    return 0;
}
