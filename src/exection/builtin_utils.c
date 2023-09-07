/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 17:56:26 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/07 23:51:18 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoin2(char *s1, char *s2)
{
	char	*new_str;
	size_t	i;
	size_t	j;

	if (!s1)
	{
		s1 = malloc(1);
		s1[0] = '\0';
	}
	new_str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (new_str == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
		new_str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		new_str[j++] = s2[i++];
	new_str[j] = '\0';
	return (new_str);
}
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

char *ft_remove_char(char *str, char c)
{
    int i = 0;
    int j = 0;
    char *new_str = ft_calloc(ft_strlen(str), sizeof(char));
    while(str[i])
    {
        if(str[i] != c)
        {
            new_str[j] = str[i];
            j++;
        }
        i++;
    }
    return (new_str);
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
	int i;
	
	i = 1;
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
	if (check_equal(token))
	{
		args = ft_split(token, '=');
		free(token);
		tmp = ft_split(args[0], ' ');
	}
	else
	{
		tmp = ft_split(token, ' ');
		free(token);
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
			free_double_pointer(tmp);
			return ;
		}
		while(tmp[i])
		{
			our_export(tmp[i], env, 0);
			i++;
		}
		free_double_pointer(tmp);
		return ;
	}
	if (args[1])
		value = ft_remove_char(args[1], '"');
	else
		value = NULL;
	key = ft_strdup(tmp[1]);
	t_arg = ft_strjoin(key, "=");
	char	*t;
	if (value)
		t = ft_strjoin(t_arg, value);
	free(value);
	free_double_pointer(tmp);
	free_double_pointer(args);
	our_export(t, env, 1);
	free(t);
}
