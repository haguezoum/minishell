#include "minishell.h"


int	check_redir(enum e_token type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == DREDIR_OUT || type == HERE_DOC);
}

// Function to check if an argument is a valid option '-n'
int check_option(char *arg) {
    int next_index = 1;

    while (arg[next_index] == 'n') {
        next_index++;
    }

    return (arg[next_index] == '\0');
}

// Function to concatenate arguments based on the value of 'args_to_concatenate'
void concatenate_arguments(char **args, int cur_index, int args_to_concatenate) {
    while (++args_to_concatenate > 0 && args[cur_index + args_to_concatenate]) {
        char *temp = ft_strjoin(args[cur_index], args[cur_index + args_to_concatenate]);
        free(args[cur_index]);
        args[cur_index] = temp;
    }
}

void process_arguments(t_cmd *command, t_global *tokenList, char **environment) {
    int cur_index = 1;
    int first_arg = 1;

    while (command->args[cur_index]) {
        if (!first_arg) {
            write(STDOUT_FILENO, " ", 1);
        } else {
            first_arg = 0;
        }

        write(STDOUT_FILENO, command->args[cur_index], ft_strlen(command->args[cur_index]));
        cur_index++;
    }
}



// Function to handle the '-n' option
void handle_option_n(int *hasOptionN, char **args, int *cur_index) {
    if (args[*cur_index] && args[*cur_index][0] == '-' && check_option(args[*cur_index])) {
        *hasOptionN = 1;
        (*cur_index)++;
    }
}


void our_echo(t_cmd *command, t_global *tokenList, char **environment) {
    int cur_index = 1;
    int first_arg = 1;
    int has_option_n = 0;

    // Handle the -n option
    while (command->args[cur_index] && check_option(command->args[cur_index])) {
        has_option_n = 1;
        cur_index++;
    }

    // Print the arguments
    while (command->args[cur_index]) {
        if (!first_arg) {
            write(STDOUT_FILENO, " ", 1);
        } else {
            first_arg = 0;
        }

        write(STDOUT_FILENO, command->args[cur_index], strlen(command->args[cur_index]));
        cur_index++;
    }

    // Print newline if -n option not used
    if (!has_option_n) {
        write(STDOUT_FILENO, "\n", 1);
    }

    check.exit_status = EXIT_SUCCESS;
}
