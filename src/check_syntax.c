#include "./includes/minishell.h"

// Function to skip over white spaces in the token list in a given direction.
// Parameters:
//   - current_token: A pointer to the current token in the list.
//   - direction: The direction to skip white spaces (1 for forward, 0 for backward).
// Returns: A pointer to the next non-white space token in the specified direction.
t_global *skip_whitespace(t_global *current_token, int direction) {
    while (current_token && current_token->type == WHITE_SPACE) {
        if (direction)
            current_token = current_token->next_token;
        else
            current_token = current_token->prev_token;
    }
    return current_token;
}

// Function to check if a token type is an operator (e.g., pipe, redirection).
// Parameter:
//   - type: The token type to check.
// Returns: 1 if the token type is an operator, 0 otherwise.
int is_operator(enum e_token type) {
    return (type == PIPE_LINE || type == REDIR_IN || type == REDIR_OUT || type == DREDIR_OUT || type == HERE_DOC);
}

// Function to check if a token type is a valid word (e.g., word or environment variable).
// Parameter:
//   - type: The token type to check.
// Returns: 1 if the token type is a valid word, 0 otherwise.
int is_valid_word(enum e_token type) {
    return (type == WORD || type == ENV);
}

// Function to check if a token type is a valid character (e.g., white space, quote, escape).
// Parameter:
//   - type: The token type to check.
// Returns: 1 if the token type is a valid character, 0 otherwise.
int is_valid_char(enum e_token type) {
    return (type == WHITE_SPACE || type == NEW_LINE || type == SQUOTE || type == DQUOTE ||
            type == ESCAPE || is_operator(type) || is_valid_word(type));
}

// Function to display a syntax error message to standard error.
// Parameter:
//   - msg: The error message to display.
// Returns: EXIT_FAILURE to indicate an error.
int syntax_error(const char *msg) {
    write(STDERR_FILENO, "minishell: syntax error: ", ft_strlen("minishell: syntax error: "));
    write(STDERR_FILENO, msg, ft_strlen(msg));
    write(STDERR_FILENO, "\n", 1);
    return EXIT_FAILURE;
}

// Function to check for syntax errors related to the pipe operator.
// Parameter:
//   - current_token: A pointer to the current token in the token list.
// Returns: EXIT_SUCCESS if there are no syntax errors, EXIT_FAILURE otherwise.
int check_pipe_error(t_global *current_token) {
    t_global *prev_token = skip_whitespace(current_token->prev_token, 0);
    t_global *next_token = skip_whitespace(current_token->next_token, 1);

    // Check if the pipe operator is surrounded by valid words or other operators.
    if ((!prev_token || !next_token) || (prev_token->type != WORD && next_token->type != WORD && !is_operator(next_token->type)))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

// Function to check for syntax errors related to redirection operators.
// Parameter:
//   - current_token: A pointer to the current token in the token list.
// Returns: EXIT_SUCCESS if there are no syntax errors, EXIT_FAILURE otherwise.
int check_redirection_error(t_global *current_token) {
    t_global *next_token = skip_whitespace(current_token->next_token, 1);

    // Check if the redirection operator is followed by a valid word or environment variable.
    if (!next_token || (next_token->type != WORD && next_token->type != ENV))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

// Function to check for unclosed quotes (single or double) in the token list.
// Parameters:
//   - current_token: A pointer to the current token in the token list.
//   - type: The type of quote to check for (QUOTE or DOUBLE_QUOTE).
// Returns: A pointer to the closing quote token if found, NULL otherwise.
t_global *check_unclosed_quotes(t_global **current_token, enum e_token type) {
    while (*current_token) {
        *current_token = (*current_token)->next_token;
        // Check if the closing quote is found or if the end of the token list is reached.
        if (!*current_token || (*current_token)->type == type)
            break;
    }
    // If the end of the token list is reached without finding the closing quote, display an error message.
    if (!*current_token)
        write(STDERR_FILENO, "minishell: unclosed quotes detected.\n", ft_strlen("minishell: unclosed quotes detected.\n"));
    return *current_token;
}

// Function to check the syntax of the entire command in the token list.
// Parameter:
//   - lexer: A pointer to the lexer object containing the token list.
// Returns: EXIT_SUCCESS if there are no syntax errors, EXIT_FAILURE otherwise.
int check_command_syntax(t_lexer *lexer) {
    t_global *current_token = lexer->head;
    t_global *prev_word = NULL;
    int has_operator = 0;
    int has_command = 0;

    while (current_token) {
        if (current_token->type == PIPE_LINE) {
            // Check for consecutive pipes or pipes at the beginning of a command.
            if (has_operator || !has_command)
                return syntax_error("unexpected pipe `|'");
            has_operator = 1;
        } else if (is_operator(current_token->type)) {
            // Check for redirection errors and set the appropriate flags.
            if (!prev_word)
                return syntax_error("unexpected token before redirection");
            t_global *next_token = skip_whitespace(current_token->next_token, 1);
            if (!next_token || !is_valid_word(next_token->type))
                return syntax_error("unexpected token after redirection");
            has_operator = 1;
            current_token = next_token;
        } else if (current_token->type == NEW_LINE) {
            // Check for invalid newlines or newlines at the end of a command.
            if (!prev_word && !has_operator)
                return syntax_error("unexpected token `newline'");
            has_operator = 0;
            has_command = 0;
        } else if (current_token->type == DQUOTE || current_token->type == SQUOTE) {
            // Check for unclosed quotes and update the current token accordingly.
            t_global *quote = check_unclosed_quotes(&current_token, current_token->type);
            if (!quote)
                return EXIT_FAILURE;
        } else if (current_token->type == ESCAPE) {
            // Skip the escaped character.
            if (!current_token->next_token)
                return syntax_error("unexpected token `escape'");
            current_token = current_token->next_token;
        } else if (!is_valid_char(current_token->type)) {
            // Check for invalid characters.
            return syntax_error("unexpected token");
        }

        if (is_valid_word(current_token->type)) {
            // Update the previous word pointer and set the appropriate flags.
            prev_word = current_token;
            has_operator = 0;
            has_command = 1;
        }

        current_token = current_token->next_token;
    }

    // Check for missing commands or trailing operators.
    if (has_operator || !has_command)
        return syntax_error("unexpected end of command");

    return EXIT_SUCCESS;
}

