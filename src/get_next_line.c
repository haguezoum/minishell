#include "minishell.h"
void	ft_check_read(char *buffer, char *saved)
{
	free (buffer);
	if (saved)
		free (saved);
}

char	*ft_get_saved(int fd, char *saved)
{
	char	*buffer;
	int		count;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (0);
	count = 1;
	while (count > 0)
	{
		count = read(fd, buffer, BUFFER_SIZE);
		if (count == -1)
		{
			ft_check_read(buffer, saved);
			return (NULL);
		}
		if (count != 0)
		{
			buffer[count] = '\0';
			saved = ft_strjoin(saved, buffer);
			if (ft_strchr(buffer, '\n'))
				break ;
		}
	}
	free (buffer);
	return (saved);
}

char	*ft_get_line(char *buffer)
{
	char	*line;
	int		i;

	if (!buffer)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
			line [i] = buffer[i];
			i++;
	}
	if (buffer[i] == '\n')
	{
		line[i] = '\n';
		line[i + 1] = '\0';
	}
	else
		line[i] = '\0';
	return (line);
}

char	*ft_get_rest(char *buffer)
{
	char	*rest;
	int		i;

	i = 0;
	if (!buffer)
	{
		free (buffer);
		return (NULL);
	}
	while (buffer[i])
	{
		if (buffer[i] == '\n')
		{
			if (buffer[i + 1] == '\0')
			{
				free (buffer);
				return (NULL);
			}
			rest = ft_substr(buffer, i + 1, ft_strlen(buffer) - i);
			return (rest);
		}
		i++;
	}
	free (buffer);
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || fd == 1 || fd == 2 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = ft_get_saved(fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_get_line(buffer);
	buffer = ft_get_rest(buffer);
	return (line);
}
