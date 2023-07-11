/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 22:36:03 by aet-tass          #+#    #+#             */
/*   Updated: 2022/10/18 21:41:53 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;
	unsigned char	*cast_s1;
	unsigned char	*cast_s2;

	cast_s1 = (unsigned char *)s1;
	cast_s2 = (unsigned char *)s2;
	i = 0;
	while ((cast_s1[i] != '\0' || cast_s2[i] != '\0') && i < n)
	{
		if (cast_s1[i] > cast_s2[i])
		{
			return (1);
		}
		else if (cast_s1[i] < cast_s2[i])
		{
			return (-1);
		}
		i++;
	}
	return (0);
}
