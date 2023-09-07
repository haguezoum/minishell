/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 18:56:24 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/07 12:17:17 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_regular_argument(t_global **token, char **arguments, int *i,
		int *ignore_arguments)
{
	if (!(*ignore_arguments))
	{
		arguments[*i] = ft_strndup((*token)->content, (*token)->size);
		(*i)++;
		(*i)++;
	}
	*token = (*token)->next_token;
}

void	handle_other_token(t_global **token, t_environment *env, t_rlist *redir,
		int *ignore_arguments)
{
	enum e_token	type;

	type = (*token)->type;
	if (check_redir(type))
	{
		*ignore_arguments = 1;
		if (!parse_redir(token, env, redir))
		{
			return ;
		}
	}
}

void	parse_quoted_argument_value(t_global **token, char **arguments,
		int *index)
{
	enum e_token	type;
	char			*joined;

	type = (*token)->type;
	*token = (*token)->next_token;
	joined = ft_strdup("");
	while ((*token)->type != type)
	{
		joined = ft_strjoin(joined, (*token)->content);
		*token = (*token)->next_token;
	}
	arguments[*index] = joined;
	(*index)++;
}

int	parse_redirection(t_global **token, t_environment *env, t_rlist *redir)
{
	enum e_token	first_type;

	first_type = (*token)->type;
	if (check_redir(first_type))
	{
		if (!parse_redir(token, env, redir))
		{
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int	parse_command_arguments(t_global **token, t_environment *env,
		t_rlist *redir, char **arguments)
{
	int		i;
	int		ignore_arguments;
	char	**argument;

	i = 0;
	ignore_arguments = 0;
	argument = arguments;
	while (*token && (*token)->type != PIPE_LINE)
	{
		if ((*token)->type == WORD)
		{
			argument[i] = ft_strdup((*token)->content);
			i++;
		}
		else if ((*token)->type == DQUOTE || (*token)->type == SQUOTE)
		{
			parse_quoted_argument_value(token, argument, &i);
		}
		else
		{
			handle_other_token(token, env, redir, &ignore_arguments);
		}
		*token = (*token)->next_token;
	}
	return (EXIT_SUCCESS);
}
