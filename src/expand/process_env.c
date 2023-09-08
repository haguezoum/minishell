/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 04:54:45 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/08 19:22:41 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*store_vars(char *search, t_environment *env)
{
	t_environment	*tmp;

	search = ft_strtrim(search, " \t");
	tmp = env->next;
	if (ft_strcmp(search, "?") == 0 || ft_strcmp(search, "?\n") == 0)
	{
		free(search);
		return (ft_itoa(g_check.exit_status));
	}
	while (tmp)
	{
		if (ft_strcmp(tmp->name, search) == 0)
		{
			free(search);
			return (ft_strdup(tmp->data));
		}
		tmp = tmp->next;
	}
	free(search);
	return (NULL);
}

int	get_env_search_len(char *line, int *i)
{
	int	env_search_len;

	env_search_len = 0;
	while (line[*i] && ft_isalnum(line[*i]))
	{
		env_search_len++;
		(*i)++;
	}
	return (env_search_len);
}

char	*get_env_search(char *line, int i, int env_search_len)
{
	char	*env_search;

	env_search = ft_substr(line, i - env_search_len, env_search_len);
	return (env_search);
}

char	*find_env_value(char *env_search, char **environment_array)
{
	int		k;
	char	*pos;

	k = 0;
	while (environment_array[k])
	{
		pos = ft_strchr(environment_array[k], '=');
		if (pos != NULL && strncmp(environment_array[k], env_search, pos
				- environment_array[k]) == 0)
		{
			return (pos + 1);
		}
		k++;
	}
	return (NULL);
}

void	append_to_result(char *res, int *j, char *value)
{
	char	*new_res;

	while (*value)
	{
		if (*j >= 1023)
		{
			new_res = realloc(res, 2 * sizeof(res));
			if (!new_res)
			{
				free(res);
				return ;
			}
			res = new_res;
		}
		res[(*j)++] = *value++;
	}
}
