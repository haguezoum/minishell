#include "minishell.h"
#include <string.h>
#include <ctype.h>




int final_parse(t_tree **tree, t_global *token, t_environment *env) {
    t_node *temp_node;

    temp_node = NULL;
    if (!token)
        return EXIT_FAILURE;
    temp_node = build_command_tree(&token, env);
    if (!temp_node)
        return EXIT_FAILURE;
    if (!(*tree)->top)
        (*tree)->top = temp_node;
    else {
        if ((*tree)->top->node_type == PIPE) {
            if (!(*tree)->top->content.pipe.right)
                (*tree)->top->content.pipe.right = temp_node;
            else
                (*tree)->top->content.pipe.left = temp_node;
        } else {
            t_node *pipe_node = ft_calloc(1, sizeof(t_node));
            if (!pipe_node) {
                free_asn_node(temp_node);
                return EXIT_FAILURE;
            }
            pipe_node->node_type = PIPE;
            pipe_node->content.pipe.left = ft_calloc(1, sizeof(t_node));
            pipe_node->content.pipe.right = ft_calloc(1, sizeof(t_node));
            if (!pipe_node->content.pipe.left || !pipe_node->content.pipe.right) {
                free_asn_node(temp_node);
                free_asn_node(pipe_node);
                return EXIT_FAILURE;
            }
            token = token->next_token;
            while (token && token->type == WHITE_SPACE)
                token = token->next_token;
            pipe_node->content.pipe.right = (*tree)->top;
            pipe_node->content.pipe.left = temp_node;
            (*tree)->top = pipe_node;
        }
    }
    final_parse(tree, token, env);
    return EXIT_SUCCESS;
}
