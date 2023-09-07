#include "../includes/minishell.h"

void check_path(char** path, t_node *ptr)
{
    int i;
    char	*tmp;
	char	*tst;

    i = 0;
    tmp = NULL;
    while (path[i])
	{
		tst = ft_strjoin2(path[i], "/");
		tmp = ft_strjoin2(tst, ptr->content.command.args[0]);
		free(tst);
		if (access(tmp, F_OK) == 0)
		{
			free_double_pointer(path);
			return (tmp);
		}
		free(tmp);
		i++;
	}
}