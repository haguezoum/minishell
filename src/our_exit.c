#include "minishell.h"

// int our_exit(t_cmd *command, char **environment)
// {
//     int index_var;

//     index_var = 0;
//     // Check if there are no arguments after the command
//     if (!command->args[1])
//     {
//         // If there are no arguments, exit with status 0
//         if (check.exit_status != -1)
//             check.exit_status = 0;
//         return (EXIT_SUCCESS);
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
#include "minishell.h"

#include <unistd.h>
// Function to convert an integer to a string
static void int_to_str(int num, char *buffer) {
    int i = 0;
    int is_negative = 0;

    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    // Reverse the string
    int len = i;
    for (int j = 0; j < len / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[len - j - 1];
        buffer[len - j - 1] = temp;
    }
}
// Placeholder function to deallocate memory
void free_memory(char **environment) {

    // Free memory for environment variables
    if (environment) {
        for (int i = 0; environment[i]; i++) {
            free(environment[i]);
        }
        free(environment);
    }
}
void close_file_descriptors(t_tree_node *node) {
    if (node == NULL) {
        return;
    }

    if (node->node_type == COMMAND) {
        // Close input and output file descriptors if they are not standard ones
        if (node->content.command.fd.input != STDIN_FILENO) {
            close(node->content.command.fd.input);
        }
        if (node->content.command.fd.output != STDOUT_FILENO) {
            close(node->content.command.fd.output);
        }

        // Recursively close file descriptors for redirections
        t_relemt *redir = node->content.command.redirections->first;
        while (redir != NULL) {
            if (redir->type == REDIR_IN || redir->type == REDIR_OUT
                || redir->type == DREDIR_OUT || redir->type == HERE_DOC) {
                close(node->content.command.fd.input);
            }
            redir = redir->next;
        }
    } else if (node->node_type == PIPE) {
        close_file_descriptors(node->content.pipe.left);
        close_file_descriptors(node->content.pipe.right);
    }
}



// Implementation of our_exit
void our_exit(t_cmd *command) {
    if (command->args[1]) {
        int exit_code = atoi(command->args[1]);

        if (exit_code == 0 && strcmp(command->args[1], "0") != 0) {
            // Conversion failed, treat it as an error
            char error_msg[1024];
            int_to_str(check.exit_status, error_msg);
            write(STDERR_FILENO, "minishell: exit: ", 17);
            write(STDERR_FILENO, error_msg, strlen(error_msg));
            write(STDERR_FILENO, ": numeric argument required\n", 28);
            check.exit_status = EXIT_FAILURE;
        } else {
            // Set the exit status and call cleanup function before exit
            check.exit_status = exit_code;
            // cleanup_before_exit(command->environment);
            exit(check.exit_status);
        }
    } else {
        // If no argument is provided, use the default exit status
        // cleanup_before_exit(command->environment);
        exit(check.exit_status);
    }
}
