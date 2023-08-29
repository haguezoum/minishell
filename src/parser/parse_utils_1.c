#include "../includes/minishell.h"


void remove_double_quotes(char *input) {
    char *src = input;
    char *dst = input;

    while (*src) {
        if (*src != '\"') {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0';
}

char *expand_env_in_quotes(t_global **token, t_environment *env) {
    char *expanded_argument = NULL;

    if ((*token)->type == ENV && (*token)->token_state == IN_DOUBLE_QUOTES) {
        remove_double_quotes((*token)->content);
        char *expanded = store_vars((*token)->content + 1, env);
        if (expanded) {
            expanded_argument = expanded;
            (*token) = (*token)->next_token;
        }
    }

    return expanded_argument;
}


char *join_content(char *argument, char *content, int size) {
    char *tmp = ft_strndup(content, size);
    char *result = ft_strjoin(argument, tmp);
    free(tmp);
    return result;
}



char *parse_quoted_argument(t_global **token, t_environment *env) {
    char *argument = NULL;

    enum e_token open_quote_type = (*token)->type;

    *token = (*token)->next_token;

    while (*token && (*token)->type != open_quote_type) {
        if (!argument)
            argument = ft_strdup("");

        if ((*token)->type == ENV && (*token)->token_state == IN_DOUBLE_QUOTES) {
            char *expanded = expand_env_in_quotes(token, env);
            if (expanded) {
                argument = join_content(argument, expanded, strlen(expanded));
                free(expanded);
            } else {
                argument = join_content(argument, (*token)->content, (*token)->size);
            }
        } else {
            argument = join_content(argument, (*token)->content, (*token)->size);
        }

        *token = (*token)->next_token;
    }

    if (*token) {
        *token = (*token)->next_token; // Skip the closing quote token
    }

    return argument;
}

