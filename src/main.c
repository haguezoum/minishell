#include "./includes/minishell.h"
#include <readline/readline.h>


void free_ast_tree(t_tree *tree) {
    free_tree(tree->top);
    free(tree);
}

void display_prompt(t_environment *env) {
    char *line = readline(PROMPT);
    char *tmp = line;
    line = ft_strtrim(tmp, " \t");
    free(tmp);
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
        // free(line);
    }

    t_lexer *lexerObj = tokenizer(line);
    expand_all(lexerObj->head, env);
    // print_list(lexerObj);


    int syntax_result = check_command_syntax(lexerObj);
    if (syntax_result == 0) {
        t_tree *astTree = init_tree(NULL);
        final_parse(&astTree, lexerObj->head, env);
        // free(line);
        execute_tree(astTree->top, env, lexerObj->head);

        // print_node(astTree->top, 0);
        free_ast_tree(astTree);
    }
    else
    {
        printf("Syntax error in the input\n");
    }
    free(line);
    free_lexer(lexerObj);
}

int main(int ac, char **av)
{
    if (ac == 1)
    {
        extern char **environ;
        t_environment *env = create_env_vars(environ);
        check.exit_status = 0;

        struct sigaction sa;

        sa.sa_handler = &handel_signal;
        sa.sa_flags = SA_RESTART;
        sigemptyset(&sa.sa_mask);
        sigaddset(&sa.sa_mask, SIGINT);
        signal(SIGQUIT, SIG_IGN);
        sigaction(SIGINT, &sa, NULL);

        while (1)
        {
            display_prompt(env);
            system("leaks minishell");
        }
    }
    else
    {
        printf("Oooops , we dont do that here boy !\n");
        printf("\tUsage: ./minishell\n");
    }

    return 0;
}



