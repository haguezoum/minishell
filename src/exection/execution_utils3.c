/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 13:44:48 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/08 14:12:43 by haguezou         ###   ########.fr       */
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

char	*ft_remove_char(char *str, char c)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = ft_calloc(ft_strlen(str), sizeof(char));
	while (str[i])
	{
		if (str[i] != c)
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	return (new_str);
}

void	print_environment(t_environment *env)
{
	t_environment	*e_tmp;

	e_tmp = env->next;
	while (e_tmp)
	{
		if (e_tmp->data)
			printf("declare -x %s=\"%s\"\n", e_tmp->name, e_tmp->data);
		else
			printf("%s\n", e_tmp->name);
		e_tmp = e_tmp->next;
	}
}

void	export_names(char **tmp, t_environment *env)
{
	int	i;

	i = 1;
	while (tmp[i])
	{
		our_export(tmp[i], env, 0);
		i++;
	}
}
