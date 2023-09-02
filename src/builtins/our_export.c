#include "../includes/minishell.h"

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
        if (!update_env_var(env, name, data))
        {
            perror("minishell: export");
            *exit_status = EXIT_FAILURE;
            return ;
        }
        // else
        // {
        //     // printf("-------0-0-0-0-0---------\n");
        //     if (name)
        //     {
        //         // printf("data: %s\n", data);
        //         // printf("name: %s\n", name);
        //         update_env_var(env, name, data);// <<<<<<<<<<<<<<<<<<<
        //         // printf("%p\n", data);
        //     }
        // }
    }
    else
    {
        data = ft_strdup("");
        if (!update_env_var(env, name, data))
        {
            perror("minishell: export");
            *exit_status = EXIT_FAILURE;
        }
        free(data);
    }

}

int our_export(char *command, t_environment *env, int quote)
{
    int exit_status = EXIT_SUCCESS;
    char *arg ;
    char *name;
    char *data;

    arg = command;
    name = get_key(arg);
    data = get_value(arg);

    if (!command)
    {
        print_all_variables(env);
        return exit_status;
    }
    if (name && !is_valid_env_var_name(name))
    {
        free(name);
        handle_invalid_env_var(arg, &exit_status, data);
    }
    else
    {
        handle_valid_env_var(name, data, env, &exit_status);
        if(quote == 0)
        {
            free(data);
            free(name);
        }
        else if(quote == -1)
        {
            free(name);
        }

    }
    return exit_status;
}
