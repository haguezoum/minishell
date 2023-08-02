#include "minishell.h"



void free_asn_node(t_node *node) {
    if (node) {
        if (node->node_type == COMMAND) {
            // Free arguments and environment arrays
            char **args = node->content.command.args;
            char **env = node->content.command.env;
            for (int i = 0; args && args[i]; i++) {
                free(args[i]);
            }
            free(args);
            free(env);

            // Free redirections
            free_redir_list(node->content.command.redirections);
        }

        free(node);
    }
}


void free_tree(t_node *node) {
    if (!node) // Handle the case where the node is NULL
        return;

    if (node->node_type == COMMAND) {
        int i = 0;
        while (node->content.command.args[i]) {
            free(node->content.command.args[i]);
            i++;
        }
        free(node->content.command.args);
        free_redir_list(node->content.command.redirections);
    } else if (node->node_type == PIPE) {
        free_tree(node->content.pipe.right);
        free_tree(node->content.pipe.left);
    }
    free(node);
}

void free_redir_list(t_rlist *list) {
    t_relem *tmp = list->first;
    while (tmp) {
        free(tmp->argument);
        free(tmp);
        tmp = tmp->next;
    }
    free(list);
}

