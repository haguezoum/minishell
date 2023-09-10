/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 17:56:26 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/10 03:13:31 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	export_key_value(char **args, char **tmp, t_environment *env)
{
	char	*value;
	char	*key;
	char	*t_arg;
	char	*t;

	key = ft_strdup(tmp[1]);
	t_arg = ft_strjoin(key, "=");
	if (args[1])
		value = ft_remove_char(args[1], '"');
	else
		value = NULL;
	if (value)
		t = ft_strjoin(t_arg, value);
	else
		t = NULL;
	free(value);
	free_double_pointer(tmp);
	free_double_pointer(args);
	our_export(t, env, 1);
	free(t);
}

int	withou_equale(char **tmp, char *token, t_environment *env)
{
	tmp = ft_split(token, ' ');
	free(token);
	if (ft_strcmp(tmp[0], "export") == 0 && !tmp[1])
	{
		print_environment(env);
		free_double_pointer(tmp);
		return (0);
	}
	export_names(tmp, env);
	free_double_pointer(tmp);
	return (0);
}

char	*parse_tokens(t_global *token_list)
{
	char	*token;

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
	return (token);
}

int	check_equal(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	export(t_environment *env, t_global *token_list)
{
	char	*token;
	char	**args;
	char	**tmp;

	args = NULL;
	tmp = NULL;
	token = parse_tokens(token_list);
	if (check_equal(token))
	{
		args = ft_split(token, '=');
		free(token);
		tmp = ft_split(args[0], ' ');
	}
	else
	{
		if (withou_equale(tmp, token, env) == 0)
			return ;
	}
	export_key_value(args, tmp, env);
}
