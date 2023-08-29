/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 17:56:26 by haguezou          #+#    #+#             */
/*   Updated: 2023/08/29 18:47:37 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void export(t_cmd *ptr, t_environment *env)
{
    char *es;
    if (!ptr->args[1]) {
        t_environment *tmp = env->next;
        while (tmp) 
        {
            if (tmp->data) 
            {
                printf("declare -x %s=\"%s\"\n", tmp->name, tmp->data);
            }
            else
                printf("%s\n", tmp->name);
            tmp = tmp->next;
        }
        return ;
    }
    if(!ptr->args[2])
    {
        es = ft_strchr(ptr->args[1], '=');
        if (!es)
        {
            printf("es : %s\n", es);
            our_export(ptr->args[1], env);
            return ;
        }
        char *arg2 = ft_strdup(es + 1);
        char *arg1 = ft_substr(ptr->args[1], 0, ft_strlen(ptr->args[1]) - ft_strlen(es));
        char *str = ft_strjoin(arg1, "=");
        str = ft_strjoin(str, arg2);
        our_export(str, env);
    }
    else
    {
        char *str = ft_strjoin(ptr->args[1], ptr->args[2]);
        our_export(str, env);
    }
}
