/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 23:54:01 by srapin            #+#    #+#             */
/*   Updated: 2023/04/23 23:42:39 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	clean_buffer(char *buffer, int i)
{
	int	j;

	j = 0;
	if (i >= 0)
	{
		i++;
		while (j + i < BUFFER_SIZE + 1)
		{
			buffer[j] = buffer[i + j];
			j++;
		}
	}
	while (j < BUFFER_SIZE + 1)
	{
		buffer[j] = 0;
		j++;
	}
}

int	check_data(char **save_data, char *buffer, char *tmp, int s_lenght)
{
	int	i;

	i = 0;
	if (save_data && *save_data)
		free(*save_data);
	*save_data = tmp;
	while (buffer[i])
	{
		if ((*save_data)[s_lenght + i] == '\n')
		{
			clean_buffer(buffer, i);
			return (1);
		}
		i++;
	}
	clean_buffer(buffer, -1);
	return (-1);
}

int	cat_save_and_buffer(char **save_data, char *buffer)
{
	int		s_lenght;
	int		b_lenght;
	char	*tmp;

	s_lenght = 0;
	if (save_data && *save_data)
		s_lenght = ft_strlen(*save_data);
	b_lenght = ft_strlen(buffer) + 1;
	tmp = ft_calloc(s_lenght + b_lenght + 1, sizeof(char));
	if (!tmp && save_data)
	{
		free(save_data);
		return (-1);
	}
	ft_strcat(tmp, *save_data);
	ft_strcat(tmp, buffer);
	return (check_data(save_data, buffer, tmp, s_lenght));
}

char	*get_line(int fd, char *buffer)
{
	int		check_read;
	int		check_end_of_line;
	char	*save_data;

	check_read = -1;
	check_end_of_line = -1;
	save_data = NULL;
	if (*buffer != '\0')
		check_end_of_line = cat_save_and_buffer(&save_data, buffer);
	while (check_end_of_line < 0 && check_read)
	{
		check_read = read(fd, buffer, BUFFER_SIZE);
		if (check_read)
			check_end_of_line = cat_save_and_buffer(&save_data, buffer);
		if (check_read < 0)
		{
			free(save_data);
			return (NULL);
		}
		if (check_read < BUFFER_SIZE && check_read != 0)
			return (save_data);
	}
	return (save_data);
}

char	*get_next_line(int fd)
{
	static char	buffer[MAX_FD][BUFFER_SIZE + 1];
	char		*line;

	if (fd < 0 || fd > MAX_FD)
	{
		return (NULL);
	}
	line = get_line(fd, buffer[fd]);
	return (line);
}
