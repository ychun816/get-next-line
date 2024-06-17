/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 19:43:06 by yilin             #+#    #+#             */
/*   Updated: 2024/06/17 17:47:33 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/* 
 get str
 * Extracts a line from the given string.
 * @str: The original string containing the data read from the file.
 * Return: A new string containing the extracted line.
*/
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

/* 
 get_left_str
 * Updates the static string with leftover data after extracting a line.
 * @str: The original string containing the data read from the file.
 * Return: 
   A new string with the leftover data or NULL if there is no leftover data.
*/
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

/* 
 read_n_append (str by str)
 * Reads data from the file and appends it to the existing string.
 * @str: The existing string containing previously read data.
 * @fd: The file descriptor to read from.
 * Return: The updated string with the newly read data.
*/
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

/* Static string to hold leftover data between calls */
char	*get_next_line(int fd)
{
	static char	*left_data;
	char		*result;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		if (left_data)
		{
			free(left_data);
			left_data = NULL;
		}
		return (NULL);
	}
	left_data = read_n_append(fd, left_data);
	if (!left_data)
		return (NULL);
	result = get_str(left_data);
	left_data = get_left_str(left_data);
	return (result);
}
/*
#include <stdio.h>
=========== MAIN A ================
int main(void)
{
	int fd;
	char	*read_line;
	int i = 0;

	fd = open("test1.txt", O_RDONLY);
	while (i < 4)
	{
		read_line = get_next_line(fd);
		printf("%s", read_line);
		free(read_line);
		i++;
	}
	close(fd);
	return 0;
}
=========== MAIN A ================
int main(int ac, char *av[])
{
	(void)ac;

	char *read_line;
	int	fd;
	int i;

	i = 0;
	fd = open(av[1], O_RDONLY);
	if (fd < 0)
	{
		perror("ERROR!");
		return (1);
	}
	while (i < 3)
	{	
		read_line = get_next_line(fd);
		if (read_line == NULL)
            break;
		printf("%s", read_line);
		free(read_line);
		i++;
	}
	close(fd);
	return (0);
}
*/