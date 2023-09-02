/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 18:13:50 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/02 17:07:38 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int is_valid_env_var_char(char c) {
    return ft_isalnum(c) || c == '_';
}

int is_valid_env_var_name(char *str)
{
    int i;

    i = 0;
    if (!str || !is_valid_env_var_char(str[0]))
        return 0;
    while(str[i])
    {
        if (!is_valid_env_var_char(str[i]))
            return 0;
        i++;
    }
    return 1;
}

int update_env_var(t_environment *env, char *name, char *data) {
    t_environment *tmp = env->next;

    while (tmp) {
        if (ft_strcmp(tmp->name, name) == 0) {
            free(tmp->data);
            tmp->data = ft_strdup(data);
            return 1; // Variable updated
        }
        tmp = tmp->next;
    }
    t_environment *new_var = ft_calloc(1,sizeof(t_environment));
    if (!new_var)
        return 0;
    new_var->name = ft_strdup(name);
    new_var->data = ft_strdup(data);
    t_environment *tail = env->next;
    if (!tail)
    {
        env->next = new_var;
        new_var->prev = env;
    }
    else
    {
        while (tail->next)
            tail = tail->next;
        tail->next = new_var;
        new_var->prev = tail;
    }
    env->count++;
    return 1;
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
        data = ft_strdup(equal_sign + 1);
    }
    return (data);
}
