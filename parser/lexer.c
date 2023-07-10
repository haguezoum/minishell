#include "minishell.h"
#include <stdlib.h>

lexer_t	*init_lexer(char	*contents)
{
	lexer_t	*lexer = malloc(sizeof(lexer_t) * 1);
	lexer->contents = contents;
	lexer->i =  0;
	lexer->c =  contents;

	return lexer;
}


void	lexer_advance(lexer_t *lexer)
{
	if (lexer->c && lexer->i < ft_strlen(lexer->contents))
	{
		lexer->i++;
		lexer->c = lexer->contents[lexer->i];
	}
}
