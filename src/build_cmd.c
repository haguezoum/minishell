#include "./includes/minishell.h"

// Function to build the command tree (AST) based on the token list
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

    // Count the number of arguments in the token list
    t_global *tmp = *token;
    arg_index = 0;
    while (tmp && tmp->type != PIPE_LINE) {
        if (tmp->type == WORD || tmp->type == ENV) {
            arg_index++;
        } else if (tmp->type == DQUOTE || tmp->type == SQUOTE ||
                   tmp->type == REDIR_IN || tmp->type == REDIR_OUT ||
                   tmp->type == DREDIR_OUT || tmp->type == HERE_DOC) {
            if (tmp->type == DQUOTE || tmp->type == SQUOTE) {
                // Skip tokens until the end of the quotes
                tmp = tmp->next_token;
                while (tmp && tmp->type != tmp->type)
                    tmp = tmp->next_token;
            } else {
                // Skip tokens until the next WORD or ENV token
                while (tmp && tmp->type != WORD && tmp->type != ENV)
                    tmp = tmp->next_token;
                tmp = tmp->next_token;
            }
            arg_index++;
        }

        if (tmp)
            tmp = tmp->next_token;
    }

    // Allocate memory for the arguments array based on the number of arguments counted
    argument_size = arg_index;
    arguments = calloc(sizeof(char *), (argument_size + 1));
    if (!arguments) {
        free_redir_list(redir_list);
        return NULL;
    }

    // Parse command arguments and store them in the arguments array
    if (parse_command_arguments(token, env, redir_list, arguments) == EXIT_FAILURE) {
        free_redir_list(redir_list);
        free(arguments);
        return NULL;
    }

    // Initialize a new t_node representing the command with the parsed arguments and redirection list
    new_node = init_node(arguments, env->environment_array, redir_list);
    if (!new_node) {
        free_redir_list(redir_list);
        free(arguments);
    }

    return new_node;
}

