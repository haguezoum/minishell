#include "minishell.h"

t_environment *create_env_vars(char **environment_array) {
    t_environment *env = ft_calloc(sizeof(t_environment), 1);
    if (!env)
        return NULL;

    env->name = NULL;
    env->data = NULL;
    env->next = NULL;
    env->prev = NULL;
    env->environment_array = environment_array;
    env->count = 0;
    check.exit_status = 0; // Initialize the exit status to 0

    int i = 0;
    while (environment_array[i]) {
        char *line = environment_array[i++];
        char *env_search;
        char *value;
        char *index;

        index = ft_strchr(line, '=');
        t_environment *elem = ft_calloc(sizeof(t_environment), 1);
        if (index != NULL) {
            env_search = ft_substr(line, 0, index - line);
            value = ft_strdup(index + 1);
            if (!env_search || !value || !elem) {
                free(env_search);
                free(value);
                free(elem);
                continue;
            }
            elem->name = env_search;
            elem->data = value;
        } else {
            elem->name = ft_strdup(line);
            elem->data = NULL;
        }

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

        env->count++;
    }

    return env;
}

char *store_vars(char *search, t_environment *env) {
    t_environment *tmp = env->next;

    if (ft_strcmp(search, "?") == 0 || ft_strcmp(search, "?\n") == 0) {
        return ft_itoa(check.exit_status);
    }

    while (tmp) {
        if (strcmp(tmp->name, search) == 0) {
            return ft_strdup(tmp->data);
        }
        tmp = tmp->next;
    }

    return NULL;
}


char *expand_vars(char *line, char **environment_array) {
    int i = 0;
    int j = 0;
    char *value;
    char *res = malloc(1024); // Allocate initial memory for the expanded string
    int env_search_len = 0;
    char *env_search = NULL;

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
                env_search_len = 1;
                env_search = ft_itoa(check.exit_status);
            } else {
                env_search_len = 0;
                while (line[i] && ft_isalnum(line[i])) {
                    env_search_len++;
                    i++;
                }
                env_search = malloc(env_search_len + 1);
                if (!env_search) {
                    // Failed to allocate memory
                    free(res);
                    return NULL;
                }
                strncpy(env_search, &line[i - env_search_len], env_search_len);
                env_search[env_search_len] = '\0';
            }

            // Search for the environment variable directly in the loop
            value = NULL;
            int k = 0;
            while (environment_array[k]) {
                char *pos = ft_strchr(environment_array[k], '=');
                if (pos != NULL) {
                    if (strncmp(environment_array[k], env_search, pos - environment_array[k]) == 0) {
                        value = pos + 1; // Skip the '=' character
                        break;
                    }
                }
                k++;
            }

            free(env_search);

            if (value) {
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

    res[j] = '\0';
    return ft_strdup(res);
}

