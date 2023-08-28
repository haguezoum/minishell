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

    // printf("name: %s\n", name);
    // printf("data: %s\n", data);
    // printf("env->count: %d\n", env->count);
    // printf("env->environment_array: %s\n", env->environment_array[0]);
    // Search for the existing environment variable
    // exit(0);
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

char *get_key(char *arg)
{
    char *name = NULL;
    char *equal_sign = ft_strchr(arg, '=');

    if (equal_sign) 
    {
        name = ft_strndup(arg, equal_sign - arg);
    }
    else 
    {
        name = ft_strdup(arg);
    }

    return name;
}


char *get_value(char *arg)
{
    char *data = NULL;
    char *equal_sign = ft_strchr(arg, '=');
    if (equal_sign) 
    {
        data = ft_strdup(equal_sign + 1); // allocate memory for data and copy it from equal_sign + 1
    }
    return data;
}
// char *get_value(char *arg)
// {
//     char *equal_sign = strchr(arg, '=');
//     if (equal_sign)
//     {
//         char *value_start = equal_sign + 1;

//         // Check if the value is enclosed in double quotes
//         if (*value_start == '\"')
//         {
//             char *value_end = strchr(value_start + 1, '\"');
//             if (value_end)
//             {
//                 return strndup(value_start + 1, value_end - value_start - 1);
//             }
//         }
//         else
//         {
//             return strdup(value_start);
//         }
//     }
//     return NULL;
// }
// Implementation of our_export
int our_export(char *command, t_environment *env) 
{
    int exit_status = EXIT_SUCCESS;

    // If no arguments, print all environment variables
    if (!command) 
    {
        t_environment *tmp = env->next;
        while (tmp) 
        {
            if (tmp->data)
            {
                printf("%s=%s\n", tmp->name, tmp->data );
                // printf("%s\n", tmp->data);
            }
            else
                printf("%s", tmp->name);
            tmp = tmp->next;
        }
        return exit_status;
    }

        char *arg = command; //->args[i]; 
        char *name = NULL;
        char *data = NULL;
        
        name = get_key(arg);
        data = get_value(arg);
        // Check if the name is a valid environment variable name
        if (name && !is_valid_env_var_name(name)) 
        {
            fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
            exit_status = EXIT_FAILURE;
            if (data)
                free(data);
        }
        else 
        {
            // Update or create the environment variable
            if (data) 
            {
                printf("......\n");
                if (!update_env_var(env, name, data)) 
                {
                    perror("minishell: export");
                    exit_status = EXIT_FAILURE;
                }
                else 
                {
                    if (name) {
                        update_env_var(env, name, data);
                    }
                    // for (int i = 0; env->environment_array[i]; i++)
                    //     printf("env: %s\n", env->environment_array[i]);
                }
                free(data);
            } 
            else 
            {
                if (!update_env_var(env, name, "")) 
                {
                    perror("minishell: export");
                    exit_status = EXIT_FAILURE;
                }
            }
        }
    //    i++;
    // }

    return exit_status;
}
