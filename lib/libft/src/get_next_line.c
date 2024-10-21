/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 21:03:03 by julio.formiga     #+#    #+#             */
/*   Updated: 2023/11/15 21:03:03 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*fill_line_buffer(int fd, char *left_c, char *buffer)
{
	ssize_t	buffer_read;
	char	*tmp;

	buffer_read = 1;
	while (buffer_read > 0)
	{
		buffer_read = read(fd, buffer, BUFFER_SIZE);
		if (buffer_read == -1)
		{
			free(left_c);
			return (NULL);
		}
		else if (buffer_read == 0)
			break ;
		buffer[buffer_read] = 0;
		if (!left_c)
			left_c = ft_strdup("");
		tmp = left_c;
		left_c = ft_strjoin(tmp, buffer);
		free(tmp);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (left_c);
}

static char	*set_line(char *line_buffer)
{
	char	*left_chars;
	ssize_t	i;

	i = 0;
	while (line_buffer[i] != '\n' && line_buffer[i] != '\0')
		i++;
	if (line_buffer[i] == 0)
		return (0);
	left_chars = ft_substr(line_buffer, i + 1, ft_strlen(line_buffer) - 1);
	if (*left_chars == 0)
	{
		free(left_chars);
		left_chars = NULL;
	}
	line_buffer[i + 1] = 0;
	return (left_chars);
}

char	*get_next_line(int fd)
{
	static char	*left_char[MAX_FD];
	char		*line;
	char		*buffer;

	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (fd < 0 || fd > MAX_FD || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(buffer);
		free(left_char[fd]);
		left_char[fd] = NULL;
		buffer = NULL;
		return (NULL);
	}
	if (!buffer)
		return (NULL);
	line = fill_line_buffer(fd, left_char[fd], buffer);
	free(buffer);
	if (!line)
		return (NULL);
	left_char[fd] = set_line(line);
	return (line);
}
