#include "minishell.h"

// // Function to parse command arguments and store them in an array
// int parse_command_arguments(t_global **token, t_environment *env, t_rlist *redir, char **arguments) {
//     int i = 0;
//     while (*token && (*token)->type != PIPE_LINE) {
//         if ((*token)->type == WHITE_SPACE) {
//             (*token) = (*token)->next_token;
//         } else if ((*token)->type == WORD) {
//             // If the token is of type WORD, copy its content as an argument
//             arguments[i] = ft_strndup((*token)->content, (*token)->size);
//             i++;
//             (*token) = (*token)->next_token;
//         } else if ((*token)->type == DOUBLE_QUOTE || (*token)->type == QUOTE) {
//             // If the token is of type DOUBLE_QUOTE or QUOTE, extract the argument within the quotes
//             char *str = NULL;
//             *token = (*token)->next_token;
//             while (*token && (*token)->type != (*token)->type) {
//                 if (!str)
//                     str = ft_strdup("");
//                 if ((*token)->type == ENV && (*token)->token_state == IN_DOUBLE_QUOTES) {
//                     // If the token is an environment variable inside double quotes, expand it
//                     arguments[i] = store_vars((*token)->content + 1, env);
//                     if (arguments[i])
//                         i++;
//                 } else {
//                     // If the token is not an environment variable, add its content to the current argument string
//                     str = ft_strjoin(str, (*token)->content);
//                 }
//                 *token = (*token)->next_token;
//             }
//             if (*token) {
//                 *token = (*token)->next_token;
//             }
//             arguments[i] = str;
//             if (str) {
//                 i++;
//             }
//         } else if ((*token)->type == ENV) {
//             // If the token is of type ENV, store the value of the environment variable as an argument
//             arguments[i] = store_vars((*token)->content + 1, env);
//             if (arguments[i])
//                 i++;
//             (*token) = (*token)->next_token;
//         } else {
//             // If the token is a redirection token, parse the redirection and store it in the redir list
//             enum e_token type = (*token)->type;
//             if (type == REDIR_IN || type == REDIR_OUT || type == DREDIR_OUT || type == HERE_DOC) {
//                 if (!parse_redir(token, env, redir)) {
//                     return EXIT_FAILURE;
//                 }
//             } else {
//                 // If it's not a valid redirection token or an unknown token, skip it
//                 (*token) = (*token)->next_token;
//             }
//         }
//     }
//     arguments[i] = NULL; // Null-terminate the arguments array
//     return EXIT_SUCCESS;
// }


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

    // Skip the initial quote token
    *token = (*token)->next_token;

    while (*token && (*token)->type != (*token)->type) {
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
        *token = (*token)->next_token;
    }

    return argument;
}

// Function to parse and store command arguments
int parse_command_arguments(t_global **token, t_environment *env, t_rlist *redir, char **arguments) {
    int i = 0;

    while (*token && (*token)->type != PIPE_LINE) {
        if ((*token)->type == WHITE_SPACE) {
            (*token) = (*token)->next_token;
        } else if ((*token)->type == WORD) {
            arguments[i] = ft_strndup((*token)->content, (*token)->size);
            i++;
            (*token) = (*token)->next_token;
        } else if ((*token)->type == DOUBLE_QUOTE || (*token)->type == QUOTE) {
            arguments[i] = parse_quoted_argument(token, env);
            if (arguments[i]) {
                i++;
            }
        } else if ((*token)->type == ENV) {
            arguments[i] = store_vars((*token)->content + 1, env);
            if (arguments[i]) {
                i++;
            }
            (*token) = (*token)->next_token;
        } else {
            enum e_token type = (*token)->type;
            if (type == REDIR_IN || type == REDIR_OUT || type == DREDIR_OUT || type == HERE_DOC) {
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
