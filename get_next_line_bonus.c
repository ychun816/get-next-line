/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 19:43:06 by yilin             #+#    #+#             */
/*   Updated: 2024/06/17 18:46:42 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_str(char *data)
{
	int		len;
	char	*new_str;

	len = 0;
	if (!data[len])
		return (NULL);
	while (data[len] && data[len] != '\n')
		len++;
	new_str = ft_calloc(len + 2, sizeof(char));
	if (!new_str)
		return (NULL);
	len = 0;
	while (data[len] && data[len] != '\n')
	{
		new_str[len] = data[len];
		len++;
	}
	if (data[len] && data[len] == '\n')
		new_str[len++] = '\n';
	return (new_str);
}

char	*get_left_str(char *data)
{
	int		i;
	int		j;
	char	*dup_str;

	i = 0;
	j = 0;
	while (data[i] && data[i] != '\n')
		i++;
	if (!data[i])
	{
		free(data);
		return (NULL);
	}
	dup_str = ft_calloc((ft_strlen(data) - i + 1), sizeof(char));
	if (!dup_str)
		return (NULL);
	i += 1;
	while (data[i])
	{
		dup_str[j] = data[i];
		i++;
		j++;
	}
	free(data);
	return (dup_str);
}

char	*read_n_append(int fd, char *str)
{
	char	*buffer;
	int		bytes_read;

	if (!str)
		str = ft_calloc(1, 1);
	bytes_read = 1;
	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			bytes_read = 0;
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		str = ft_strjoin(str, buffer);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	free(buffer);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*left_data[MAX_FD];
	char		*result;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		if (left_data[fd])
		{
			free(left_data[fd]);
			left_data[fd] = NULL;
		}
		return (NULL);
	}
	left_data[fd] = read_n_append(fd, left_data[fd]);
	if (!left_data[fd])
		return (NULL);
	result = get_str(left_data[fd]);
	left_data[fd] = get_left_str(left_data[fd]);
	return (result);
}
/*
#include <stdio.h>

int main(int ac, char *av[])
{
	char *read_line;
	int	fd;
	int i;

	if (ac < 2)
	{
		printf("ERROR! CORRECT FORMAT: %s <file1> <file2>...\n", av[0]);
	}
	i = 1;
	while (i < ac)
	{
		fd = open(av[i], O_RDONLY);
		// read_line = get_next_line(fd);
		if (fd < 0)
		{
			perror("ERROR!");
			i++;
			continue;
		}
		while ((read_line = get_next_line(fd)) != NULL)
		{
			printf("%s", read_line);
			free(read_line);
		}
		close(fd);
		i++;
	}
	return (0);
}
*/