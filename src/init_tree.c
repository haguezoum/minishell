#include "minishell.h"


t_tree *init_tree(t_tree *structure_tree) {
    structure_tree = malloc(sizeof(t_tree));
    if (!structure_tree)
        return NULL;
    structure_tree->top = NULL;
    return structure_tree;
}

t_node *init_node(char **cmd_args, char **env, t_rlist *redirections) {
    t_node *node = malloc(sizeof(t_node));
    if (!node)
        return NULL;

    node->node_type = COMMAND;
    node->content.command.args = cmd_args;
    node->content.command.env = env;
    node->content.command.redirections = redirections;
    node->content.command.fd.input = 0;
    node->content.command.fd.output = 1;
    
    return node;
}

