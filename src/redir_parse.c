#include "minishell.h"

// Function to parse redirections and store them in a linked list
t_rlist *parse_redir(t_global **token, t_environment *env, t_rlist *redir_list) {
    // Store the type of the current redirection token
    enum e_token redir_type = (*token)->type;
    
    // Skip tokens until a WORD or ENV token is found
    while ((*token)->type != WORD && (*token)->type != ENV)
        *token = (*token)->next_token;
    
    char *redir_argument;
    t_relem *redir_element;

    // If the current token is of type ENV and the redirection type is not HERE_DOC
    if ((*token)->type == ENV && redir_type != HERE_DOC) {
        // Get the value of the environment variable (skipping the '$' character)
        redir_argument = store_vars((*token)->content + 1, env);
        
        // Check if the environment variable exists, if not, print an error message and return NULL
        if (!redir_argument) {
            printf("bash: %s: ambiguous redirect\n", (*token)->content);
            return NULL;
        }
    } else {
        // If the token is of type WORD or the redirection type is HERE_DOC, simply copy the token's content as the argument
        redir_argument = ft_strdup((*token)->content);
    }
    
    // Move to the next token in the global token list
    *token = (*token)->next_token;

    // If the redirection type is HERE_DOC
    if (redir_type == HERE_DOC) {
        // Open a temporary file to store the contents of the here-document
        int fd = open("/tmp/.minishell_tmp", O_RDWR | O_CREAT | O_APPEND | O_TRUNC, 0666);
        
        // Read lines from the standard input until a line matching the here-document is encountered
        while (1) {
            char *line = get_next_line(STDIN_FILENO);
            if (line == NULL || ft_strncmp(line, redir_argument, ft_strlen(redir_argument)) == 0) {
                free(line);
                break;
            } else {
                // If the line contains environment variables, expand them before writing to the temporary file
                if (ft_strchr(line, '$') != NULL) {
                    char *expanded_line = expand_vars(line, env->environment_array);
                    free(line);
                    line = expanded_line;
                }
                write(fd, line, ft_strlen(line));
                free(line);
            }
        }
        free(redir_argument);

        // Create a new t_relem element to store the here-document redirection details
        redir_element = ft_calloc(1, sizeof(t_relem));
        if (!redir_element)
            return NULL;

        // Set the argument as the path to the temporary file and the type as HERE_DOC
        redir_element->argument = ft_strdup("/tmp/.minishell_tmp");
        redir_element->type = HERE_DOC;
    } else {
        // For other redirection types, create a new t_relem element
        redir_element = ft_calloc(1, sizeof(t_relem));
        if (!redir_element) {
            free(redir_argument);
            return NULL;
        }
        // Set the argument and type based on the token and redirection type
        redir_element->argument = redir_argument;
        redir_element->type = redir_type;
    }

    // Add the new t_relem element to the redir_list linked list
    if (!redir_list->first) {
        redir_list->first = redir_element;
    } else {
        redir_list->last->next = redir_element;
    }
    redir_list->last = redir_element;
    redir_list->total++;

    // Return the updated redir_list
    return redir_list;
}

