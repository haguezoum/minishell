/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 22:01:20 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/09 22:07:25 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

t_global *check_unclosed_quotes(t_global **current_token, enum e_token type) {
    while (*current_token) {
        *current_token = (*current_token)->next_token;
        if (!*current_token || (*current_token)->type == type)
            break;
    }
    if (!*current_token)
        write(STDERR_FILENO, "minishell: unclosed quotes detected.\n", ft_strlen("minishell: unclosed quotes detected.\n"));
    return *current_token;
}

int process_operator(t_global **current_token, int *has_operator, t_global **prev_word) {
    t_global *next_token = skip_whitespace((*current_token)->next_token, 1);
    if (!next_token || !is_valid_word(next_token->type))
        return syntax_error("unexpected token after redirection");

    *has_operator = 1;
    *current_token = next_token;
    return EXIT_SUCCESS;
}

int process_token(t_global **current_token, int *has_operator, int *has_command, t_global **prev_word) {
    if (is_valid_word((*current_token)->type)) {
        *prev_word = *current_token;
        *has_operator = 0;
        *has_command = 1;
    }

    *current_token = (*current_token)->next_token;
    return EXIT_SUCCESS;
}



int handle_pipe_line(t_global **current_token, int *has_operator, int *has_command) {
    if (*has_operator || !*has_command)
        return syntax_error("unexpected pipe `|'");
    *has_operator = 1;
    return EXIT_SUCCESS;
}

int handle_operator_case(t_global **current_token, int *has_operator, t_global **prev_word) {
    if (!*prev_word)
        return syntax_error("unexpected token before redirection");
    int result = process_operator(current_token, has_operator, prev_word);
    if (result != EXIT_SUCCESS)
        return result;
    return EXIT_SUCCESS;
}
