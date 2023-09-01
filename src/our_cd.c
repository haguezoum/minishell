#include "./includes/minishell.h"

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

// Function to change the current working directory
int change_directory(const char *path, char ***environment) {
    if (chdir(path) != 0) {
        display_chdir_error(path);
        check.exit_status = 1;
        return 1;
    }
    return 0;
}

// Function to update an environment variable's value
void update_env_variable(t_environment *env_vars, const char *name, const char *value) {
    t_environment *current_env_elem = env_vars;
    while (current_env_elem && strcmp(current_env_elem->name, name) != 0)
        current_env_elem = current_env_elem->next;
    if (current_env_elem) {
        free(current_env_elem->data);
        current_env_elem->data = ft_strdup(value);
    }
}
void free_environment(t_environment *env)
{
    if (env == NULL)
        return;

    // Free individual members
    free(env->name);
    free(env->data);
    for (int i = 0; i < env->count; i++)
    {
        free(env->environment_array[i]);
    }
    free(env->environment_array);

    // Recursively free the linked list
    free_environment(env->next);

    // Free the current struct
    free(env);
}
// Function to convert environment list to an array
char **convert_env_list_to_array(t_environment *env_vars) {
    char **env_array = ft_calloc(env_vars->count + 1, sizeof(char *));
    int i = 0;

    if (env_array) {
        t_environment *current_env_elem = env_vars;
        while (current_env_elem) {
            // Create entries for environment array in the form "NAME=VALUE"
            env_array[i] = ft_strjoin(current_env_elem->name, "=");
            if (current_env_elem->data)
                env_array[i] = ft_strjoin(env_array[i], current_env_elem->data);
            current_env_elem = current_env_elem->next;
            i++;
        }
        env_array[i] = NULL;
    }
    free_environment(env_vars);
    return env_array;
}

// Handle changing directory to the specified path
int handle_change_directory(const char *path, char ***environment , int x) {
    if (change_directory(path, environment) != 0)
    {
        if(x)
            free((void*)path);
        return 1;
    }

    char current_working_dir[PATH_MAX];
    getcwd(current_working_dir, PATH_MAX);
    if(x)
        free((void*)path);
    return 0;
}

// Handle updating PWD and OLDPWD environment variables
void handle_update_env_variables(t_environment *env_vars, const char *current_working_dir, const char *old_working_dir) {
    update_env_variable(env_vars, "PWD", current_working_dir);
    update_env_variable(env_vars, "OLDPWD", old_working_dir);
    free_environment(env_vars);
}



// Custom implementation of the cd command
int our_cd(t_cmd *command, char ***environment) {

    char current_working_dir[PATH_MAX];
    char old_working_dir[PATH_MAX];

    if (!command->args[1]) {
        // const char *home_dir = expand_vars("$HOME", *environment);
        // free_environment(env_vars);
        return handle_change_directory(expand_vars("$HOME", *environment), environment, 1);
    } else {
        // free_environment(env_vars);
        return handle_change_directory(command->args[1], environment, 0);
    }

    handle_update_env_variables(create_env_vars(*environment), current_working_dir, old_working_dir);

    *environment = convert_env_list_to_array(create_env_vars(*environment));
    // free_environment(env_vars);
    check.exit_status = 0;
    return 0;
}
