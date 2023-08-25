#include "minishell.h"

// Function to check if a character is a valid environment variable name character
int is_valid_env_var_char(char c) {
    return ft_isalnum(c) || c == '_'; // Checks if the character is alphanumeric or an underscore
}

// Function to check if a string is a valid environment variable name
int is_valid_env_var_name(char *str) {
    if (!str || !is_valid_env_var_char(str[0]))
        return 0; // Returns 0 if the string is empty or the first character is invalid

    for (int i = 1; str[i]; i++) {
        if (!is_valid_env_var_char(str[i]))
            return 0; // Returns 0 if any character other than valid ones is found
    }

    return 1; // Returns 1 if the string is a valid environment variable name
}

// Function to update or create a new environment variable
int update_env_var(t_environment *env, char *name, char *data) {
    t_environment *tmp = env->next;

    // Search for the existing environment variable
    while (tmp) {
        if (ft_strcmp(tmp->name, name) == 0) {
            // Update the existing variable's value
            free(tmp->data);
            tmp->data = ft_strdup(data);
            return 1; // Variable updated
        }
        tmp = tmp->next;
    }

    // If not found, create a new variable
    t_environment *new_var = ft_calloc(1,sizeof(t_environment));
    if (!new_var)
        return 0; // Memory allocation error

    new_var->name = ft_strdup(name);
    new_var->data = ft_strdup(data);

    // Add the new variable to the linked list
    t_environment *tail = env->next;
    if (!tail) {
        env->next = new_var;
        new_var->prev = env;
    } else {
        while (tail->next)
            tail = tail->next;
        tail->next = new_var;
        new_var->prev = tail;
    }
    env->count++; // Increment the count of environment variables
    return 1; // Variable added
}

// Implementation of our_export
int our_export(t_cmd *command, t_environment *env) {
    int exit_status = EXIT_SUCCESS;

    // If no arguments, print all environment variables
    if (!command->args[1]) {
        t_environment *tmp = env->next;
        while (tmp) {
            if (ft_strchr(tmp->name, '='))
                printf("declare -x %s\n", tmp->name); // Print variable with declaration
            tmp = tmp->next;
        }
        return exit_status;
    }

    // Loop through arguments and process each one
    int i = 1;
    while (command->args[i]) {
        char *arg = command->args[i];
        char *name = arg;
        char *data = NULL;

        // Separate name and data parts if '=' exists
        char *equal_sign = ft_strchr(arg, '=');
        if (equal_sign) {
            name = ft_strndup(arg, equal_sign - arg);
            data = ft_strdup(equal_sign + 1);
        }

   // Check if the name is a valid environment variable name
if (!is_valid_env_var_name(name)) {
    fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
    exit_status = EXIT_FAILURE;
    if (data)
        free(data);
} else {
    // Update or create the environment variable
    if (data) {
        printf("......\n");
        if (!update_env_var(env, name, data)) {
            perror("minishell: export");
            exit_status = EXIT_FAILURE;
        }
        else {
            for (int i = 0; env->environment_array[i]; i++)
                printf("env: %s\n", env->environment_array[i]);
        }
        free(data);
    } else {
        if (!update_env_var(env, name, "")) {
            perror("minishell: export");
            exit_status = EXIT_FAILURE;
        }
    }
}
if (equal_sign)
    // free(name);

        // if (data)
        //     free(data);

        i++;
    }

    return exit_status;
}
