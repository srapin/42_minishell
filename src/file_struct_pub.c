/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_struct_pub.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:08:11 by srapin            #+#    #+#             */
/*   Updated: 2023/05/23 02:23:08 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/priv.h"

void init_file_struct_with_filename(t_file *file_struct, char *filename)
{
	init_file_struct(file_struct);
	file_struct->name = filename;
}

void init_file_struct_with_sep(t_file *file_struct, char *sep)
{
	init_file_struct(file_struct);
	file_struct->sep = sep;
}

void init_file_struct_with_fd(t_file *file_struct, int fd)
{
	init_file_struct(file_struct);
	file_struct->fd = fd;
}

t_file *create_file_struct_with_filename(char *filename)
{
	t_file *file_struct;

	file_struct = malloc(sizeof(t_file));
	init_file_struct_with_filename(file_struct, filename);
	return file_struct;
}

t_file * create_file_struct_with_sep(char *sep)
{
	t_file *file_struct;

	file_struct = malloc(sizeof(t_file));
	init_file_struct_with_sep(file_struct, sep);
	return file_struct;
}

t_file * create_file_struct_with_fd(int fd)
{
	t_file *file_struct;

	file_struct = malloc(sizeof(t_file));
	init_file_struct_with_fd(file_struct, fd);
	return file_struct;
}


void open_cmd_files(t_cmd * cmd)
{
	if (cmd->red.in_type == fd && !((t_file *) (cmd->red.in_content))->sep)
		set_fd(cmd->red.in_content, &(cmd->red.in_fd), O_RDONLY);
	if (cmd->red.out_type == fd)
		set_fd(cmd->red.out_content, &(cmd->red.out_fd), O_WRONLY);
	if (cmd->red.err_type == fd)
		set_fd(cmd->red.err_content, &(cmd->red.err_fd), O_WRONLY);
}