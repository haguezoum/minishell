#include "includes/minishell.h"

int is_valid_env_var_char(char c) {
    return ft_isalnum(c) || c == '_';
}

int is_valid_env_var_name(char *str) 
{
    int i;
    
    i = 0;
    if (!str || !is_valid_env_var_char(str[0]))
        return 0;
    while(str[i])
    {
        if (!is_valid_env_var_char(str[i]))
            return 0;
        i++;
    } 
    return 1;
}

int update_env_var(t_environment *env, char *name, char *data) {
    t_environment *tmp = env->next;

    while (tmp) {
        if (ft_strcmp(tmp->name, name) == 0) {
            free(tmp->data);
            tmp->data = ft_strdup(data);
            return 1; // Variable updated
        }
        tmp = tmp->next;
    }
    t_environment *new_var = ft_calloc(1,sizeof(t_environment));
    if (!new_var)
        return 0;
    new_var->name = ft_strdup(name);
    new_var->data = ft_strdup(data);
    t_environment *tail = env->next;
    if (!tail) 
    {
        env->next = new_var;
        new_var->prev = env;
    } 
    else 
    {
        while (tail->next)
            tail = tail->next;
        tail->next = new_var;
        new_var->prev = tail;
    }
    env->count++;
    // free(name);
    return 1;
}

char *get_key(char *arg)
{
    char *name = NULL;
    char *equal_sign = ft_strchr(arg, '=');

    if (equal_sign)
    {
        name = ft_strndup(arg, equal_sign - arg);
    }
    else
    {
        name = ft_strdup(arg);
    }

    return name;
}


char *get_value(char *arg)
{
    char *data = NULL;
    char *equal_sign = ft_strchr(arg, '=');
    if (equal_sign)
    {
        data = ft_strdup(equal_sign + 1);
    }
    return (data);
}

//-----------------
void print_all_variables(t_environment *env) {
    t_environment *tmp = env->next;
    while (tmp) {
        if (tmp->data) {
            printf("%s=%s\n", tmp->name, tmp->data);
        } else {
            printf("%s", tmp->name);
        }
        tmp = tmp->next;
    }
}

void print_export_error(const char *arg) {
    char *error_msg = "minishell: export: ";
    char *not_valid_msg = ": not a valid identifier\n";
    write(STDERR_FILENO, error_msg, strlen(error_msg));
    write(STDERR_FILENO, arg, strlen(arg));
    write(STDERR_FILENO, not_valid_msg, strlen(not_valid_msg));
}

void handle_invalid_env_var(char *arg, int *exit_status, char *data) {
    print_export_error(arg);
    *exit_status = EXIT_FAILURE;
    if (data)
        free(data);
}

void handle_valid_env_var(char *name, char *data, t_environment *env, int *exit_status) 
{
    if (data) 
    {
        printf("....\n");
        if (!update_env_var(env, name, data))
        {
            printf("-----------------\n");
            perror("minishell: export");
            *exit_status = EXIT_FAILURE;
            return ;
        }
        else 
        {
            printf("-------0-0-0-0-0---------\n");
            if (name) 
            {
                printf("data: %s\n", data);
                printf("name: %s\n", name);
                update_env_var(env, name, data);// <<<<<<<<<<<<<<<<<<<
                printf("%p\n", data);
            }
        }
    }
    else
    {
        data = ft_strdup(""); // Set data to empty string
        if (!update_env_var(env, name, data)) 
        {
            perror("minishell: export");
            *exit_status = EXIT_FAILURE;
        }
    }
    
}

int our_export(char *command, t_environment *env, int quote) 
{
    int exit_status = EXIT_SUCCESS;

    if (!command) {
        print_all_variables(env);
        return exit_status;
    }
    char *arg = command;
    char *name = get_key(arg);
    char *data = get_value(arg);

    if (name && !is_valid_env_var_name(name)) 
    {
        free(name);
        // free(arg);
        handle_invalid_env_var(arg, &exit_status, data);
    }
    else 
    {
        // printf("command add: %p\n", command);
        // printf("command val: %s\n", command);
        handle_valid_env_var(name, data, env, &exit_status);
        // free
        printf(">>>>>>>>data %s\n", data);
        printf(">>>>>>>>data %p\n", data);
        printf(">>>>>>>>arg %p\n", arg);
        printf(">>>>>>>>command %p\n", arg);
        printf(">>>>>>>>name %p\n", name);
        if(quote == 1)
        {
            printf("with quote\n");
            // free(data);
            // free(name);
            free(arg);
        }
        else
        {
            printf("without quote\n");
            free(data);
            free(name);
            // free(arg);
        }
    }
    return exit_status;
}
