# ifndef MINISHELL_H
# define MINISHELL_H
#include <unistd.h>
#include <stdlib.h>
# include "libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>



/*
 * the ANSI escape sequence \x1B[37m is used to set the color to white for "minishell",
 *  \x1B[31m sets the color to red for "_gpt",
 *   \x1B[33m sets the color to yellow for "~>",
 *    and \x1B[0m resets the color to the default.
 */

#define PROMPT "\x1B[37mminishell\x1B[31m_gpt\x1B[0m\x1B[33m~>\x1B[0m "
#endif
