/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 17:04:36 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/02 17:04:46 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void update_env_variable(t_environment *env_vars, const char *name, const char *value) {
    t_environment *current_env_elem = env_vars;
    while (current_env_elem && strcmp(current_env_elem->name, name) != 0)
        current_env_elem = current_env_elem->next;
    if (current_env_elem) {
        free(current_env_elem->data);
        current_env_elem->data = ft_strdup(value);
    }
}

char **convert_env_list_to_array(t_environment *env_vars) {
    char **env_array = ft_calloc(env_vars->count + 1, sizeof(char *));
    int i = 0;

    if (env_array) {
        t_environment *current_env_elem = env_vars;
        while (current_env_elem) {
            env_array[i] = ft_strjoin(current_env_elem->name, "=");
            if (current_env_elem->data)
                env_array[i] = ft_strjoin(env_array[i], current_env_elem->data);
            current_env_elem = current_env_elem->next;
            i++;
        }
        env_array[i] = NULL;
    }
    return env_array;
}

int handle_change_directory(const char *path, char ***environment) {
    if (change_directory(path, environment) != 0)
        return 1;

    char current_working_dir[PATH_MAX];
    getcwd(current_working_dir, PATH_MAX);

    return 0;
}

void handle_update_env_variables(t_environment *env_vars, const char *current_working_dir, const char *old_working_dir) {
    update_env_variable(env_vars, "PWD", current_working_dir);
    update_env_variable(env_vars, "OLDPWD", old_working_dir);
}

int our_cd(t_cmd *command, char ***environment) {
    t_environment *env_vars = create_env_vars(*environment);

    char current_working_dir[PATH_MAX];
    char old_working_dir[PATH_MAX];

    if (!command->args[1]) {
        const char *home_dir = expand_vars("$HOME", *environment);
        return handle_change_directory(home_dir, environment);
    } else {
        return handle_change_directory(command->args[1], environment);
    }

    handle_update_env_variables(env_vars, current_working_dir, old_working_dir);

    char **env_array = convert_env_list_to_array(env_vars);
    *environment = env_array;

    check.exit_status = 0;
    return 0;
}
