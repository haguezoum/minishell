/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haguezou <haguezou@student.1337.ma >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 23:19:52 by aet-tass          #+#    #+#             */
/*   Updated: 2023/09/10 03:54:14 by haguezou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strdup(const char *s1)
{
	char	*str;
	int		i;

	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	i = 0;
	if (!str)
		return (NULL);
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strtrim(char *s1, char const *set)

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

char	*ft_strjoin(char *s1, char *s2)
{
	char	*new_str;
	size_t	i;
	size_t	j;

	if (!s1)
	{
		s1 = malloc(1);
		s1[0] = '\0';
	}
	new_str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (new_str == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
		new_str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		new_str[j++] = s2[i++];
	new_str[j] = '\0';
	free(s1);
	return (new_str);
}

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

char	*ft_strchr(const char *str, int c)
{
	int		i;
	char	*cast_str;

	i = 0;
	cast_str = (char *)str;
	while (cast_str[i] && cast_str[i] != (char)c)
		i++;
	if (cast_str[i] == (char)c)
		return (&cast_str[i]);
	return (0);
}
