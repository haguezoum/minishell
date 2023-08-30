/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signales.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 21:57:04 by haguezou          #+#    #+#             */
/*   Updated: 2023/08/30 15:55:26 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exit_signal(void);

void	handel_signal(int sig)
{
	if (sig == SIGINT)
		exit_signal();
}

void	exit_signal(void)
{
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("\033[1;32mminishell$ \033[0m", 1);
}
