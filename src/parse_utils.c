#include "minishell.h"

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
            char *str = NULL;
            *token = (*token)->next_token;
            while (*token && (*token)->type != (*token)->type) {
                if (!str)
                    str = ft_strdup("");
                if ((*token)->type == ENV && (*token)->token_state == IN_DOUBLE_QUOTES) {
                    arguments[i] = store_vars((*token)->content + 1, env);
                    if (arguments[i])
                        i++;
                } else {
                    str = ft_strjoin(str, (*token)->content);
                }
                *token = (*token)->next_token;
            }
            if (*token) {
                *token = (*token)->next_token;
            }
            arguments[i] = str;
            if (str) {
                i++;
            }
        } else if ((*token)->type == ENV) {
            arguments[i] = store_vars((*token)->content + 1, env);
            if (arguments[i])
                i++;
            (*token) = (*token)->next_token;
        } else {
            enum e_token type = (*token)->type;
            if (type == REDIR_IN || type == REDIR_OUT || type == DREDIR_OUT || type == HERE_DOC) {
                if (!parse_redir(token, env, redir)) {
                    return EXIT_FAILURE;
                }
            } else {
                // If it's not a valid redirection token or an unknown token, skip it
                (*token) = (*token)->next_token;
            }
        }
    }
    arguments[i] = NULL;
    return EXIT_SUCCESS;
}

t_node *build_command_tree(t_global **token, t_environment *env) {
    t_node *new_node;
    char **arguments;
    t_rlist *redir_list;
    int argument_size;
    int arg_index;

    // Initialize the redirection list
    redir_list = calloc(1, sizeof(t_rlist));
    if (!redir_list)
        return NULL;

    // Count the number of arguments
    t_global *tmp = *token;
    arg_index = 0;
    while (tmp && tmp->type != PIPE_LINE) {
        if (tmp->type == WORD || tmp->type == ENV) {
            arg_index++;
        } else if (tmp->type == DOUBLE_QUOTE || tmp->type == QUOTE ||
                   tmp->type == REDIR_IN || tmp->type == REDIR_OUT ||
                   tmp->type == DREDIR_OUT || tmp->type == HERE_DOC) {
            if (tmp->type == DOUBLE_QUOTE || tmp->type == QUOTE) {
                tmp = tmp->next_token;
                while (tmp && tmp->type != tmp->type)
                    tmp = tmp->next_token;
            } else {
                while (tmp && tmp->type != WORD && tmp->type != ENV)
                    tmp = tmp->next_token;
                tmp = tmp->next_token;
            }
            arg_index++;
        }

        if (tmp)
            tmp = tmp->next_token;
    }

    argument_size = arg_index;
    arguments = calloc(sizeof(char *), (argument_size + 1));
    if (!arguments) {
        free_redir_list(redir_list);
        return NULL;
    }

    if (parse_command_arguments(token, env, redir_list, arguments) == EXIT_FAILURE) {
        free_redir_list(redir_list);
        free(arguments);
        return NULL;
    }

    new_node = init_node(arguments, env->environment_array, redir_list);
    if (!new_node) {
        free_redir_list(redir_list);
        free(arguments);
    }

    return new_node;
}


