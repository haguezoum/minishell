#include "minishell.h"

// Custom realloc function
void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
    // Allocate memory for the new size
    void *new_ptr = malloc(new_size);
    if (new_ptr)
    {
        if (ptr)
        {
            // Copy the contents from the old memory to the new memory
            ft_memcpy(new_ptr, ptr, old_size);
            // Free the old memory
            free(ptr);
        }
    }
    return new_ptr;
}

// Helper function to write error message for cd command
void write_cd_error(char *command_name, char *arg, char *home_var)
{
    // Write error message with relevant information
    write(2, "minishell: cd: ", 15);
    write(2, command_name, ft_strlen(command_name));
    write(2, ": ", 2);
    write(2, arg, ft_strlen(arg));
    write(2, ": ", 2);
    write(2, home_var, ft_strlen(home_var));
    write(2, ": No such file or directory\n", 28);
}

int our_cd(t_cmd *command, char ***environment)
{
    t_environment *vars;
    t_environment *tmp_vars;
    char **new_environment = NULL;
    int i;

    char *current_dir = NULL;
    char *old_dir = NULL;

    // Allocate memory for current_dir and old_dir using dynamic allocation
    current_dir = (char *)malloc(PATH_MAX);
    old_dir = (char *)malloc(PATH_MAX);

    if (!current_dir || !old_dir)
    {
        // Handle memory allocation error
        perror("Memory allocation error");
        check.exit_status = 1;
        return (1);
    }

    // Create environment variables list from existing environment
    vars = create_env_vars(*environment);
    // Get the current working directory and store it in old_dir
    getcwd(old_dir, PATH_MAX);

    // Check if a directory argument is provided
    if (!command->args[1])
    {
        // Change to the home directory
        if (chdir(expand_vars("$HOME", *environment)) != 0)
        {
            // Handle chdir error by writing an error message
            write_cd_error(command->args[0], expand_vars("$HOME", *environment), ": ");
            check.exit_status = 1;
            free(current_dir);
            free(old_dir);
            return (1);
        }
    }
    else
    {
        // Change to the specified directory argument
        if (chdir(command->args[1]) != 0)
        {
            // Handle chdir error by writing an error message
            write_cd_error(command->args[0], command->args[1], ": ");
            check.exit_status = 1;
            free(current_dir);
            free(old_dir);
            return (1);
        }
    }
    // Get the updated current working directory
    getcwd(current_dir, PATH_MAX);

    // Set the "PWD" and "OLDPWD" variables in the environment
    t_env_elem *elem = search_env_elem(vars, "PWD");
    if (elem)
    {
        // Update the value of the "PWD" variable
        free(elem->value);
        elem->value = ft_strdup(current_dir);
    }

    elem = search_env_elem(vars, "OLDPWD");
    if (elem)
    {
        // Update the value of the "OLDPWD" variable
        free(elem->value);
        elem->value = ft_strdup(old_dir);
    }

    i = 0;
    tmp_vars = vars->head;
    while (i < vars->size)
    {
        char *key = ft_strdup(tmp_vars->key);
        char *value = NULL;
        if (tmp_vars->value)
        {
            // If the variable has a value, create "key=value" format
            value = ft_strjoin(key, "=");
            value = ft_strjoin(value, tmp_vars->value);
        }
        // Reallocate memory for the new_environment array and store the new entry
        new_environment = ft_realloc(new_environment, i, i + 1, sizeof(char *));
        new_environment[i] = value ? value : key;
        free(key);
        tmp_vars = tmp_vars->next;
        i++;
    }
    // Finish the new_environment array with a NULL entry
    new_environment = ft_realloc(new_environment, i, i + 1, sizeof(char *));
    new_environment[i] = NULL;

    // Free allocated memory and update the environment variable list
    free(current_dir);
    free(old_dir);
    free_env_vars(vars);
    *environment = new_environment;

    // Set the exit status to indicate success
    check.exit_status = 0;
    return (0);
}

