/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 18:54:56 by aet-tass          #+#    #+#             */
/*   Updated: 2023/08/29 19:33:41 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int is_env_var(t_global *token) {
    return token->type == ENV;
}

void skip_to_word_or_env(t_global **token) {
    while ((*token)->type != WORD && !is_env_var(*token))
        *token = (*token)->next_token;
}

char *get_env_var_value(char *var_name, t_environment *env) {
    return store_vars(var_name + 1, env);
}

int is_whitespace_tokene(t_global *token) {
    return token->type == WHITE_SPACE;
}

void skip_whitespace_tokene(t_global **token) {
    while (*token && is_whitespace_tokene(*token)) {
        *token = (*token)->next_token;
    }
}
