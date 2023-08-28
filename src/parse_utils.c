#include "./includes/minishell.h"


// // Function to parse and expand an environment variable inside quotes
// char *expand_env_in_quotes(t_global **token, t_environment *env) {
//     char *expanded_argument = NULL;

//     if ((*token)->type == ENV && (*token)->token_state == IN_DOUBLE_QUOTES) {
//         char *expanded = store_vars((*token)->content + 1, env);
//         if (expanded) {
//             expanded_argument = ft_strdup(expanded);
//             free(expanded);
//         }
//         (*token) = (*token)->next_token;
//     }

//     return expanded_argument;
// }


// Function to parse and expand an environment variable inside double quotes
char *expand_env_in_quotes(t_global **token, t_environment *env) {
    char *expanded_argument = NULL;

    if ((*token)->type == ENV && (*token)->token_state == IN_DOUBLE_QUOTES) {
        char *expanded = store_vars((*token)->content + 1, env);
        if (expanded) {
            expanded_argument = expanded;
            (*token) = (*token)->next_token;
        }
    }

    return expanded_argument;
}


// char * join_content(char *argument, char *content, int size) { // join the content of the token to the argument
//     char *tmp = ft_strndup(content, size);
//     char *tmp2 = ft_strjoin(argument, tmp);
//     free(tmp);
//     free(argument);
//     return tmp2;
// }
char *join_content(char *argument, char *content, int size) {
    char *tmp = ft_strndup(content, size);
    char *result = ft_strjoin(argument, tmp);
    free(tmp);
    // Don't free(argument) here, as you still need to return it
    return result;
}

// // Function to parse a quoted argument and expand environment variables
// char *parse_quoted_argument(t_global **token, t_environment *env) {
//     char *argument = NULL;

//     // Get the type of the opening quote token (single or double)
//     enum e_token open_quote_type = (*token)->type;

//     // Skip the initial quote token
//     *token = (*token)->next_token;

//     while (*token && (*token)->type != open_quote_type) {
//         if (!argument)
//             argument = ft_strdup("");

//         char *expanded = expand_env_in_quotes(token, env); // Expand environment variables inside quotes
//         if (expanded) {
//             argument = ft_strjoin(argument, expanded);
//             free(expanded);
//         } else {
//             // If the token is not an environment variable, add its content to the current argument string
//             argument = join_content(argument, (*token)->content, (*token)->size); // join the content of the token to the argument
//             *token = (*token)->next_token;
//         }
//     }

//     if (*token) {
//         *token = (*token)->next_token; // Skip the closing quote token
//     }

//     return argument;
// }

// Function to parse a quoted argument and expand environment variables
// char *parse_quoted_argument(t_global **token, t_environment *env) {
//     char *argument = NULL;

//     // Get the type of the opening quote token (single or double)
//     enum e_token open_quote_type = (*token)->type;

//     // Skip the initial quote token
//     *token = (*token)->next_token;

//     while (*token && (*token)->type != open_quote_type) {
//         if (!argument)
//             argument = ft_strdup("");

//         if ((*token)->type == ENV && (*token)->token_state == IN_DOUBLE_QUOTES) {
//             char *expanded = store_vars((*token)->content + 1, env);
//             if (expanded) {
//                 argument = join_content(argument, expanded, strlen(expanded)); // join the content of the token to the argument
//                 free(expanded);
//             } else {
//                 argument = join_content(argument, (*token)->content, (*token)->size); // join the content of the token to the argument
//             }
//         } else {
//             argument = join_content(argument, (*token)->content, (*token)->size); // join the content of the token to the argument
//         }

//         *token = (*token)->next_token;
//     }

//     if (*token) {
//         *token = (*token)->next_token; // Skip the closing quote token
//     }

//     return argument;
// }
void handel_dollar(t_global **token, t_environment *env, char **argument) {
    char *expanded = store_vars((*token)->content + 1, env);
    if (expanded) {
        *argument = ft_strjoin(*argument, expanded);
        free(expanded);
    } else {
        // If the token is not an environment variable, add its content to the current argument string
        *argument = join_content(*argument, (*token)->content, (*token)->size); // join the content of the token to the argument
        *token = (*token)->next_token;
    }
}

char *parse_quoted_argument(t_global **token, t_environment *env) {
    char *argument = NULL;

    // Get the type of the opening quote token (single or double)
    enum e_token open_quote_type = (*token)->type;

    // Skip the initial quote token
    *token = (*token)->next_token;

    while (*token && (*token)->type != open_quote_type) {
        if (!argument)
            argument = ft_strdup("");

        if ((*token)->type == ENV && (*token)->token_state == IN_DOUBLE_QUOTES) {
            handel_dollar(token, env, &argument); // Use the handel_dollar function to handle dollar sign expansion
        } else {
            argument = join_content(argument, (*token)->content, (*token)->size); // join the content of the token to the argument
        }

        *token = (*token)->next_token;
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
        } else if ((*token)->type == DQUOTE || (*token)->type == SQUOTE) {
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
