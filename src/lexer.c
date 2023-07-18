#include "minishell.h"

int tokenize_lexeme(t_lexer *lexer, char *line, int i, enum e_state *state)
{
    int j = i; // Store the starting index

    if (!((line[i] == '\'') || (line[i] == '\"') || (line[i] == '<') || (line[i] == '>') || (line[i] == '|')
            || (line[i] == '$') || (line[i] == ' ') || (line[i] == '\t') || (line[i] == '\v')
            || (line[i] == '\r') || (line[i] == '\f') || (line[i] == '\n') || (line[i] == '\0')))
    {
        // Tokenize a word
        while (!((line[i] == '\'') || (line[i] == '\"') || (line[i] == '<') || (line[i] == '>') || (line[i] == '|')
                || (line[i] == '$') || (line[i] == ' ') || (line[i] == '\t') || (line[i] == '\v')
                || (line[i] == '\r') || (line[i] == '\f') || (line[i] == '\n') || (line[i] == '\0')))
            i++;
        add_token(lexer, new_token(line + j, i - j, WORD, *state));
    }
    else if (line[i] == '\'')
    {
        // Handle single quotes state
        enum e_state e_state = (*state == DEFAULT) ? IN_SINGLE_QUOTES : DEFAULT;
        enum e_token e_type = (e_state == IN_SINGLE_QUOTES) ? QUOTE : DOUBLE_QUOTE;
        add_token(lexer, new_token(line + j, 1, e_type, *state));
        *state = e_state;
    }
    else if (line[i] == '\"')
    {
        // Handle double quotes state
        enum e_state e_state = (*state == DEFAULT) ? IN_DOUBLE_QUOTES : DEFAULT;
        enum e_token e_type = (e_state == IN_DOUBLE_QUOTES) ? DOUBLE_QUOTE : QUOTE;
        add_token(lexer, new_token(line + j, 1, e_type, *state));
        *state = e_state;
    }
    else if (line[i] == '$')
    {
        if ((line[i + 1] == ' ') || (line[i + 1] == '\t') || (line[i + 1] == '\v')
            || (line[i + 1] == '\r') || (line[i + 1] == '\f') || (line[i + 1] == '\n')
            || (line[i + 1] == '\0') || (line[i + 1] == '='))
        {
            // Tokenize a word
            add_token(lexer, new_token(line + i, 1, WORD, *state));
            i++;
        }
        else
        {
            // Tokenize an environment variable
            int k = i + 1;
            if ((line[k] == '?' || (line[k] >= '0' && line[k] <= '9')))
                k++;
            else
            {
                while (((line[k] >= 'A' && line[k] <= 'Z') || (line[k] >= 'a' && line[k] <= 'z') || (line[k] >= '0' && line[k] <= '9')) && line[k] != '\n' && line[k] != '\0')
                    k++;
            }
            add_token(lexer, new_token(line + i, k - i, ENV, *state));
            i = k;
        }
    }
    else if (line[i] == '>' || line[i] == '<')
    {
        // Tokenize redirection
        int k = i + 1;
        if (line[i] == '<')
        {
            if (line[k] == '<')
            {
                add_token(lexer, new_token(line + i, 2, HERE_DOC, *state));
                k++;
            }
            else
                add_token(lexer, new_token(line + i, 1, REDIR_IN, *state));
        }
        else if (line[i] == '>')
        {
            if (line[k] == '>')
            {
                add_token(lexer, new_token(line + i, 2, DREDIR_OUT, *state));
                k++;
            }
            else
                add_token(lexer, new_token(line + i, 1, REDIR_OUT, *state));
        }
        i = k;
    }
    else if (line[i] == '|')
    {
        // Tokenize pipe
        add_token(lexer, new_token(line + i, 1, PIPE, *state));
        i++;
    }
    else if (line[i] == ' ' || line[i] == '\t' || line[i] == '\v'
            || line[i] == '\r' || line[i] == '\f' || line[i] == '\n')
    {
        // Tokenize whitespace
        add_token(lexer, new_token(line + i, 1, WHITE_SPACE, *state));
        i++;
    }

    return i;
}

t_lexer *lexer(char *line)
{
    int i;
    t_lexer *lexer;
    enum e_state state;

    i = 0;
    state = DEFAULT;
    lexer = init_lexer(lexer);
    if (lexer)
    {
        while (line[i])
            i = tokenize_lexeme(lexer, line, i, &state);
    }
    free(line);
    return (lexer);
}

