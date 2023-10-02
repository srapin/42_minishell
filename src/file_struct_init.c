/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_struct_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:08:04 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_file_struct(t_file *file_struct)
{
	file_struct->name = NULL;
	file_struct->fd = -1;
	file_struct->flag = -1;
	file_struct->out = false;
}

void	init_file_struct_with_filename(t_file *file_struct, char *filename)
{
	init_file_struct(file_struct);
	file_struct->name = filename;
}

void	init_file_struct_with_fd(t_file *file_struct, int fd)
{
	init_file_struct(file_struct);
	file_struct->fd = fd;
}

t_file	*create_file_struct_with_filename(char *filename)
{
	t_file	*file_struct;
	char	*f;

	f = ft_strdup(filename);
	file_struct = malloc(sizeof(t_file));
	init_file_struct_with_filename(file_struct, f);
	return (file_struct);
}
