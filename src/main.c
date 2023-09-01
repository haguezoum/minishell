#include "./includes/minishell.h"



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
        return ;
    }

    // Add non-empty input to the command history
    if (ft_strlen(line) > 0) {
        add_history(line);
    }

    // Call the lexer function
    t_lexer *lexerObj = tokenizer(line);
    // print_list(lexerObj);

    // Perform syntax check
    int syntax_result = check_command_syntax(lexerObj);
    if (syntax_result == 0) {
        // Create AST tree and parse the input
        t_tree *astTree = init_tree(NULL);
        final_parse(&astTree, lexerObj->head, env); // call the function that parse the input and store it in the astTree
        execute_tree(astTree->top, env, lexerObj->head); // call the fucntion that excute the command that stored in structer astTree->top
        // print_node(astTree->top, 0);
        free_ast_tree(astTree); // Use the new function to free the AST tree
    }
    else
    {
        printf("Syntax error in the input\n");
    }
    // Free the lexer object after parsing
    free_lexer(lexerObj);
    // Free the memory allocated by readline
    free(line);
}

int main()
{
    extern char **environ;
    t_environment *env = create_env_vars(environ);
    check.exit_status = 0; // Initialize the exit status to 0

    // Enter an infinite loop to continuously display the prompt
    struct sigaction sa;

    sa.sa_handler = &handel_signal;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);

    while (1) {
        display_prompt(env);
        system("leaks -q minishell");
    }

    // You may choose to free the environment elements here if needed
    // del_environment(env);

    return 0;
}



