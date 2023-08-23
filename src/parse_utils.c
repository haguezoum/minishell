#include "minishell.h"


// Function to parse and expand an environment variable inside quotes
char *expand_env_in_quotes(t_global **token, t_environment *env) {
    char *expanded_argument = NULL;

    if ((*token)->type == ENV && (*token)->token_state == IN_DOUBLE_QUOTES) {
        char *expanded = store_vars((*token)->content + 1, env);
        if (expanded) {
            expanded_argument = ft_strdup(expanded);
            free(expanded);
        }
        (*token) = (*token)->next_token;
    }

    return expanded_argument;
}

// Function to parse a quoted argument and expand environment variables
char *parse_quoted_argument(t_global **token, t_environment *env) {
    char *argument = NULL;

    // Get the type of the opening quote token (single or double)
    enum e_token open_quote_type = (*token)->type;

    // Skip the initial quote token
    *token = (*token)->next_token;

    while (*token && (*token)->type != open_quote_type) {
        if (!argument)
            argument = ft_strdup("");

        char *expanded = expand_env_in_quotes(token, env);
        if (expanded) {
            argument = ft_strjoin(argument, expanded);
            free(expanded);
        } else {
            // If the token is not an environment variable, add its content to the current argument string
            argument = ft_strjoin(argument, (*token)->content);
            *token = (*token)->next_token;
        }
    }

    if (*token) {
        *token = (*token)->next_token; // Skip the closing quote token
    }

    return argument;
}

int parse_command_arguments(t_global **token, t_environment *env, t_rlist *redir, char **arguments) {
    int i = 0;
    int ignore_arguments = 0;

    // Check if the first token is a redirection token
    enum e_token first_type = (*token)->type;
    if (first_type == REDIR_IN || first_type == REDIR_OUT || first_type == DREDIR_OUT || first_type == HERE_DOC) {
        if (!parse_redir(token, env, redir)) {
            return EXIT_FAILURE;
        }
    }

    while (*token && (*token)->type != PIPE_LINE) {
        if ((*token)->type == WHITE_SPACE) {
            (*token) = (*token)->next_token;
        } else if ((*token)->type == WORD) {
            if (!ignore_arguments) {
                arguments[i] = ft_strndup((*token)->content, (*token)->size);
                i++;
            }
            (*token) = (*token)->next_token;
        } else if ((*token)->type == DOUBLE_QUOTE || (*token)->type == QUOTE) {
            if (!ignore_arguments) {
                arguments[i] = parse_quoted_argument(token, env);
                if (arguments[i]) {
                    i++;
                }
            } else {
                *token = (*token)->next_token;
            }
        } else if ((*token)->type == ENV) {
            if (!ignore_arguments) {
                arguments[i] = store_vars((*token)->content + 1, env);
                if (arguments[i]) {
                    i++;
                }
            }
            (*token) = (*token)->next_token;
        } else {
            enum e_token type = (*token)->type;
            if (type == REDIR_IN || type == REDIR_OUT || type == DREDIR_OUT || type == HERE_DOC) {
                // Skip the redirection tokens and any following tokens until PIPE_LINE or ENV/WORD token
                ignore_arguments = 1;
                if (!parse_redir(token, env, redir)) {
                    return EXIT_FAILURE;
                }
            } else {
                (*token) = (*token)->next_token;
            }
        }
    }

    arguments[i] = NULL; // Null-terminate the arguments array
    return EXIT_SUCCESS;
}