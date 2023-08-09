#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"



void free_ast_tree(t_tree *tree) {
    free_tree(tree->top);
    free(tree);
}

void display_prompt(t_environment *env) {
    // Display the prompt and read user input
    char *line = readline(PROMPT);

    // Check if readline encountered an EOF (Ctrl + D)
    if (!line) {
        printf("exit\n");
        exit(1);
    }

    if (ft_strcmp(line, "") == 0 || *line == ' ' || *line == '\t') {
        // Check if the input line contains only spaces or tabs
        // Return from the loop and display a new prompt
        free(line);
        return;
    }

    // Add non-empty input to the command history
    if (ft_strlen(line) > 0) {
        add_history(line);
    }

    // Call the lexer function
    t_lexer *lexerObj = lexer(line);
    // print_list(lexerObj);

    // Perform syntax check
    int syntax_result = check_command_syntax(lexerObj);
    if (syntax_result == 0) {
        // Create AST tree and parse the input
        t_tree *astTree = init_tree(NULL);
        final_parse(&astTree, lexerObj->head, env);
        execute(astTree->top); // call the fucntion that excute the command that stored in structer astTree->top
        // Free the AST tree
        free_ast_tree(astTree); // Use the new function to free the AST tree
    } else {
        printf("Syntax error in the input\n");
    }

    // Free the lexer object after parsing
    free_lexer(lexerObj);

    // Free the memory allocated by readline
    free(line);
}

int main() 
{
    // Initialize the environment with some sample data
    char *env_vars[] = {
        "HOME=/home/user",
        "PATH=/usr/bin:/bin",
        "USER=TassGPT",
        NULL
    };
    t_environment *env = create_env_vars(env_vars);
    check.exit_status = 0; // Initialize the exit status to 0

    // Enter an infinite loop to continuously display the prompt
    while (1) {
        display_prompt(env); // Just call display_prompt without the astTree parameter
    }

    // You may choose to free the environment elements here if needed
    // del_environment(env);

    return 0;
}

