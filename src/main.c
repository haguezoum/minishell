/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:02:15 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/08 20:14:20 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	free_ast_tree(t_tree *tree)
{
	free_tree(tree->top);
	free(tree);
}

char	*process_user_input(void)
{
	char	*line;
	char	*tmp;

	line = readline(PROMPT);
	tmp = line;
	line = ft_strtrim(tmp, " \t");
	free(tmp);
	if (!line)
	{
		printf("exit\n");
		exit(1);
	}
	if (ft_strcmp(line, "") == 0 || *line == ' ' || *line == '\t')
	{
		free(line);
		return (NULL);
	}
	if (ft_strlen(line) > 0)
		add_history(line);
	return (line);
}

void	execute_command(char *line, t_environment *env)
{
	t_lexer	*lexer_obj;
	int		syntax_result;
	t_tree	*ast_tree;

	lexer_obj = tokenizer(line);
	expand_all(lexer_obj->head, env);
	syntax_result = check_command_syntax(lexer_obj);
	if (syntax_result == 0)
	{
		ast_tree = init_tree(NULL);
		final_parse(&ast_tree, lexer_obj->head, env);
		execute_tree(ast_tree->top, env, lexer_obj->head);
		free_ast_tree(ast_tree);
	}
	else
        return ;
	free_lexer(lexer_obj);
}

void	display_prompt(t_environment *env)
{
	char	*line;

	line = process_user_input();
	if (line)
	{
		execute_command(line, env);
		free(line);
	}
}

int	main(int ac, char **av)
{
	extern char			**environ;
	t_environment		*env;
	struct sigaction	sa;

	if (ac == 1)
	{
		env = create_env_vars(environ);
		g_check.exit_status = 0;
		sa.sa_handler = &handel_signal;
		sa.sa_flags = SA_RESTART;
		sigemptyset(&sa.sa_mask);
		sigaddset(&sa.sa_mask, SIGINT);
		signal(SIGQUIT, SIG_IGN);
		sigaction(SIGINT, &sa, NULL);
		while (1)
			display_prompt(env);
	}
	else
	{
		printf("Oooops , we dont do that here boy !\n");
		printf("\tUsage: ./minishell\n");
	}
	return (0);
}
