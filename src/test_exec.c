#include "minishell.h"

void execute(t_node *ptr)
{
    if (ptr->node_type == COMMAND) {

        // printf("%s*/*/*/*/**/*\n", ptr->content.command.args[0]);
        pid_t pid;
        int status;
        pid = fork();
        if (pid == 0)
        {
            if (execv(ft_strjoin("/bin/", ptr->content.command.args[0]), ptr->content.command.args) == -1)
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
    else if (ptr->node_type == PIPE) 
    {
        execute(ptr->content.pipe.right);

        execute(ptr->content.pipe.left );

    }

}
