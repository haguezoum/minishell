/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 04:55:39 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/03 22:38:59 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_dollar(char *line, int *i, char **environment_array)
{
	int		env_search_len;
	char	*env_search;
	char	*value;

	if (line[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(check.exit_status));
	}
	else
	{
		env_search_len = get_env_search_len(line, i);
		if (env_search_len > 0)
		{
			env_search = get_env_search(line, *i, env_search_len);
			if (env_search)
			{
				value = find_env_value(env_search, environment_array);
				free(env_search);
				return (value);
			}
		}
	}
	return (NULL);
}

char	*allocate_initial_memory(void)
{
	char	*res;

	res = malloc(1000);
	return (res);
}

char	*expand_vars(char *line, char **environment_array)
{
	char	*res;
	int		i;
	int		j;
	char	*value;

	res = allocate_initial_memory();
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] != '$')
			res[j++] = line[i++];
		else
		{
			i++;
			value = expand_dollar(line, &i, environment_array);
			if (value)
				append_to_result(res, &j, value);
		}
	}
	res[j] = '\0';
	return (res);
}
