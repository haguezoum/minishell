#include "minishell.h"
/*
 *  Helper function to handle single quotes state :
 *  This function handle_single_quotes adds a single quote token to the lexer,
 *  and it updates the state of the lexer. If the current state is DEFAULT,
 *   it changes it to IN_SINGLE_QUOTES, indicating that we are inside a single-quoted string.
 *    If the current state is IN_SINGLE_QUOTES, it changes it back to DEFAULT,
 *     indicating that we have exited the single-quoted string
 */

void handle_single_quotes(t_lexer *lexer, char *line, int i, enum e_state *state) {
    // Add a single quote token to the lexer, based on the current state
    add_token(lexer, new_token(line + i, 1, QUOTE, *state));

    // Update the state enum variable accordingly
    if (*state == DEFAULT)
        *state = IN_SINGLE_QUOTES;
    else
        *state = DEFAULT;
}

/**********************************************************************************************/


/* Helper function to handle double quotes state
 * This function handle_double_quotes works similarly to handle_single_quotes,
 *  but it handles double quotes instead. It adds a double quote token to the
 *  lexer and updates the state accordingly. If the current state is DEFAULT,
 *  it changes it to IN_DOUBLE_QUOTES, indicating that we are inside a double-quoted string.
 *   If the current state is IN_DOUBLE_QUOTES, it changes it back to DEFAULT,
 *   indicating that we have exited the double-quoted string.
 */

void handle_double_quotes(t_lexer *lexer, char *line, int i, enum e_state *state) {
    // Add a double quote token to the lexer, based on the current state
    add_token(lexer, new_token(line + i, 1, DOUBLE_QUOTE, *state));

    // Update the state enum variable accordingly
    if (*state == DEFAULT)
        *state = IN_DOUBLE_QUOTES;
    else
        *state = DEFAULT;
}


/**********************************************************************************************/


/*
 *  Helper function to tokenize a word :
 *  This function tokenize_word is a helper function to tokenize a word.
 *  It starts by setting j to the current index i, which represents the starting
 *  index of the word.  Then, it enters a loop that continues as long as the character
 *  at index i is not one of the specified characters: ', double_quote, <, >, |, $, space, tab,
 *  vertical tab, carriage return, form feed, or newline. Inside the loop, i is incremented until
 *  it finds the end of the word. Once the loop finishes, it adds the word token to the lexer using
 *   the add_token function, passing the starting address of the word, its length (i - j),
 *   the type of token (WORD), and the current state.
 */

/*
void tokenize_word(t_lexer *lexer, char *line, int i, enum e_state *state)
{
    // Store the starting index of the word
    int j = i;

    // Loop to find the end of the word
    while (line[i] && !ft_strchr("\'\"<>|$ \t\v\r\f\n", line[i]))
        i++;

    // Add the word token to the lexer
    add_token(lexer, new_token(line + j, i - j, WORD, *state));
}
*/

void tokenize_word(t_lexer *lexer, char *line, int i, enum e_state *state)
{
    // Store the starting index of the word
    int j = i;

    // Loop to find the end of the word
    while (line[i] && !ft_strchr("\'\"<>|$ \t\v\r\f\n", line[i]))
        i++;

    // Add the word token to the lexer
    add_token(lexer, new_token(line + j, i - j, WORD, *state));
}

/**************************************************************************************************/


/*  Helper function to tokenize an environment variable :
 *  The tokenize_env_variable function is responsible for tokenizing environment variables.
 *  It starts by setting k to the next index after the current index i. Then, it checks if the
 *  character at index k is either a question mark or a digit (0-9). If so, it increments k to
 *  move past the environment variable. Otherwise, it enters a loop that continues as long as the
 *  character at index k is an alphanumeric character (letters or digits) and not a newline or the
 *  end of the line. This loop is responsible for finding the end of the environment variable. Once
 *  the loop finishes, the function adds the environment variable token to the lexer using the
 *  add_token function, passing the starting address of the environment variable, its length (k - i),
 *  the type of token (ENV), and the current state. Finally, the function returns the index k, which
 *  represents the next position in the input line after the environment variable.
 */

int tokenize_env_variable(t_lexer *lexer, char *line, int i, enum e_state *state) {
    // Move to the next index after the current index 'i'
    int k = i + 1;

    // Check if the character is either '?' or a digit (0-9)
    if ((line[k] == '?' || (line[k] >= '0' && line[k] <= '9')))
        k++;
    else {
        // Enter a loop to find the end of the environment variable
        while (ft_isalnum(line[k]) && line[k] != '\n' && line[k] != '\0')
            k++;
    }

    // Add the environment variable token to the lexer
    add_token(lexer, new_token(line + i, k - i, ENV, *state));

    // Return the index 'k', which represents the next position after the environment variable
    return k;
}

/*****************************************************************************************************/

/* Helper function to tokenize redirection :
 * The tokenize_redirection function handles the tokenization of redirection symbols.
 * It starts by setting k to the next index after the current index i. Then, it checks
 * whether the current character is < or >. If it's <, it further checks if the character
 * at index k is also <. If so, it adds a here document token to the lexer and increments k to
 * move past the here document symbol. Otherwise, it adds a regular input redirection token to
 * the lexer. If the current character is >, the function performs a similar check for >> to handle
 * append output redirection. After adding the appropriate token, the function returns the index k,
 * which represents the next position in the input line after the redirection symbol.
 */

int tokenize_redirection(t_lexer *lexer, char *line, int i, enum e_state *state)
{
    // Move to the next index after the current index 'i'
    int k = i + 1;

    // Check if the current character is '<'
    if (line[i] == '<') {
        // If the next character is also '<', add a here document token and move past it
        if (line[k] == '<') {
            add_token(lexer, new_token(line + i, 2, HERE_DOC, *state));
            k++;
        } else {
            // Otherwise, add a regular input redirection token
            add_token(lexer, new_token(line + i, 1, REDIR_IN, *state));
        }
    } else if (line[i] == '>') {
        // If the current character is '>', check for '>>' for append output redirection
        if (line[k] == '>') {
            add_token(lexer, new_token(line + i, 2, DREDIR_OUT, *state));
            k++;
        } else {
            // Otherwise, add a regular output redirection token
            add_token(lexer, new_token(line + i, 1, REDIR_OUT, *state));
        }
    }

    // Return the index 'k', which represents the next position after the redirection symbol
    return k;
}


/**********************************************************************************************************/

/*
 * The tokenize_lexeme function is the main tokenization function. It receives the lexer,
 * the input line, the current index i, and a pointer to the state enum variable state. Based
 * on the current character, it tokenizes the input accordingly by either calling the appropriate
 * helper functions or directly adding tokens to the lexer. This function handles single and double quotes,
 * environment variables, redirection symbols, whitespaces, and words, updating the lexer and state as needed.
 * Finally, the function returns the updated index i, which represents the next position in the input line to
 * be tokenized.
 */


int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\v' || c == '\r' || c == '\f' || c == '\n';
}


int tokenize_lexeme(t_lexer *lexer, char *line, int i, enum e_state *state) {
    // Check the current character and tokenize accordingly
    if (line[i] == '\'') {
        // If the character is a single quote, handle single quotes state
        handle_single_quotes(lexer, line, i, state);
        i++;
    } else if (line[i] == '\"') {
        // If the character is a double quote, handle double quotes state
        handle_double_quotes(lexer, line, i, state);
        i++;
    } else if (line[i] == '$') {
        // If the character is '$', tokenize an environment variable or a word
        if (ft_strchr(" \t\v\r\f\n=<>|", line[i + 1])) {
            // If the next character is a whitespace or a special character, tokenize a word
            add_token(lexer, new_token(line + i, 1, WORD, *state));
            i++;
        } else {
            // Otherwise, tokenize an environment variable
            i = tokenize_env_variable(lexer, line, i, state);
        }
    } else if (ft_strchr("<>|", line[i])) {
        // If the character is a redirection symbol or a pipe, tokenize accordingly
        add_token(lexer, new_token(line + i, 1, line[i], *state));
        i++;
    } else if (is_whitespace(line[i])) {
        // If the character is a whitespace, tokenize whitespace
        add_token(lexer, new_token(line + i, 1, WHITE_SPACE, *state));
        i++;
    } else {
        // Otherwise, tokenize a word and pass the current state
        int j = i;
        while (line[i] && !ft_strchr(" \t\v\r\f\n\'\"<>|$", line[i])) {
            if (*state == DEFAULT && (line[i] == '\'' || line[i] == '\"')) {
                // Inside a word, set token_state to 0 if we encounter a quote
                if (line[i] == '\"') {
                    *state = IN_DOUBLE_QUOTES;
                } else if (line[i] == '\'') {
                    *state = IN_SINGLE_QUOTES;
                }
                add_token(lexer, new_token(line + j, i - j, WORD, *state));
                j = i + 1; // Skip the quote
            }
            i++;
        }
        add_token(lexer, new_token(line + j, i - j, WORD, *state));
    }

    // Check for HEREDOC (<<)
    if (line[i] == '<' && line[i + 1] == '<') {
        add_token(lexer, new_token(line + i, 2, HERE_DOC, *state));
        i += 2; // Move past the HEREDOC token
        return i;
    }

    // Check for DRED (>>)
    if (line[i] == '>' && line[i + 1] == '>') {
        add_token(lexer, new_token(line + i, 2,   DREDIR_OUT, *state));
        i += 2; // Move past the DRED token
        return i;
    }

    // Update the state back to DEFAULT when reaching the closing single or double quote
    if ((*state == IN_DOUBLE_QUOTES && line[i] == '\"') || (*state == IN_SINGLE_QUOTES && line[i] == '\'')) {
        *state = DEFAULT;
        add_token(lexer, new_token(line + i, 1, line[i], *state)); // Add the closing quote as a separate token
        i++; // Move past the closing quote
    }

    // Return the updated index 'i'
    return i;
}








/*******************************************************************************************************************************/


t_lexer *lexer(char *line) {
    // Initialize the lexer and set the initial state to DEFAULT
    t_lexer *lexer = init_lexer(lexer);
    if (lexer) {
        enum e_state state = DEFAULT;
        int i = 0;
        while (line[i]) {
            // Tokenize each lexeme in the input line
            i = tokenize_lexeme(lexer, line, i, &state);
        }
    }
    return lexer;
}

