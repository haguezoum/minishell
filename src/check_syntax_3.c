/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 22:01:59 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/09 22:02:32 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"


int is_valid_word(enum e_token type) {
    return (type == WORD || type == ENV);
}

int is_valid_char(enum e_token type) {
    return (type == WHITE_SPACE || type == NEW_LINE || type == SQUOTE || type == DQUOTE ||
            type == ESCAPE || is_operator(type) || is_valid_word(type));
}


int syntax_error(const char *msg) {
    write(STDERR_FILENO, "minishell: syntax error: ", ft_strlen("minishell: syntax error: "));
    write(STDERR_FILENO, msg, ft_strlen(msg));
    write(STDERR_FILENO, "\n", 1);
    return EXIT_FAILURE;
}
int check_pipe_error(t_global *current_token) {
    t_global *prev_token = skip_whitespace(current_token->prev_token, 0);
    t_global *next_token = skip_whitespace(current_token->next_token, 1);

    if ((!prev_token || !next_token) || (prev_token->type != WORD && next_token->type != WORD && !is_operator(next_token->type)))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


int check_redirection_error(t_global *current_token) {
    t_global *next_token = skip_whitespace(current_token->next_token, 1);

    if (!next_token || (next_token->type != WORD && next_token->type != ENV))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
