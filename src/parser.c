#include "minishell.h"
#include <string.h>
#include <ctype.h>

int final_parse(t_tree **tree, t_global *token, t_environment *env) {
    // Declare a pointer to a node of the abstract syntax tree (AST).
    t_node *temp_node;
    temp_node = NULL; // Initialize the temporary node pointer to NULL.

    // If the token list is empty, return with EXIT_FAILURE indicating failure.
    if (!token)
        return EXIT_FAILURE;

    // Call the build_command_tree function to construct an AST node for the current command in the token list.
    // The updated token list is passed as a reference, and temp_node receives the constructed node.
    temp_node = build_command_tree(&token, env);

    // If the construction of the AST node (temp_node) failed, return with EXIT_FAILURE.
    if (!temp_node)
        return EXIT_FAILURE;

    // If the top node of the tree (*tree) is NULL, indicating an empty tree, assign the temp_node as the top node of the tree.
    if (!(*tree)->top)
        (*tree)->top = temp_node;
    else {
        // If the top node of the tree is not NULL, check if it's a PIPE node.
        if ((*tree)->top->node_type == PIPE) {
            // If the right child of the PIPE node is NULL, assign temp_node as the right child.
            if (!(*tree)->top->content.pipe.right)
                (*tree)->top->content.pipe.right = temp_node;
            // If the right child of the PIPE node is not NULL, assign temp_node as the left child.
            else
                (*tree)->top->content.pipe.left = temp_node;
        } else {
            // If the top node of the tree is not a PIPE node (it's a COMMAND node), create a new PIPE node and adjust the tree structure.

            // Allocate memory for a new PIPE node and handle allocation failure.
            t_node *pipe_node = ft_calloc(1, sizeof(t_node));
            if (!pipe_node) {
                free_asn_node(temp_node);
                return EXIT_FAILURE;
            }

            // Set the type of the new PIPE node and allocate memory for its left and right children. Handle allocation failure.
            pipe_node->node_type = PIPE;
            pipe_node->content.pipe.left = ft_calloc(1, sizeof(t_node));
            pipe_node->content.pipe.right = ft_calloc(1, sizeof(t_node));
            if (!pipe_node->content.pipe.left || !pipe_node->content.pipe.right) {
                free_asn_node(temp_node);
                free_asn_node(pipe_node);
                return EXIT_FAILURE;
            }

            // Advance the token pointer while skipping white space tokens.
            token = token->next_token;
            while (token && token->type == WHITE_SPACE)
                token = token->next_token;

            // Adjust the tree structure by making the current PIPE node the top node, with its left child as temp_node and its right child as the previous top node.
            pipe_node->content.pipe.right = (*tree)->top;
            pipe_node->content.pipe.left = temp_node;
            (*tree)->top = pipe_node;
        }
    }

    // Recursive call to final_parse with the updated token list and the same environment to continue parsing the rest of the tokens.
    final_parse(tree, token, env);

    // Return with EXIT_SUCCESS indicating successful parsing.
    return EXIT_SUCCESS;
}

