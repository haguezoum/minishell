#include "minishell.h"

t_global *skip_spaces(t_global *ptr, int opt) {
    while (ptr && ptr->type == WHITE_SPACE) {
        if (opt)
            ptr = ptr->next_token;
        else
            ptr = ptr->prev_token;
    }
    return ptr;
}

int is_operator(enum e_token type) {
    return (type == PIPE || type == REDIR_IN || type == REDIR_OUT || type == DREDIR_OUT);
}

int is_valid_word(enum e_token type) {
    return (type == WORD || type == ENV);
}

int is_valid_char(enum e_token type) {
    return (type == WHITE_SPACE || type == NEW_LINE || type == QUOTE || type == DOUBLE_QUOTE ||
            type == ESCAPE || is_operator(type) || is_valid_word(type));
}

int syntax_error(const char *msg) {
    write(STDERR_FILENO, "minishell: syntax error: ", ft_strlen("minishell: syntax error: "));
    write(STDERR_FILENO, msg, ft_strlen(msg));
    write(STDERR_FILENO, "\n", 1);
    return EXIT_FAILURE;
}

int pipe_error(t_global *ptr) {
    t_global *prev = skip_spaces(ptr->prev_token, 0);
    t_global *next = skip_spaces(ptr->next_token, 1);

    if ((!prev || !next) || (prev->type != WORD && next->type != WORD && !is_operator(next->type)))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int redir_error(t_global *ptr) {
    t_global *next = skip_spaces(ptr->next_token, 1);

    if (!next || (next->type != WORD && next->type != ENV))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


t_global *check_unclosed_quotes(t_global **ptr, enum e_token type) {
    while (*ptr) {
        *ptr = (*ptr)->next_token;
        if (!*ptr || (*ptr)->type == type)
            break;
    }
    if (!*ptr)
        write(STDERR_FILENO, "minishell: unclosed quotes detected.\n", ft_strlen("minishell: unclosed quotes detected.\n"));
    return *ptr;
}

int check_syntax(t_lexer *lexer) {
    t_global *ptr = lexer->head;
    t_global *prev_word = NULL;
    int has_operator = 0;
    int has_command = 0;

    while (ptr) {
        if (ptr->type == PIPE) {
            if (has_operator || !has_command)
                return syntax_error("unexpected token `|'");
            has_operator = 1;
        } else if (is_operator(ptr->type)) {
            if (!prev_word)
                return syntax_error("unexpected token before redirection");
            t_global *next_token = skip_spaces(ptr->next_token, 1);
            if (!next_token || !is_valid_word(next_token->type))
                return syntax_error("unexpected token after redirection");
            has_operator = 1;
            ptr = next_token;
        } else if (ptr->type == NEW_LINE) {
            if (!prev_word && !has_operator)
                return syntax_error("unexpected token `newline'");
            has_operator = 0;
            has_command = 0;
        } else if (ptr->type == DOUBLE_QUOTE || ptr->type == QUOTE) {
            t_global *quote = check_unclosed_quotes(&ptr, ptr->type);
            if (!quote)
                return EXIT_FAILURE;
        } else if (ptr->type == ESCAPE) {
            if (!ptr->next_token)
                return syntax_error("unexpected token `escape'");
            ptr = ptr->next_token; // Skip the escaped character
        } else if (!is_valid_char(ptr->type)) {
            return syntax_error("unexpected token");
        }

        if (is_valid_word(ptr->type)) {
            prev_word = ptr;
            has_operator = 0;
            has_command = 1;
        }

        ptr = ptr->next_token;
    }

    if (has_operator || !has_command)
        return syntax_error("unexpected end of command");

    return EXIT_SUCCESS;
}

