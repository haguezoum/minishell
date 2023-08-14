#include "minishell.h"



int	check_redir(enum e_token type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == DREDIR_OUT || type == HERE_DOC);
}

void our_echo(t_cmd *command, t_global *tokenList, char **environment) {
	// Initialize variables for the loop and option handling
	int cur_index, args_to_concatenate;
	int hasOptionN = 0;

	// Start at index 1 to skip the command name itself (e.g., "echo")
	cur_index = 1;

	// Check if the first argument starts with a dash ('-')
	if (command->args[cur_index] && command->args[cur_index][0] == '-') {
		// Initialize a variable 'nextCharIndex' to keep track of the index in the argument
		int next_index = 1;

		// Check if the first character after the dash is 'n'
		while (command->args[cur_index][next_index] == 'n') {
			next_index++; // Move to the next character if it's 'n'
		}

		// If the loop above reaches the end of the argument (null terminator '\0'),
		// it means the argument contains only 'n's and is a valid option.
		// Set 'hasOptionN' to 1 to indicate the option was found, and move to the next argument.
		if (command->args[cur_index][next_index] == '\0') {
			hasOptionN = 1;
			cur_index++;
		}
	}

	// Start the loop to process the remaining arguments after the option (if any)
	while (command->args[cur_index]) {
		// Initialize 'args_to_concatenate' variable to keep track of the number of arguments to concatenate
		args_to_concatenate = 0;

		// 'cur_Token' will be used to traverse the 'tokenList' linked list of tokens
		t_global *cur_Token = tokenList;

		// Loop to process tokens until a pipe ('|'), redirection, or whitespace is found
		while (cur_Token && (cur_Token->type != PIPE_LINE) && !check_redir(cur_Token->type) && (cur_Token->type != WHITE_SPACE)) {
			// Check if the token is a WORD or an ENV variable
			if (cur_Token->type == WORD || cur_Token->type == ENV) {
				// If the token's state is DEFAULT, it means it is not part of a command substitution
				// or variable expansion. So, we need to check if it's an empty environment variable.
				if (cur_Token->token_state == DEFAULT) {
					// If the token is of type ENV and its expanded content is an empty string, reduce 'args_to_concatenate'.
					if (cur_Token->type == ENV) {
						if (ft_strcmp(expand_vars(cur_Token->content, environment), "") == 0)
							args_to_concatenate--;
					}
				} else {
					// If the token's state is not DEFAULT, it means it's part of a command substitution
					// or variable expansion. Move to the next DEFAULT state token in the list and increase 'args_to_concatenate'.
					while (cur_Token && (cur_Token->token_state != DEFAULT))
						cur_Token = cur_Token->next_token;
					args_to_concatenate++;
				}
			}
			cur_Token = cur_Token->next_token; // Move to the next token in the 'tokenList'
		}


		// Concatenate arguments based on the value of 'args_to_concatenate' (number of arguments to join)
		while (++args_to_concatenate > 0 && command->args[cur_index + args_to_concatenate])
			command->args[cur_index] = ft_strjoin(command->args[cur_index], command->args[cur_index + args_to_concatenate]);

		// Write the current argument to the standard output (stdout)
		write(STDOUT_FILENO, command->args[cur_index], ft_strlen(command->args[cur_index]));

		// If 'args_to_concatenate' is 0 and there's at least one more argument, write a space to separate them
		if (!args_to_concatenate && command->args[cur_index + 1])
			write(STDOUT_FILENO, " ", 1);

		// Move to the next argument position
		cur_index += args_to_concatenate + 1;
	}

	// After processing all arguments, if the '-n' option was not used, write a newline to stdout
	if (!hasOptionN)
		write(STDOUT_FILENO, "\n", 1);

	// Set the exit status to EXIT_SUCCESS
	check.exit_status = EXIT_SUCCESS;
}
