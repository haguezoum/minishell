/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signales.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 21:57:04 by haguezou          #+#    #+#             */
/*   Updated: 2023/09/07 23:26:52 by haguezou         ###   ########.fr       */
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
	printf("\n");
	rl_initialize();
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	check.exit_status = 130;
}
