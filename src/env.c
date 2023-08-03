#include "minishell.h"

// Function to create environment variables from the given environment array
t_environment *create_env_vars(char **environment_array) {
    // Allocate memory for the t_environment structure and initialize it to zero
    t_environment *env = ft_calloc(sizeof(t_environment), 1);
    
    // Check if the memory allocation was successful. If not, return NULL.
    if (!env)
        return NULL;

    // Initialize the fields of the t_environment structure
    env->name = NULL;
    env->data = NULL;
    env->next = NULL;
    env->prev = NULL;
    env->environment_array = environment_array;
    env->count = 0;
    
    // Initialize the exit status to 0
    check.exit_status = 0;

    int i = 0;
    // Loop through the environment_array and create t_environment elements for each environment variable
    while (environment_array[i]) {
        char *line = environment_array[i++];
        char *env_search;
        char *value;
        char *index;

        // Find the position of '=' character in the line
        index = ft_strchr(line, '=');
        
        // Allocate memory for the new t_environment element
        t_environment *elem = ft_calloc(sizeof(t_environment), 1);
        
        // If '=' is found in the line, it means the line contains a name=value pair
        if (index != NULL) {
            // Extract the name and value parts of the environment variable
            env_search = ft_substr(line, 0, index - line);
            value = ft_strdup(index + 1);
            
            // Check if memory allocation was successful. If not, free the allocated memory and continue to the next line.
            if (!env_search || !value || !elem) {
                free(env_search);
                free(value);
                free(elem);
                continue;
            }
            elem->name = env_search;
            elem->data = value;
        } else {
            // If '=' is not found in the line, it means the line contains only the name of the environment variable (without value)
            // Set the name part of the t_environment element and keep the data part as NULL.
            elem->name = ft_strdup(line);
            elem->data = NULL;
        }

        // Add the new t_environment element to the linked list
        t_environment *tail = env->next;
        if (!tail) {
            env->next = elem;
            elem->prev = env;
        } else {
            while (tail->next)
                tail = tail->next;
            tail->next = elem;
            elem->prev = tail;
        }

        // Increment the count of environment variables in the t_environment structure
        env->count++;
    }

    return env;
}

// Function to store the value of a given environment variable
char *store_vars(char *search, t_environment *env) {
    // Start searching for the environment variable from the first node in the linked list
    t_environment *tmp = env->next;

    // If the search string is equal to "?", return the string representation of the exit status
    if (ft_strcmp(search, "?") == 0 || ft_strcmp(search, "?\n") == 0) {
        return ft_itoa(check.exit_status);
    }

    // Loop through the linked list to find the matching environment variable
    while (tmp) {
        if (strcmp(tmp->name, search) == 0) {
            // If the environment variable is found, return its data part (value)
            return ft_strdup(tmp->data);
        }
        tmp = tmp->next;
    }

    // If the environment variable is not found, return NULL
    return NULL;
}

// Function to expand environment variables in a given string
char *expand_vars(char *line, char **environment_array) {
    int i = 0;
    int j = 0;
    char *value;
    
    // Allocate initial memory for the expanded string
    char *res = malloc(1024);
    
    // Check if memory allocation was successful. If not, return NULL.
    if (!res) {
        // Failed to allocate memory
        return NULL;
    }

    while (line[i]) {
        if (line[i] != '$') {
            res[j++] = line[i++];
        } else {
            i++;
            if (line[i] == '?') {
                // If "$?" is found, replace it with the string representation of the exit status
                value = ft_itoa(check.exit_status);
            } else {
                int env_search_len = 0;
                while (line[i] && ft_isalnum(line[i])) {
                    env_search_len++;
                    i++;
                }
                // Allocate memory for the environment variable name
                char *env_search = malloc(env_search_len + 1);
                
                // Check if memory allocation was successful. If not, free the allocated memory and return NULL.
                if (!env_search) {
                    // Failed to allocate memory
                    free(res);
                    return NULL;
                }
                // Copy the environment variable name from the original string
                strncpy(env_search, &line[i - env_search_len], env_search_len);
                env_search[env_search_len] = '\0';
                
                // Search for the environment variable in the environment_array
                value = NULL;
                int k = 0;
                while (environment_array[k]) {
                    char *pos = ft_strchr(environment_array[k], '=');
                    if (pos != NULL) {
                        if (strncmp(environment_array[k], env_search, pos - environment_array[k]) == 0) {
                            // If the environment variable is found, set the value pointer to point to the value part of the environment variable
                            value = pos + 1; // Skip the '=' character
                            break;
                        }
                    }
                    k++;
                }

                free(env_search);
            }

            if (value) {
                // If the environment variable value is found, append it to the expanded string
                while (*value) {
                    if (j >= 1023) {
                        // Double the buffer size if needed
                        char *new_res = realloc(res, 2 * sizeof(res));
                        if (!new_res) {
                            // Failed to allocate memory
                            free(res);
                            return NULL;
                        }
                        res = new_res;
                    }
                    res[j++] = *value++;
                }
            }
        }
    }

    // Null-terminate the expanded string and return it
    res[j] = '\0';
    return ft_strdup(res);
}

