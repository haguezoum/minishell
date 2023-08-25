#include "minishell.h"



// Function to check if the token is of type ENV
int is_env_var(t_global *token) {
    return token->type == ENV;
}

// Function to skip tokens until a WORD or ENV token is found
void skip_to_word_or_env(t_global **token) {
    while ((*token)->type != WORD && !is_env_var(*token))
        *token = (*token)->next_token;
}

// Function to get the value of an environment variable
char *get_env_var_value(char *var_name, t_environment *env) {
    return store_vars(var_name + 1, env);
}

// Function to read lines from standard input until a matching line is encountered
char *read_until_match(char *match, t_environment *env) {
    char *line = NULL;
    int fd = open("/tmp/.minishell_tmp", O_RDWR | O_CREAT | O_APPEND | O_TRUNC, 0666);

    while (1) {
        line = get_next_line(STDIN_FILENO);
        if (line == NULL || ft_strncmp(line, match, ft_strlen(match)) == 0) {
            free(line);
            break;
        } else {
            if (ft_strchr(line, '$') != NULL) {
                char *expanded_line = expand_vars(line, env->environment_array);
                free(line);
                line = expanded_line;
            }
            write(fd, line, ft_strlen(line));
            free(line);
        }
    }

    return ft_strdup("/tmp/.minishell_tmp");
}

// Function to create a redirection element for HERE_DOC type
t_relem *create_here_doc_element(char *match, t_environment *env) {
    char *path = read_until_match(match, env);
    t_relem *element = ft_calloc(1, sizeof(t_relem));
    if (!element) {
        free(path);
        return NULL;
    }
    element->argument = path;
    element->type = HERE_DOC;
    return element;
}

// Function to create a redirection element for non-HERE_DOC type
t_relem *create_non_here_doc_element(enum e_token redir_type, char *argument) {
    t_relem *element = ft_calloc(1, sizeof(t_relem));
    if (!element) {
        free(argument);
        return NULL;
    }
    element->argument = argument;
    element->type = redir_type;
    return element;
}

// Function to add a redirection element to the redir_list
t_rlist *add_redir_element(t_rlist *redir_list, t_relem *redir_element) {
    if (!redir_list->first) {
        redir_list->first = redir_element;
    } else {
        redir_list->last->next = redir_element;
    }
    redir_list->last = redir_element;
    redir_list->total++;
    return redir_list;
}


char *parse_redir_argument_value(t_global **token, t_environment *env, enum e_token redir_type) {
    char *redir_argument = NULL;

    if (is_env_var(*token) && redir_type != HERE_DOC) {
        redir_argument = get_env_var_value((*token)->content, env);
        if (!redir_argument) {
            printf("minishell: %s: ambiguous redirect\n", (*token)->content);
            return NULL;
        }
    } else {
        redir_argument = ft_strdup((*token)->content);
    }

    char **args = ft_split(redir_argument, ' '); // Split the redir_argument into an array of strings
    char *first_arg = NULL;

    if (args && args[0]) {
        first_arg = ft_strdup(args[0]);
    }

    // Free the memory used by the array of strings
    for (int i = 0; args && args[i]; i++) {
        free(args[i]);
    }
    free(args);

    free(redir_argument); // Free the original redir_argument

    return first_arg;
}



t_relem *create_redir_element(enum e_token redir_type, char *redir_argument) {
    t_relem *element = ft_calloc(1, sizeof(t_relem));
    if (!element) {
        free(redir_argument);
        return NULL;
    }
    element->argument = redir_argument;
    element->type = redir_type;
    return element;
}

t_rlist *parse_redir(t_global **token, t_environment *env, t_rlist *redir_list) {
    // Get the type of the current redirection token
    enum e_token redir_type = (*token)->type;

    // Skip tokens until a WORD or ENV token is found
    skip_to_word_or_env(token);

    // Parse the redirection argument value
    char *redir_argument_value = parse_redir_argument_value(token, env, redir_type);
    if (!redir_argument_value)
        return NULL;

    // Create a redirection element based on the type
    t_relem *redir_element = create_redir_element(redir_type, redir_argument_value);
    if (!redir_element) {
        return NULL;
    }

    // Add the redirection element to the redir_list
    return add_redir_element(redir_list, redir_element);
}


