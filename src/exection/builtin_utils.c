/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 17:56:26 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/07 19:21:38 by aet-tass         ###   ########.fr       */
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
		our_export(arg, env, -1);
	else
	{
		arg2 = ft_strdup(es + 1);
		arg1 = ft_strndup(arg, es - arg);
		str = ft_strjoin(arg1, "=");
		str = ft_strjoin(str, arg2);
		free(arg2);
		our_export(str, env, 0);
		free(str);
		free(arg1);
	}
}

void	export(t_cmd *ptr, t_environment *env, t_global *token_list)
{
	char			*token;
	char			**args;
	char			**tmp;
	t_environment	*e_tmp;
	char			*value;
	char			*key;
	char			*t_arg;

	token = ft_strdup("");
	while (token_list)
	{
		if (token_list->token_state == IN_DOUBLE_QUOTES)
		{
			token = ft_strjoin(token, token_list->content);
			token_list = token_list->next_token;
		}
		token = ft_strjoin(token, token_list->content);
		token_list = token_list->next_token;
	}
	args = ft_split(token, '=');
	free(token);
	tmp = ft_split(args[0], ' ');
	if (ft_strcmp(tmp[0], "export") == 0 && !tmp[1])
	{
		e_tmp = env->next;
		while (e_tmp)
		{
			if (e_tmp->data)
				printf("declare -x %s=\"%s\"\n", e_tmp->name, e_tmp->data);
			else
				printf("%s\n", e_tmp->name);
			e_tmp = e_tmp->next;
		}
		free_double_pointer(args);
		free_double_pointer(tmp);
		return ;
	}
	value = ft_strdup(args[1]);
	key = ft_strdup(tmp[1]);
	t_arg = ft_strjoin(key, "=");
	t_arg = ft_strjoin(t_arg, value);
	our_export(t_arg, env, 1);
	free(value);
	free(t_arg);
	free_double_pointer(tmp);
	free_double_pointer(args);
}
