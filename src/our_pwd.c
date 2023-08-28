/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   our_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 23:45:25 by aet-tass          #+#    #+#             */
/*   Updated: 2023/08/28 23:45:26 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

int our_pwd(t_cmd *command)
{
    // Declare a character pointer to hold the directory path
    char *directory_path;

    // Ignore the command parameter (it's unused in this implementation)
    (void)command;

    // Allocate memory to store the directory path
    directory_path = (char *)malloc(PATH_MAX);

    // Check if memory allocation was successful
    if (!directory_path)
    {
        perror("minishell: pwd: malloc");
        check.exit_status = EXIT_FAILURE;
        return (-1);
    }

    // Get the current working directory and store it in 'directory_path'
    if (getcwd(directory_path, PATH_MAX) == NULL)
    {
        perror("minishell: pwd: ");
        free(directory_path);
        check.exit_status = EXIT_FAILURE;
        return (-1);
    }

    // Print the current working directory to stdout
    printf("%s\n", directory_path);

    // Free the dynamically allocated memory for 'directory_path'
    free(directory_path);

    // Set the exit status to indicate success
    check.exit_status = EXIT_SUCCESS;
    return (EXIT_SUCCESS);
}

