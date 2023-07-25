#include "minishell.h"

// Create an environment from a given array of strings (variable/data pairs).
t_environment *initialize_environment(char **env_vars)
{
    t_environment *env = (t_environment*)malloc(sizeof(t_environment));
    if (!env)
        return NULL;

    int i = 0;
    while (env_vars[i])
    {
        char *variable;
        char *data;
        int index = strchr(env_vars[i], '=');
        if (index != -1)
        {
            variable = strdup(env_vars[i]);
            variable[index] = '\0'; // Remove the '=' character
            data = strdup(env_vars[i] + index + 1);
        }
        else
        {
            variable = strdup(env_vars[i]);
            data = NULL;
        }

        t_environment *new_elem = (t_environment*)malloc(sizeof(t_environment));
        if (!new_elem)
        {
            delete_environment(env);
            return NULL;
        }

        new_elem->variable = variable;
        new_elem->data = data;

        new_elem->next = NULL;
        new_elem->prev = NULL;

        if (env->head == NULL)
        {
            env->head = new_elem;
            env->tail = new_elem;
        }
        else
        {
            new_elem->prev = env->tail;
            env->tail->next = new_elem;
            env->tail = new_elem;
        }

        i++;
    }
    env->size = i;

    return env;
}

// Print the entire environment.
void print_environment(t_environment *env)
{
    t_environment *tmp = env->head;
    while (tmp)
    {
        printf("%s=%s\n", tmp->variable, tmp->data);
        tmp = tmp->next;
    }
}

// Convert the environment to an array of strings (variable/data pairs).
char **environment_to_array(t_environment *env)
{
    char **arr = (char**)malloc((env->size + 1) * sizeof(char *));
    if (!arr)
        return NULL;

    t_environment *tmp = env->head;
    int i = 0;

    while (i < env->size)
    {
        arr[i] = strdup(tmp->variable);
        if (tmp->data)
        {
            char *data_str = (char*)malloc(strlen(tmp->data) + 2); // +2 for '=' and '\0'
            if (!data_str)
            {
                // Handle allocation failure
                for (int j = 0; j < i; j++)
                    free(arr[j]);
                free(arr);
                return NULL;
            }

            snprintf(data_str, strlen(tmp->data) + 2, "=%s", tmp->data);
            arr[i] = strcat(arr[i], data_str);
            free(data_str);
        }
        tmp = tmp->next;
        i++;
    }
    arr[i] = NULL;
    return arr;
}

// Delete the entire environment.
void delete_environment(t_environment *env)
{
    t_environment *tmp = env->head;
    while (tmp)
    {
        t_environment *next = tmp->next;
        free(tmp->variable);
        free(tmp->data);
        free(tmp);
        tmp = next;
    }
    free(env);
}

// Print an array of strings representing the environment.
void print_environment_array(char **env)
{
    int i = 0;
    while (env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
}

// Get the value of a given variable in the environment.

char *get_environment_value(char *variable, t_environment *env, int exit_status)
{
    // Special case for getting the exit status as a string
    if (strcmp(variable, "?") == 0 || strcmp(variable, "?\n") == 0)
    {
        char *exit_status_str = (char*)malloc(5); // Max exit status is 255 (3 digits) + '\0'
        if (!exit_status_str)
            return NULL;

        // Use ft_itoa to convert exit_status to a string
        ft_itoa(exit_status, exit_status_str, 10);

        return exit_status_str;
    }

    // Search for the specified variable in the environment
    t_environment *elem = env->head;
    while (elem && strcmp(elem->variable, variable))
        elem = elem->next;

    // If the variable is found, return its value. Otherwise, return NULL.
    if (elem)
    {
        char *value = strdup(elem->data);
        return value;
    }
    else
    {
        return NULL;
    }
}

// Expand the environment variables in a given input.
char *expand_environment(char *input, t_environment *env, int exit_status)
{
    char *expanded_str = (char*)malloc(1024);
    if (!expanded_str)
        return NULL;

    int i = -1;
    char *value = NULL;
    while (input[++i] && input[i] != '$')
        expanded_str[i] = input[i];

    if (input[i] == '$')
    {
        input = input + i;
        value = get_environment_value(&input[1], env, exit_status);
        if (value)
        {
            int j = 0;
            while (value[j])
                expanded_str[i++] = value[j++];
        }
    }

    while (*input)
        expanded_str[i++] = *(input++);

    expanded_str[i] = '\0';
    return expanded_str;
}

