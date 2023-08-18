#include "minishell.h"

// Function to display error message for chdir failures
void display_chdir_error(const char *path)
{
    // Write the error message prefix
    write(STDERR_FILENO, "minishell: cd: ", 15);

    // Write the provided path causing the error
    write(STDERR_FILENO, path, strlen(path));

    // Write the error message suffix
    write(STDERR_FILENO, ": No such file or directory\n", 29);

    // Print the error message using perror
    perror("");
}

// Custom implementation of the cd command
int our_cd(t_cmd *command, char ***environment)
{
    // Declare variables
    t_environment *env_vars; // Stores environment variables as a linked list
    char *current_working_dir = NULL; // Stores the current working directory path
    char *old_working_dir = NULL; // Stores the previous working directory path

    // Initialize environment variables by creating a linked list
    env_vars = create_env_vars(*environment);

    // Allocate memory for dynamic strings to store paths
    current_working_dir = (char *)malloc(PATH_MAX); // Allocate space for current working directory path
    if (!current_working_dir)
    {
        perror("Failed to allocate memory for current_working_dir");
        return -1; // Return an appropriate error code
    }

    old_working_dir = (char *)malloc(PATH_MAX); // Allocate space for previous working directory path
    if (!old_working_dir)
    {
        perror("Failed to allocate memory for old_working_dir");
        free(current_working_dir); // Clean up previously allocated memory
        return -1; // Return an appropriate error code
    }

    // Handle cd command with or without arguments
    if (!command->args[1])
    {
        // If no arguments are provided, change to the HOME directory
        if (chdir(expand_vars("$HOME", *environment)) != 0)
        {
            // Display error message using the custom function
            display_chdir_error(expand_vars("$HOME", *environment));

            // Set the exit status to indicate an error
            check.exit_status = 1;
            return 1; // Return an error code
        }
    }
    else
    {
        // Change to the specified directory if an argument is provided
        if (chdir(command->args[1]) != 0)
        {
            // Display error message using the custom function
            display_chdir_error(command->args[1]);

            // Set the exit status to indicate an error
            check.exit_status = 1;
            return 1; // Return an error code
        }
    }

    // Get the current working directory and store it in current_working_dir
    getcwd(current_working_dir, PATH_MAX);

    // Update PWD and OLDPWD environment variables in the linked list
    t_environment *current_env_elem = env_vars;
    while (current_env_elem && strcmp(current_env_elem->name, "PWD") != 0)
        current_env_elem = current_env_elem->next;
    if (current_env_elem)
    {
        free(current_env_elem->data);
        current_env_elem->data = ft_strdup(current_working_dir);
    }

    // Update OLDPWD with the previous working directory
    current_env_elem = env_vars;
    while (current_env_elem && strcmp(current_env_elem->name, "OLDPWD") != 0)
        current_env_elem = current_env_elem->next;
    if (current_env_elem)
    {
        free(current_env_elem->data);
        current_env_elem->data = ft_strdup(old_working_dir);
    }

    // Convert the linked list of environment variables to an array
    current_env_elem = env_vars;
    char **env_array = ft_calloc(current_env_elem->count + 1, sizeof(char *));
    int i = 0;

    if (env_array)
    {
        while (current_env_elem)
        {
            // Create entries for environment array in the form "NAME=VALUE"
            env_array[i] = ft_strjoin(current_env_elem->name, "=");
            if (current_env_elem->data)
                env_array[i] = ft_strjoin(env_array[i], current_env_elem->data);
            current_env_elem = current_env_elem->next;
            i++;
        }
        env_array[i] = NULL; // Set the last element to NULL to terminate the array
    }

    // Update the environment with the new environment array
    *environment = env_array;

    // Clean up dynamic memory allocated for path strings
    free(current_working_dir);
    free(old_working_dir);

    // Update exit status and return success code
    check.exit_status = 0; // Set exit status to indicate success
    return 0; // Return success code
}