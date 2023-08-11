#include "minishell.h"

void update_environment_array(t_environment *env) {
    // Free the existing environment array
    if (env->environment_array) {
        for (int i = 0; i < env->count; i++) {
            free(env->environment_array[i]); // Free the memory of each entry
        }
        free(env->environment_array); // Free the memory of the array itself
    }

    // Create a new environment array with updated variables
    env->environment_array = (char **)malloc((env->count + 1) * sizeof(char *));
    if (env->environment_array) {
        t_environment *current_env_elem = env; // Point to the current environment element
        int i = 0; // Index for the new environment array

        while (current_env_elem) {
            // Calculate the length needed for the entry (name + '=' + data + '\0')
            size_t entry_length = strlen(current_env_elem->name) + strlen(current_env_elem->data) + 2;
            
            // Allocate memory for the entry
            env->environment_array[i] = (char *)malloc(entry_length);
            
            if (env->environment_array[i]) {
                // Copy the name and data to the entry
                strcpy(env->environment_array[i], current_env_elem->name);
                strcat(env->environment_array[i], "=");
                strcat(env->environment_array[i], current_env_elem->data);
                
                current_env_elem = current_env_elem->next; // Move to the next environment element
                i++; // Move to the next index
            } else {
                // Handle memory allocation failure
                for (int j = 0; j < i; j++) {
                    free(env->environment_array[j]); // Free previously allocated entries
                }
                free(env->environment_array); // Free the array itself
                env->environment_array = NULL; // Reset the environment array pointer
                break; // Exit the loop
            }
        }
        env->environment_array[i] = NULL; // Set the last element to NULL to terminate the array
    }
}

int our_unset(t_cmd *cmd, t_environment **env)
{
    int i = 1; // Start from the first argument (skip the command itself)
    check.exit_status = EXIT_SUCCESS; // Initialize the exit status as success

    while (cmd->args[i])
    {
        t_environment *current = *env; // Point to the current environment element
        t_environment *previous = NULL; // Point to the previous environment element

        char *name = cmd->args[i]; // Get the name of the variable to unset

        while (current && strcmp(current->name, name) != 0)
        {
            previous = current; // Keep track of the previous element
            current = current->next; // Move to the next environment element
        }

        if (current)
        {
            if (previous)
                previous->next = current->next; // Link the previous element to the next
            else
                *env = current->next; // If no previous, update the environment pointer
            if (current->next)
                current->next->prev = previous; // Update the prev pointer of the next element
            (*env)->count--; // Decrement the environment count
            free(current->name); // Free the memory of the variable name
            free(current->data); // Free the memory of the variable data
            free(current); // Free the memory of the environment element
        }

        if (name && !ft_isalpha(name[0]))
        {
            write(2, "minishell: unset: `", 19); // Print the error message header
            write(2, name, strlen(name));   // Print the invalid identifier
            write(2, "`: not a valid identifier\n", 27); // Print the error message footer
            check.exit_status = EXIT_FAILURE; // Set the exit status to failure
        }

        i++; // Move to the next argument
    }

    update_environment_array(*env); // Update the environment array
    return (check.exit_status); // Return the exit status
}

