#include "../includes/minishell.h"

char *check_command_in_path(char **path, t_node *ptr)
{
    int i = 0;
    char *tmp;
    char *tst;

    while (path[i])
    {
        tst = ft_strjoin2(path[i], "/");
        tmp = ft_strjoin2(tst, ptr->content.command.args[0]);
        free(tst);
        if (access(tmp, F_OK) == 0)
        {
            return (tmp);
        }
        free(tmp);
        i++;
    }
    return (NULL);
}

char *check_cmand_exist_in_dir(t_node *ptr)
{
    char *str;
    char **path;
    char *tmp;

    str = getenv("PATH");
    path = ft_split(str, ':');
    if (!path)
        return (NULL);
    if (access(ptr->content.command.args[0], F_OK) == 0)
    {
        free_double_pointer(path);
        return (ptr->content.command.args[0]);
    }
    tmp = check_command_in_path(path, ptr);
    free_double_pointer(path);
    return (tmp);
}