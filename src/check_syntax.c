#include "./includes/minishell.h"

t_global *skip_whitespace(t_global *current_token, int direction) {
    while (current_token && current_token->type == WHITE_SPACE) {
        if (direction)
            current_token = current_token->next_token;
        else
            current_token = current_token->prev_token;
    }
    return current_token;
}


int is_operator(enum e_token type) {
    return (type == PIPE_LINE || type == REDIR_IN || type == REDIR_OUT || type == DREDIR_OUT || type == HERE_DOC);
}


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
