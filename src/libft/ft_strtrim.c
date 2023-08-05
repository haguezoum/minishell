/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 23:05:12 by aet-tass          #+#    #+#             */
/*   Updated: 2022/10/18 18:45:19 by aet-tass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*cast_s1;
	char	*final_str;
	size_t	len_s1;

	if (!s1 || !set)
		return (NULL);
	cast_s1 = (char *)s1;
	while (*cast_s1 && ft_strchr(set, *cast_s1))
		cast_s1++;
	len_s1 = ft_strlen(cast_s1);
	while (len_s1 && ft_strchr(set, cast_s1[len_s1]))
		len_s1--;
	final_str = ft_substr(cast_s1, 0, len_s1 + 1);
	return (final_str);
}
