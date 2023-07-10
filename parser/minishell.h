# ifndef MINISHELL_H
# define MINISHELL_H
#include <unistd.h>
#include <stdlib.h>
# include "libft/libft.h"
typedef struct lexer_struct
{
	char c;
	unsigned int i;
	char	*contents;
} lexer_t;

typedef	struct token_struct
{
	enum
	{
		TOKEN_ID,
		TOKEN_EQUALS,
	} type;
	char	*value;
} token_t;

token_t	*init_token(int	type, char	*value);
lexer_t	*init_lexer(char	*contents);
void	lexer_advance(lexer_t	*lexer);
void	lexer_skip_whitespace(lexer_t	*lexer);

#endif
