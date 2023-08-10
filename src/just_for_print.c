#include <stdio.h>
#include "minishell.h"
// Define ANSI escape codes for colors
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_BOLD          "\x1b[1m"
#define ANSI_UNDERLINE     "\x1b[4m"
#define ANSI_COLOR_GREEN "\x1b[92m"

void print_node(t_node *cmd, int depth) {
    if (cmd->node_type == COMMAND) {
        int i = 0;
        printf("%*s" ANSI_COLOR_GREEN "COMMAND Node : \n" ANSI_COLOR_GREEN , depth * 1, "");
        while (cmd->content.command.args[i]) {
	            printf(ANSI_COLOR_RESET "    command arg[%i]: " ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET, i, cmd->content.command.args[i]);

            i++;
        }

        t_relem *tmp = cmd->content.command.redirections->first;
	if (tmp)
	{
		printf ("\n");
		 printf(ANSI_BOLD  "    Redirection List : \n" ANSI_COLOR_RESET);
        	while (tmp) {
            	printf("%*s" ANSI_COLOR_YELLOW " -> redir type: %i\n" ANSI_COLOR_RESET, (depth + 1) * 4, "", tmp->type);
            	printf("%*s" ANSI_COLOR_YELLOW " -> redir arg: %s\n" ANSI_COLOR_RESET, (depth + 1) * 4, "", tmp->argument);
            	tmp = tmp->next;
		printf("\n");
        }
	}
        printf("%*sstd_in: %d, std_out: %d\n", (depth + 1) * 4, "", cmd->content.command.fd.input, cmd->content.command.fd.output);
    } else if (cmd->node_type == PIPE) {
        printf("%*s" ANSI_COLOR_CYAN "PIPE Node\n" ANSI_COLOR_RESET, depth * 4, "");
    }
}



