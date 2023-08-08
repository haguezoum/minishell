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



/*

#include <stdio.h>

void print_list(t_lexer *lexer) {
    t_global *token;

    printf("\n\x1b[1;33m╔═════════════════════════════════════════╗\x1b[0m\n");
    printf("\x1b[1;33m║          Lexer Output                   ║\x1b[0m\n");
    printf("\x1b[1;33m╠═════════════════════════════════════════╣\x1b[0m\n");
    printf("\x1b[1;33m║   \x1b[1;35mContent   \x1b[0m\x1b[1;33m| \x1b[1;35mSize \x1b[0m\x1b[1;33m| \x1b[1;35mToken State \x1b[0m\x1b[1;33m| \x1b[1;35mType \x1b[0m\x1b[1;33m║\x1b[0m\n");
    printf("\x1b[1;33m╠═════════════╪═════╪═════════════╪══════╣\x1b[0m\n");

    token = lexer->head;
    while (token) {
        printf("\x1b[1;33m║ \x1b[1;32m%-11.*s \x1b[0m\x1b[1;33m│ \x1b[1;36m%3d \x1b[0m\x1b[1;33m│ \x1b[1;36m%9d \x1b[0m\x1b[1;33m│ \x1b[1;35m%4d \x1b[0m\x1b[1;33m║\x1b[0m\n",
               token->size, token->content,
               token->size, token->token_state, token->type);
        printf("\x1b[1;33m╟─────────────┼─────┼─────────────┼──────╢\x1b[0m\n");
        token = token->next_token;
    }

    printf("\x1b[1;33m╚═════════════╧═════╧═════════════╧══════╝\x1b[0m\n");
}
*/
