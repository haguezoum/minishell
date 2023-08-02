
/*#include "minishell.h"

t_rlist *parse_redir(t_global **token, t_environment *env, t_rlist *redir) {
    enum e_token type = (*token)->type;
    while ((*token)->type != WORD && (*token)->type != ENV)
        *token = (*token)->next_token;
    
    char *arg;
    t_relem *elem;

    if ((*token)->type == ENV && type != HERE_DOC) {
        arg = store_vars((*token)->content + 1, env);
        if (!arg) {
            printf("bash: %s: ambiguous redirect\n", (*token)->content);
            return NULL;
        }
    } else {
        arg = ft_strdup((*token)->content);
    }
    *token = (*token)->next_token;

    if (type == HERE_DOC) {
        int fd = open("/tmp/.minishell_tmp", O_RDWR | O_CREAT | O_APPEND | O_TRUNC, 0666);
        while (1) {
            char *line = get_next_line(STDIN_FILENO);
            if (line == NULL || ft_strncmp(line, arg, ft_strlen(arg)) == 0) {
                free(line);
                break;
            } else {
                if (ft_strchr(line, '$') != NULL) {
                    char *expanded_line = expand_vars(line, env->environment_array);
                    free(line);
                    line = expanded_line;
                }
                write(fd, line, ft_strlen(line));
                free(line);
            }
        }
        free(arg);

        elem = ft_calloc(1, sizeof(t_relem));
        if (!elem)
            return NULL;

        elem->argument = ft_strdup("/tmp/.minishell_tmp");
        elem->type = HERE_DOC;
    } else {
        elem = ft_calloc(1, sizeof(t_relem));
        if (!elem) {
            free(arg);
            return NULL;
        }
        elem->argument = arg;
        elem->type = type;
    }

    if (!redir->first) {
        redir->first = elem;
    } else {
        redir->last->next = elem;
    }
    redir->last = elem;
    redir->total++;

    return redir;
}
*/


#include "minishell.h"

t_rlist *parse_redir(t_global **token, t_environment *env, t_rlist *redir_list) {
    enum e_token redir_type = (*token)->type;
    while ((*token)->type != WORD && (*token)->type != ENV)
        *token = (*token)->next_token;
    
    char *redir_argument;
    t_relem *redir_element;

    if ((*token)->type == ENV && redir_type != HERE_DOC) {
        redir_argument = store_vars((*token)->content + 1, env);
        if (!redir_argument) {
            printf("bash: %s: ambiguous redirect\n", (*token)->content);
            return NULL;
        }
    } else {
        redir_argument = ft_strdup((*token)->content);
    }
    *token = (*token)->next_token;

    if (redir_type == HERE_DOC) {
        int fd = open("/tmp/.minishell_tmp", O_RDWR | O_CREAT | O_APPEND | O_TRUNC, 0666);
        while (1) {
            char *line = get_next_line(STDIN_FILENO);
            if (line == NULL || ft_strncmp(line, redir_argument, ft_strlen(redir_argument)) == 0) {
                free(line);
                break;
            } else {
                if (ft_strchr(line, '$') != NULL) {
                    char *expanded_line = expand_vars(line, env->environment_array);
                    free(line);
                    line = expanded_line;
                }
                write(fd, line, ft_strlen(line));
                free(line);
            }
        }
        free(redir_argument);

        redir_element = ft_calloc(1, sizeof(t_relem));
        if (!redir_element)
            return NULL;

        redir_element->argument = ft_strdup("/tmp/.minishell_tmp");
        redir_element->type = HERE_DOC;
    } else {
        redir_element = ft_calloc(1, sizeof(t_relem));
        if (!redir_element) {
            free(redir_argument);
            return NULL;
        }
        redir_element->argument = redir_argument;
        redir_element->type = redir_type;
    }

    if (!redir_list->first) {
        redir_list->first = redir_element;
    } else {
        redir_list->last->next = redir_element;
    }
    redir_list->last = redir_element;
    redir_list->total++;

    return redir_list;
}

