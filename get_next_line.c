/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfirmino <vfirmino@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:50:35 by vfirmino          #+#    #+#             */
/*   Updated: 2025/08/11 19:33:14 by vfirmino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strclen(const char *s, char stop)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] && s[i] != stop)
		i++;
	return (i + (s[i] == stop));
}

char	*extract_line(char *str)
{
	size_t	i;
	size_t	j;
	char	*ptr;

	if (!str || !str[0])
		return (NULL);
	i = ft_strclen(str, '\n');
	ptr = (char *)malloc(i + 1);
	if (!ptr)
		return (NULL);
	j = 0;
	while (j < i)
	{
		ptr[j] = str[j];
		j++;
	}
	ptr[j] = '\0';
	return (ptr);
}

char	*save_rest(char *str)
{
	size_t	i;
	size_t	j;
	char	*ptr;

	if (!str)
		return (NULL);
	i = ft_strclen(str, '\n');
	if (!str[i])
	{
		free(str);
		return (NULL);
	}
	ptr = (char *)malloc(ft_strlen(str + i) + 1);
	if (!ptr)
	{
		free(str);
		return (NULL);
	}
	j = 0;
	while (str[i])
		ptr[j++] = str[i++];
	ptr[j] = '\0';
	free(str);
	return (ptr);
}

static char	*read_and_store(int fd, char *rest, char *buffer)
{
	int	bytes;

	bytes = 1;
	while (!ft_strchr(rest, '\n') && bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
		{
			free(buffer);
			free(rest);
			return (NULL);
		}
		buffer[bytes] = '\0';
		rest = ft_strjoin(rest, buffer);
		if (!rest)
		{
			free(buffer);
			return (NULL);
		}
	}
	free(buffer);
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*rest;
	char		*line;
	char		*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	rest = read_and_store(fd, rest, buffer);
	if (!rest)
		return (NULL);
	line = extract_line(rest);
	rest = save_rest(rest);
	if (!line)
	{
		free(line);
		return (NULL);
	}
	return (line);
}
