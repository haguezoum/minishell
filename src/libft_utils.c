#include "minishell.h"

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    
    if (*s1 == *s2)
        return 0;
    else if (*s1 < *s2)
        return -1;
    else 
        return 1;
}



char	*ft_strndup(char *s1, int n)
{
	char	*copy;
	size_t	s1_len;
	int		i;

	i = 0;
	s1_len = ft_strlen(s1);
	copy = malloc(sizeof(char) * (s1_len + 1));
	if (!copy)
		return (NULL);
	while (s1[i] && i < n)
	{
		copy[i] = s1[i];
		i++;
	}
	copy[i] = '\0';

	return (copy); // Add this line to return the allocated copy.
}


