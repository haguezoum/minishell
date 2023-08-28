#include "./includes/minishell.h"




t_environment *find_elem(t_environment *env, char *key)
{
	t_environment *tmp;

	tmp = env->next;
	while (tmp && strcmp(tmp->name, key) != 0)
		tmp = tmp->next;
	return tmp;
}


void delete_elem(t_environment *env, t_environment *elem)
{
    printf("Deleting [%s]\n", env->data);
    printf("key: %s val: %s\n", elem->name, elem->data);
	if (elem && elem->next)
		elem->next->prev = elem->prev;
	if (elem && elem->prev)
		elem->prev->next = elem->next;
	env->count--;

	free(elem->name);
	free(elem->data);
	free(elem);
}
// void del_env_elem(t_environment *env, t_environment *elem)
// {
//     printf("Before deletion: elem=%p, elem->prev=%p, elem->next=%p\n", (void *)elem, (void *)(elem->prev), (void *)(elem->next));

//     if (elem && elem->next)
//         elem->next->prev = elem->prev;
//     if (elem && elem->prev)
//         elem->prev->next = elem->next;

//     printf("After updating pointers: elem->prev=%p, elem->next=%p\n", (void *)(elem->prev), (void *)(elem->next));

//     env->count--;

//     free(elem->name);
//     free(elem->data);
//     free(elem);
// }

void del_env(t_environment *env)
{
	t_environment *tmp;

	tmp = env->next;
	while (tmp)
	{
		t_environment *next = tmp->next;
		free(tmp->name);
		free(tmp->data);
		free(tmp);
		tmp = next;
	}
	free(env);
}

// char	**convert_array(t_environment *env)
// {
// 	t_environment	*tmp;
// 	char			**arr;
// 	int				i;

// 	i = 0;
// 	tmp = env->next; // Assuming the head of the linked list is represented by a sentinel node
// 	arr = ft_calloc(env->count + 1, sizeof(char *));
// 	if (arr)
// 	{
// 		while (i < env->count)
// 		{
// 			arr[i] = ft_strjoin(env->name, "=");
// 			if (env->data)
// 			{
// 				arr[i] = ft_strjoin(arr[i], env->data);
// 			}
// 			env = env->next;
// 			i++;
// 		}
// 		arr[i] = NULL;
// 	}
// 	return (arr);
// }


void main_unset(char *args, t_environment *env_vars)
{
    t_environment *elem;

    elem = NULL;
    if (args && !(ft_isalpha(args[0]) || args[0] == '_') && strlen(args) > 1)
    {
        printf("minishell: unset: `%s`: not a valid identifier\n", args);
        check.exit_status = EXIT_FAILURE;
    }
    else
    {
        elem = find_elem(env_vars, args);
        if (elem)
            delete_elem(env_vars, elem);
    }
}


int our_unset(t_cmd *cmd, t_environment *ev)
{
	// t_environment *env;
	int i;

	i = 1;
	// env = NULL;
	// env = create_env_vars(*ev);
	check.exit_status = EXIT_SUCCESS;
	while (cmd->args[i])
	{
		main_unset(cmd->args[i], ev);
		i++;
	}
	// *ev = convert_array(env);
	// del_env(env);
	return check.exit_status;
}
