#include "minishell.h"

t_lexer *init_lexer(t_lexer *lexer) {
    lexer = malloc(sizeof(t_lexer));
    if (!lexer)
        return NULL;
    lexer->head = NULL;
    lexer->last = NULL;
    lexer->count = 0;
    return lexer;
}

t_global *new_token(char *content, int size, enum e_token type, enum e_state token_state) {
    t_global *token = malloc(sizeof(t_global));
    if (!token)
        return NULL;
    token->content = ft_strdup(content);
    token->size = size;
    token->type = type;
    token->token_state = token_state;
    token->next_token = NULL;
    token->prev_token = NULL;
    return token;
}

void add_token(t_lexer *lexer, t_global *token) {
    if (lexer->head == NULL) {
        lexer->head = token;
        lexer->last = token;
    } else {
        lexer->last->next_token = token;
        token->prev_token = lexer->last;
        lexer->last = token;
    }
    lexer->count++;
}

void free_lexer(t_lexer *lexer) {
    t_global *current = lexer->head;
    while (current) {
        t_global *next = current->next_token;
        free(current->content);
        free(current);
        current = next;
    }
    free(lexer);
}

void print_list(t_lexer *lexer)
{
    t_global *token;

    token = lexer->head;
    while (token)
    {
        int i = 0;
        printf("content: ");
        while (i < token->size)
            putchar(token->content[i++]);
        printf(", size: %i", token->size);
        printf(", token_state: %i", token->token_state);
        printf(", type: %i\n", token->type);

        token = token->next_token;
    }
}


