/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 18:56:24 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/05 18:21:26 by aet-tass         ###   ########.fr       */
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
	}
	*token = (*token)->next_token;
}

void	handle_quoted_argument(t_global **token, t_environment *env,
		char **arguments, int *i, int *ignore_arguments)
{
	if (!(*ignore_arguments))
	{
		arguments[*i] = parse_quoted_argument(token, env);
		if (arguments[*i])
		{
			(*i)++;
		}
	}
	else
	{
		*token = (*token)->next_token;
	}
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	handle_env_argument(t_global **token, t_environment *env,
		char **arguments, int *i, int *ignore_arguments)
{
	char	**split;
	int		j;
	char	*first_line;
	char	*line;

	if (!(*ignore_arguments))
	{
		split = ft_split((*token)->content, '$');
		j = 0;
		while (split[j])
		{
			first_line = ft_strtrim(split[j], " ");
			line = ft_strtrim(first_line, "\"");
			free(first_line);
			arguments[*i] = store_vars(line, env);
			free(line);
			if (arguments[*i])
			{
				(*i)++;
			}
			j++;
		}
		j = 0;
		while (split[j])
		{
			*token = (*token)->next_token;
			j++;
		}
		if (*token)
			*token = (*token)->next_token;
		ft_free_split(split);
	}
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
int	parse_command_arguments(t_global **token, t_environment *env,
		t_rlist *redir, char **arguments)
{
	int				i;
	int				ignore_arguments;
	enum e_token	first_type;
	char			**argument;
	char			*joined;

	i = 0;
	ignore_arguments = 0;
	first_type = (*token)->type;
	if (check_redir(first_type))
	{
		if (!parse_redir(token, env, redir))
		{
			return (EXIT_FAILURE);
		}
	}
	argument = arguments;
	while (*token && (*token)->type != PIPE_LINE)
	{
		if ((*token)->type == WORD)
		{
			*argument = ft_strdup((*token)->content);
			argument++;
		}

		else if ((*token)->type == DQUOTE || (*token)->type == SQUOTE)
		{
			enum e_token	type = (*token)->type;
			*token = (*token)->next_token;
			joined = ft_strdup("");
			while ((*token)->type != type)
			{
				joined = ft_strjoin(joined, (*token)->content);
				*token = (*token)->next_token;
			}
			*argument = joined;
			argument++;
		}


		else
			handle_other_token(token, env, redir, &ignore_arguments);
		*token = (*token)->next_token;
	}
	return (EXIT_SUCCESS);
}


void	expand_all(t_global *_token, t_environment *env)
{
	t_global	*token;
	char		*old_content;

	token = _token;
	while (token)
	{
		if (token->type == ENV && ( token->token_state == DEFAULT || token->token_state == IN_DOUBLE_QUOTES))
		{
			old_content = token->content;
			token->content = store_vars(old_content + 1, env);
			token->type = WORD;
			token->size = ft_strlen(token->content);
			free(old_content);
		}
		token = token->next_token;
	}
}
