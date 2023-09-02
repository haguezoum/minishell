/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 04:54:45 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/02 22:44:46 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *store_vars(char *search, t_environment *env) {
    search = ft_strtrim(search, " \t");
    t_environment *tmp = env->next;
    if (ft_strcmp(search, "?") == 0 || ft_strcmp(search, "?\n") == 0) {
        return ft_itoa(check.exit_status);
    }
    while (tmp) {
        if (ft_strcmp(tmp->name, search) == 0) {
            free(search);
            return ft_strdup(tmp->data);
        }
        tmp = tmp->next;
    }

    return NULL;
}


int get_env_search_len(char *line, int *i) {
    int env_search_len = 0;
    while (line[*i] && ft_isalnum(line[*i])) {
        env_search_len++;
        (*i)++;
    }
    return env_search_len;
}

char *get_env_search(char *line, int i, int env_search_len) {
    char *env_search = ft_substr(line, i - env_search_len, env_search_len);
    return env_search;
}

char *find_env_value(char *env_search, char **environment_array) {
    int k = 0;
    while (environment_array[k]) {
        char *pos = ft_strchr(environment_array[k], '=');
        if (pos != NULL && strncmp(environment_array[k], env_search, pos - environment_array[k]) == 0) {
            return pos + 1; // Skip the '=' character
        }
        k++;
    }
    return NULL;
}

void append_to_result(char *res, int *j, char *value) {
    while (*value) {
        if (*j >= 1023) {
            char *new_res = realloc(res, 2 * sizeof(res));
            if (!new_res) {
                free(res);
                return;
            }
            res = new_res;
        }
        res[(*j)++] = *value++;
    }
}
