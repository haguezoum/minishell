#include "minishell.h"
#include <string.h>
#include <ctype.h>

// int final_parse(t_tree **tree, t_global *token, t_environment *env) {
//     // Declare a pointer to a node of the abstract syntax tree (AST).
//     t_node *temp_node;
//     temp_node = NULL; // Initialize the temporary node pointer to NULL.

//     // If the token list is empty, return with EXIT_FAILURE indicating failure.
//     if (!token)
//         return EXIT_FAILURE;

//     // Call the build_command_tree function to construct an AST node for the current command in the token list.
//     // The updated token list is passed as a reference, and temp_node receives the constructed node.
//     temp_node = build_command_tree(&token, env);

//     // If the construction of the AST node (temp_node) failed, return with EXIT_FAILURE.
//     if (!temp_node)
//         return EXIT_FAILURE;

//     // If the top node of the tree (*tree) is NULL, indicating an empty tree, assign the temp_node as the top node of the tree.
//     if (!(*tree)->top)
//         (*tree)->top = temp_node;
//     else {
//         // If the top node of the tree is not NULL, check if it's a PIPE node.
//         if ((*tree)->top->node_type == PIPE) {
//             // If the right child of the PIPE node is NULL, assign temp_node as the right child.
//             if (!(*tree)->top->content.pipe.right)
//                 (*tree)->top->content.pipe.right = temp_node;
//             // If the right child of the PIPE node is not NULL, assign temp_node as the left child.
//             else
//                 (*tree)->top->content.pipe.left = temp_node;
//         } else {
//             // If the top node of the tree is not a PIPE node (it's a COMMAND node), create a new PIPE node and adjust the tree structure.

//             // Allocate memory for a new PIPE node and handle allocation failure.
//             t_node *pipe_node = ft_calloc(1, sizeof(t_node));
//             if (!pipe_node) {
//                 free_asn_node(temp_node);
//                 return EXIT_FAILURE;
//             }

//             // Set the type of the new PIPE node and allocate memory for its left and right children. Handle allocation failure.
//             pipe_node->node_type = PIPE;
//             pipe_node->content.pipe.left = ft_calloc(1, sizeof(t_node));
//             pipe_node->content.pipe.right = ft_calloc(1, sizeof(t_node));
//             if (!pipe_node->content.pipe.left || !pipe_node->content.pipe.right) {
//                 free_asn_node(temp_node);
//                 free_asn_node(pipe_node);
//                 return EXIT_FAILURE;
//             }

//             // Advance the token pointer while skipping white space tokens.
//             token = token->next_token;
//             while (token && token->type == WHITE_SPACE)
//                 token = token->next_token;

//             // Adjust the tree structure by making the current PIPE node the top node, with its left child as temp_node and its right child as the previous top node.
//             pipe_node->content.pipe.right = (*tree)->top;
//             pipe_node->content.pipe.left = temp_node;
//             (*tree)->top = pipe_node;
//         }
//     }

//     // Recursive call to final_parse with the updated token list and the same environment to continue parsing the rest of the tokens.
//     final_parse(tree, token, env);

//     // Return with EXIT_SUCCESS indicating successful parsing.
//     return EXIT_SUCCESS;
// }

// Function to handle building a command node from tokens.
t_node *build_command_node(t_global **token, t_environment *env) {
    t_node *command_node = NULL;

    // Call the build_command_tree function to construct an AST node for the current command in the token list.
    // The updated token list is passed as a reference, and command_node receives the constructed node.
    command_node = build_command_tree(token, env);

    return command_node;
}

// Function to handle creating and setting up a new PIPE node.
t_node *create_pipe_node(t_node *left_child, t_node *right_child) {
    t_node *pipe_node = ft_calloc(1, sizeof(t_node));
    if (!pipe_node) {
        free_asn_node(left_child);
        free_asn_node(right_child);
        return NULL;
    }

    // Set the type of the new PIPE node and allocate memory for its left and right children. Handle allocation failure.
    pipe_node->node_type = PIPE;
    pipe_node->content.pipe.left = left_child;
    pipe_node->content.pipe.right = right_child;
    if (!pipe_node->content.pipe.left || !pipe_node->content.pipe.right) {
        free_asn_node(pipe_node);
        return NULL;
    }

    return pipe_node;
}
// Function to skip tokens until the next PIPE_LINE token.
t_global *skip_tokens_until_pipe_line(t_global *token) {
    while (token && token->type != PIPE_LINE) {
        token = token->next_token;
    }

    if (token) {
        token = token->next_token;
    }

    return token;
}
// Function to handle adding a command node to the AST.
t_node *add_command_to_tree(t_node *left_child, t_node *right_child) {
    t_node *pipe_node = create_pipe_node(left_child, right_child);
    if (pipe_node) {
        return pipe_node;
    }
    // Handle allocation failure if necessary.
    return NULL;
}

// Recursive function to handle final parsing of tokens and building the AST.
int parse_recursive(t_tree **tree, t_global *token, t_environment *env) {
    if (!token)
        return EXIT_FAILURE;

    t_node *command_node = build_command_node(&token, env);

    if (!command_node)
        return EXIT_FAILURE;

    if (!(*tree)->top) {
        (*tree)->top = command_node;
    } else {
        if ((*tree)->top->node_type == PIPE) {
            // Add the new command node to the existing PIPE node.
            t_node *updated_pipe = add_command_to_tree(command_node, (*tree)->top);
            if (updated_pipe) {
                (*tree)->top = updated_pipe;
            }
            // Handle allocation failure if necessary.
        } else {
            // Create a new PIPE node with the current top node and the new command node.
            t_node *pipe_node = add_command_to_tree(command_node, (*tree)->top);
            if (pipe_node) {
                (*tree)->top = pipe_node;
            }
            // Handle allocation failure if necessary.
        }
    }

    // Skip tokens until the next PIPE_LINE token.
    token = skip_tokens_until_pipe_line(token);

    // Recursive call to continue parsing the rest of the tokens.
    return parse_recursive(tree, token, env);
}

// Main entry point for final parsing.
int final_parse(t_tree **tree, t_global *token, t_environment *env) {
    return parse_recursive(tree, token, env);
}
