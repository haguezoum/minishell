#include "./includes/minishell.h"


int handle_new_line(t_global **current_token, int *has_operator, int *has_command, t_global **prev_word) {
    if (!*prev_word && !*has_operator)
        return syntax_error("unexpected token `newline'");
    *has_operator = 0;
    *has_command = 0;
    return EXIT_SUCCESS;
}


int handle_other_cases(t_global **current_token) {
    if ((*current_token)->type == DQUOTE || (*current_token)->type == SQUOTE) {
        t_global *quote = check_unclosed_quotes(current_token, (*current_token)->type);
        if (!quote)
            return EXIT_FAILURE;
    } else if ((*current_token)->type == ESCAPE) {
        if (!(*current_token)->next_token)
            return syntax_error("unexpected token `escape'");
        *current_token = (*current_token)->next_token;
    } else if (!is_valid_char((*current_token)->type)) {
        return syntax_error("unexpected token");
    }
    return EXIT_SUCCESS;
}

int handle_token_case(t_global **current_token, int *has_operator, int *has_command, t_global **prev_word) {
    if ((*current_token)->type == PIPE_LINE) {
        int result = handle_pipe_line(current_token, has_operator, has_command);
        if (result != EXIT_SUCCESS)
            return result;
    } else if (is_operator((*current_token)->type)) {
        int result = handle_operator_case(current_token, has_operator, prev_word);
        if (result != EXIT_SUCCESS)
            return result;
    } else if ((*current_token)->type == NEW_LINE) {
        int result = handle_new_line(current_token, has_operator, has_command, prev_word);
        if (result != EXIT_SUCCESS)
            return result;
    } else {
        int result = handle_other_cases(current_token);
        if (result != EXIT_SUCCESS)
            return result;
    }

    return EXIT_SUCCESS;
}

int parse_token(t_global **current_token, int *has_operator, int *has_command, t_global **prev_word) {
    int result = handle_token_case(current_token, has_operator, has_command, prev_word);
    if (result != EXIT_SUCCESS)
        return result;

    result = process_token(current_token, has_operator, has_command, prev_word);
    if (result != EXIT_SUCCESS)
        return result;

    return EXIT_SUCCESS;
}

int check_command_syntax(t_lexer *lexer) {
    t_global *current_token = lexer->head;
    t_global *prev_word = NULL;
    int has_operator = 0;
    int has_command = 0;

    while (current_token) {
        int result = parse_token(&current_token, &has_operator, &has_command, &prev_word);
        if (result != EXIT_SUCCESS)
            return result;
    }

    if (has_operator || !has_command)
        return syntax_error("unexpected end of command");

    return EXIT_SUCCESS;
}
