#include "minishell.h"

void display_prompt()
{
    // Display the prompt and read user input
    char *line = readline(PROMPT);

    // Check if readline encountered an EOF (Ctrl + D)
    if (!line) 
    {
        printf("exit\n");
        exit(1);
    }
    
    if (ft_strcmp(line, "") == 0 || *line == ' ' || *line == '\t')
    {
        // Check if the input line contains only spaces or tabs
        // Return from the loop and display a new prompt
        free(line);
        return;
    }

    // Add non-empty input to the command history
    if (ft_strlen(line) > 0) 
    {
        add_history(line);
    }

    // Free the memory allocated by readline
    free(line);
}

int main() {
    // Enter an infinite loop to continuously display the prompt
    while (1) {
        display_prompt();
    }

    /* This line will not be reached as the loop is infinite,
     *  but included for completeness
     */
    return 0;
}
