#include "minishell.h"

t_environment *initialize_environment(char **env_vars)
{
    t_environment *env = (t_environment*)malloc(sizeof(t_environment));
    if (!env)
        return NULL;

    int i = 0;
    while (env_vars[i])
    {
        char *var_name;
        char *data;
        int index = strchr(env_vars[i], '=');
        if (index != -1)
        {
            var_name = strdup(env_vars[i]);
            var_name[index] = '\0'; // Remove the '=' character
            data = strdup(env_vars[i] + index + 1);
        }
        else
        {
            var_name = strdup(env_vars[i]);
            data = NULL;
        }

        t_environment *new_elem = (t_environment*)malloc(sizeof(t_environment));
        if (!new_elem)
        {
            delete_environment(env);
            return NULL;
        }

        new_elem->var_name = var_name;
        new_elem->data = data;

        new_elem->next = NULL;
        new_elem->prev = NULL;

        if (env->head == NULL)
        {
            env->head = new_elem;
            env->tail = new_elem;
        }
        else
        {
            new_elem->prev = env->tail;
            env->tail->next = new_elem;
            env->tail = new_elem;
        }

        i++;
    }
    env->size = i;

    return env;
}

