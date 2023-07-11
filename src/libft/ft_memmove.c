/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 18:17:34 by aet-tass          #+#    #+#             */
/*   Updated: 2022/10/24 01:47:15 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;
	char	*cast_dst;
	char	*cast_src;

	i = 0;
	cast_src = (char *)src;
	cast_dst = (char *)dst;
	if (cast_src < cast_dst)
	{
		while (len--)
			cast_dst[len] = cast_src[len];
	}
	else
		ft_memcpy(cast_dst, cast_src, len);
	return (dst);
}
