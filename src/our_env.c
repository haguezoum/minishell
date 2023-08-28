#include "minishell.h"

// int our_env(t_cmd *command, char **environment)
// {
//     int index_var;

//     index_var = 0;
//     // Check if there are no arguments after the command
//     if (!command->args[1])
//     {
//         // Iterate through the environment variables and print those that have an '=' character
//         while (environment[index_var])
//         {
//             // Check if the environment variable contains '='
//             if (ft_strchr(environment[index_var], '=') != -1)
//                 printf("%s\n", environment[index_var]); // Print the environment variable
//             index_var++;
//         }
//     }
//     else
//     {
//         // If there are arguments provided after the command, indicate an error
//         write(STDERR_FILENO, "minishell: env: Too many arguments.\n", 36);
//         if (check.exit_status != -1)
//             check.exit_status = 1;
//         return (EXIT_FAILURE);
//     }
//     return (EXIT_SUCCESS); // Return success
// }

int our_env(t_cmd *command, t_environment *env)
{
    // Check if there are no arguments after the command
    if (!command->args[1])
    {
        t_environment *tmp = env->next;
        // Iterate through the environment variables and print those that have an '=' character
        while (tmp)
        {
            if (tmp->data) {
                if (tmp->data[0] != '\0')
                    printf("%s=%s\n", tmp->name, tmp->data);
            }
            tmp = tmp->next;
        }
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
