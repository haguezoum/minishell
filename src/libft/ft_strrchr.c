/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 00:26:22 by aet-tass          #+#    #+#             */
/*   Updated: 2022/10/18 21:48:05 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int		i;
	char	*cast_str;

	i = ft_strlen(str);
	cast_str = (char *)str;
	while (i >= 0)
	{
		if (cast_str[i] == (char)c)
			return (&cast_str[i]);
		i--;
	}
	return (0);
}
