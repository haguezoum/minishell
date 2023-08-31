#include "./includes/minishell.h"

int ft_strcmp(const char *s1, const char *s2)
{
	if (s1 == NULL || s2 == NULL) // Add this line to avoid segfaults.
	{
        return 0;
    }
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

size_t	ft_strlen(const char *str)
{
	int	i;
	if (!str)
		return (0);
	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

void	ft_bzero(void *string, size_t n)
{
	while (n != 0)
	{
		*(char *)string = '\0';
		n--;
		string++;
	}
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

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (ptr == NULL)
		return (ptr);
	ft_bzero(ptr, count * size);
	return (ptr);
}

// ft_itoa.c -------------------
static int	ft_check(int nbr)
{
	if (!nbr)
		return (0);
	if (nbr < 0)
		return (-nbr);
	return (nbr);
}

static int	ft_len(int nbr)
{
	int	len;

	len = 0;
	if (nbr <= 0)
		len++;
	while (nbr != 0)
	{
		len++;
		nbr = nbr / 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		i;
	char	*str;

	i = ft_len(n);
	str = (char *)malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	str[i] = '\0';
	if (n < 0)
		str[0] = '-';
	else if (n == 0)
		str[0] = '0';
	while (n != 0)
	{
		i--;
		str[i] = ft_check(n % 10) + '0';
		n /= 10;
	}
	return (str);
}
// ft_split.c -------------------

static int	count_words(const char *str, char c)
{
	int	i;
	int	counter;

	counter = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] != c && (str[i + 1] == c || str[i + 1] == '\0'))
			counter++;
		i++;
	}
	return (counter);
}

char	**ft_split(char const *s, char c)
{
	char	**dest;
	int		i;
	int		j;
	int		len_s;

	i = 0;
	j = 0;
	dest = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!dest)
		return (NULL);
	while (j < count_words(s, c))
	{
		while (s[i] && s[i] == c)
			i++;
		len_s = 0;
		while (s[i] && s[i] != c)
		{
			len_s++;
			i++;
		}
		dest[j] = ft_substr(s, (i - len_s), len_s);
		j++;
	}
	dest[j] = NULL;
	return (dest);
}

// ft_strdup.c -------------------
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

// ft_strjoin.c -------------------
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
	free (s1);
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

