/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 18:56:24 by aet-tass          #+#    #+#             */
/*   Updated: 2023/08/29 19:34:43 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void handle_regular_argument(t_global **token, char **arguments, int *i, int *ignore_arguments) {
    if (!(*ignore_arguments)) {
        arguments[*i] = ft_strndup((*token)->content, (*token)->size);
        (*i)++;
    }
    *token = (*token)->next_token;
}

void handle_quoted_argument(t_global **token, t_environment *env, char **arguments, int *i, int *ignore_arguments) {
    if (!(*ignore_arguments)) {
        arguments[*i] = parse_quoted_argument(token, env);
        if (arguments[*i]) {
            (*i)++;
        }
    } else {
        *token = (*token)->next_token;
    }
}

void handle_env_argument(t_global **token, t_environment *env, char **arguments, int *i, int *ignore_arguments) {
    if (!(*ignore_arguments)) {
        arguments[*i] = store_vars((*token)->content + 1, env);
        if (arguments[*i]) {
            (*i)++;
        }
    }
    *token = (*token)->next_token;
}

void handle_other_token(t_global **token, t_environment *env, t_rlist *redir, int *ignore_arguments) {
    enum e_token type = (*token)->type;
    if (check_redir(type)) {
        *ignore_arguments = 1;
        if (!parse_redir(token, env, redir)) {
            return;
        }
    } else {
        *token = (*token)->next_token;
    }
}

int parse_command_arguments(t_global **token, t_environment *env, t_rlist *redir, char **arguments) {
    int i = 0;
    int ignore_arguments = 0;

    enum e_token first_type = (*token)->type;
    if (check_redir(first_type)) {
        if (!parse_redir(token, env, redir)) {
            return EXIT_FAILURE;
        }
    }

    while (*token && (*token)->type != PIPE_LINE) {
        if (is_whitespace_tokene(*token)) {
            skip_whitespace_tokene(token);
        } else if ((*token)->type == WORD) {
            handle_regular_argument(token, arguments, &i, &ignore_arguments);
        } else if ((*token)->type == DQUOTE || (*token)->type == SQUOTE) {
            handle_quoted_argument(token, env, arguments, &i, &ignore_arguments);
        } else if ((*token)->type == ENV) {
            handle_env_argument(token, env, arguments, &i, &ignore_arguments);
        } else {
            handle_other_token(token, env, redir, &ignore_arguments);
        }
    }
    arguments[i] = NULL;
    return EXIT_SUCCESS;
}

