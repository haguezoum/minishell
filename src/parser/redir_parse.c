/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 08:02:26 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/07 11:40:19 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_rlist	*add_redir_element(t_rlist *redir_list, t_relem *redir_element)
{
	if (!redir_list->first)
	{
		redir_list->first = redir_element;
	}
	else
	{
		redir_list->last->next = redir_element;
	}
	redir_list->last = redir_element;
	redir_list->total++;
	return (redir_list);
}

char	*extract_first_argument(char *redir_argument)
{
	char	**args;
	char	*first_arg;
	int		i;

	args = ft_split(redir_argument, ' ');
	first_arg = NULL;
	if (args && args[0])
	{
		first_arg = ft_strdup(args[0]);
	}
	i = 0;
	while (args && args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
	return (first_arg);
}

char	*parse_redir_argument_value(t_global **token, t_environment *env,
		enum e_token redir_type)
{
	char	*redir_argument;
	char	*first_arg;

	redir_argument = NULL;
	if (is_env_var(*token) && redir_type != HERE_DOC)
	{
		redir_argument = get_env_var_value((*token)->content, env);
		if (!redir_argument)
		{
			printf("minishell: %s: ambiguous redirect\n", (*token)->content);
			return (NULL);
		}
	}
	else
	{
		redir_argument = ft_strdup((*token)->content);
	}
	first_arg = extract_first_argument(redir_argument);
	free(redir_argument);
	return (first_arg);
}

t_relem	*create_redir_element(enum e_token redir_type, char *redir_argument)
{
	t_relem	*element;

	element = ft_calloc(1, sizeof(t_relem));
	if (!element)
	{
		free(redir_argument);
		return (NULL);
	}
	element->argument = redir_argument;
	element->type = redir_type;
	return (element);
}

t_rlist	*parse_redir(t_global **token, t_environment *env, t_rlist *redir_list)
{
	enum e_token	redir_type;
	char			*redir_argument_value;
	t_relem			*redir_element;

	redir_type = (*token)->type;
	skip_to_word_or_env(token);
	redir_argument_value = parse_redir_argument_value(token, env, redir_type);
	if (!redir_argument_value)
		return (NULL);
	redir_element = create_redir_element(redir_type, redir_argument_value);
	if (!redir_element)
	{
		return (NULL);
	}
	return (add_redir_element(redir_list, redir_element));
}
