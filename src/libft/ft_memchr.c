/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 02:33:54 by aet-tass          #+#    #+#             */
/*   Updated: 2022/10/26 18:42:16 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*cast_str;
	unsigned char	cast_c;

	i = 0;
	cast_str = (unsigned char *)s;
	cast_c = (unsigned char)c;
	while (i < n)
	{
		if (cast_str[i] == cast_c)
			return (&cast_str[i]);
		i++;
	}
	return (NULL);
}
