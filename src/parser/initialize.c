/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 08:01:28 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/03 22:44:04 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_rlist *initialize_redir_list() {
    t_rlist *redir_list = calloc(1, sizeof(t_rlist));
    return redir_list;
}

void clean_up(t_rlist *redir_list, char **arguments) {
    free_redir_list(redir_list);
    free(arguments);
}

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
