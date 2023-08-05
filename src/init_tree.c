#include "minishell.h"


// Function to initialize the tree
// This function takes a pointer to a t_tree structure (which represents the tree) as an argument
// and returns a pointer to the initialized tree.
t_tree *init_tree(t_tree *structure_tree) {
    // Allocate memory for the tree structure using malloc and store the pointer to it in the structure_tree variable.
    structure_tree = malloc(sizeof(t_tree));
    
    // Check if the memory allocation was successful. If not, return NULL, indicating an error in memory allocation.
    if (!structure_tree)
        return NULL;
    
    // Set the top (root) node of the tree to NULL, indicating an empty tree.
    structure_tree->top = NULL;
    
    // Return the pointer to the initialized tree.
    return structure_tree;
}

// Function to initialize a tree node representing a command
// This function takes arrays of command arguments (cmd_args), environment variables (env),
// and a pointer to a linked list of redirections (redirections) as arguments.
// It returns a pointer to the initialized tree node.
t_node *init_node(char **cmd_args, char **env, t_rlist *redirections) {
    // Allocate memory for the tree node using malloc and store the pointer to it in the node variable.
    t_node *node = malloc(sizeof(t_node));
    
    // Check if the memory allocation was successful. If not, return NULL, indicating an error in memory allocation.
    if (!node)
        return NULL;

    // Set the node_type of the tree node to COMMAND, indicating that this node represents a command.
    node->node_type = COMMAND;
    
    // Set the args member of the content union to point to the array of command arguments.
    node->content.command.args = cmd_args;
    
    // Set the env member of the content union to point to the array of environment variables.
    node->content.command.env = env;
    
    // Set the redirections member of the content union to point to the linked list of redirections.
    node->content.command.redirections = redirections;
    
    // Set the input member of the fd struct to 0, representing stdin as the default input file descriptor.
    node->content.command.fd.input = 0;
    
    // Set the output member of the fd struct to 1, representing stdout as the default output file descriptor.
    node->content.command.fd.output = 1;
    
    // Return the pointer to the initialized tree node.
    return node;
}

