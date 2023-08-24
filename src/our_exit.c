#include "minishell.h"

int our_exit(t_cmd *command, char **environment)
{
    int index_var;

    index_var = 0;
    // Check if there are no arguments after the command
    if (!command->args[1])
    {
        // If there are no arguments, exit with status 0
        if (check.exit_status != -1)
            check.exit_status = 0;
        return (EXIT_SUCCESS);
    }
    else
    {
        // If there are arguments provided after the command, indicate an error
        write(STDERR_FILENO, "minishell: env: Too many arguments.\n", 36);
        if (check.exit_status != -1)
            check.exit_status = 1;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS); // Return success
}