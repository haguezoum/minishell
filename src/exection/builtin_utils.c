/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 17:56:26 by haguezou          #+#    #+#             */
/*   Updated: 2023/08/30 15:52:09 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	export_utils(t_cmd *ptr, t_environment *env, char *arg)
{
	char	*es;
	char	*str;
	char	*arg2;
	char	*arg1;

	es = ft_strchr(arg, '=');
	if (!es)
	{
		our_export(arg, env);
		return ;
	}
	arg2 = ft_strdup(es + 1);
	arg1 = ft_substr(arg, 0, ft_strlen(arg) - ft_strlen(es));
	str = ft_strjoin(arg1, "=");
	str = ft_strjoin(str, arg2);
	our_export(str, env);
}

void	export(t_cmd *ptr, t_environment *env)
{
	char			*es;
	t_environment	*tmp;
	char			*str;

	if (!ptr->args[1])
	{
		tmp = env->next;
		while (tmp)
		{
			if (tmp->data)
				printf("declare -x %s=\"%s\"\n", tmp->name, tmp->data);
			else
				printf("%s\n", tmp->name);
			tmp = tmp->next;
		}
		return ;
	}
	if (!ptr->args[2])
		export_utils(ptr, env, ptr->args[1]);
	else
	{
		str = ft_strjoin(ptr->args[1], ptr->args[2]);
		our_export(str, env);
	}
}
