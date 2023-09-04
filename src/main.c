#include "./includes/minishell.h"



void free_ast_tree(t_tree *tree) {
    free_tree(tree->top);
    free(tree);
}

void display_prompt(t_environment *env) {
    char *line = readline(PROMPT);
    line = ft_strtrim(line, " \t");
    if (!line) {
        printf("exit\n");
        exit(1);
    }

    if (ft_strcmp(line, "") == 0 || *line == ' ' || *line == '\t') {
        free(line);
        return ;
    }

    if (ft_strlen(line) > 0) {
        add_history(line);
    }

    t_lexer *lexerObj = tokenizer(line);
    // print_list(lexerObj);

    int syntax_result = check_command_syntax(lexerObj);
    if (syntax_result == 0) {
        t_tree *astTree = init_tree(NULL);
        final_parse(&astTree, lexerObj->head, env);
        execute_tree(astTree->top, env, lexerObj->head);

        // print_node(astTree->top, 0);
        free_ast_tree(astTree);
    }
    else
    {
        printf("Syntax error in the input\n");
    }
    free_lexer(lexerObj);
    free(line);
}

int main()
{
    extern char **environ;
    t_environment *env = create_env_vars(environ);
    check.exit_status = 0;

    struct sigaction sa;

    sa.sa_handler = &handel_signal;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);

    while (1) {
        display_prompt(env);
    }

    return 0;
}



