/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:17:07 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/07 12:51:16 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*new_str;
	unsigned int	i;

	i = 0;
	if (!s)
		return (NULL);
	new_str = (char *)malloc(len + 1);
	if (!new_str)
		return (NULL);
	if (start <= ft_strlen(s))
	{
		while (i < len)
		{
			new_str[i] = s[start];
			i++;
			start++;
		}
		new_str[i] = '\0';
	}
	else
		new_str[0] = '\0';
	return (new_str);
}

int	ft_isalnum(int c)
{
	if (((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'z')) || ((c >= 'A')
			&& (c <= 'Z')))
		return (1);
	return (0);
}

int	ft_isalpha(int ch)
{
	if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')))
		return (1);
	return (0);
}
