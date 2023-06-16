/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_struct_pub.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:08:11 by srapin            #+#    #+#             */
/*   Updated: 2023/06/16 12:13:25 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/priv.h"

void init_file_struct_with_filename(t_file *file_struct, char *filename)
{
	init_file_struct(file_struct);
	file_struct->name = filename;
}

// void init_file_struct_with_sep(t_file *file_struct, char *sep)
// {
// 	init_file_struct(file_struct);
// 	file_struct->sep = sep;
// }

void init_file_struct_with_fd(t_file *file_struct, int fd)
{
	init_file_struct(file_struct);
	file_struct->fd = fd;
}

t_file *create_file_struct_with_filename(char *filename)
{
	t_file *file_struct;
	char *f;

	f = ft_strdup(filename);
	file_struct = malloc(sizeof(t_file));
	init_file_struct_with_filename(file_struct, f);
	return file_struct;
}


// t_file * create_file_struct_with_sep(char *sep)
// {
// 	t_file *file_struct;

// 	file_struct = malloc(sizeof(t_file));
// 	init_file_struct_with_sep(file_struct, sep);
// 	return file_struct;
// }

t_file * create_file_struct_with_fd(int fd)
{
	t_file *file_struct;

	file_struct = malloc(sizeof(t_file));
	init_file_struct_with_fd(file_struct, fd);
	return file_struct;
}


void replace_fd(t_file *f_s, int *to_rep, bool out)
{
	int new_fd;

	//dprintf(1, "in replace_fd name = %s flag = %i, num = %i\n", f_s->name, f_s->flag, f_s->fd);
	new_fd = *to_rep;
	if (f_s->fd > -1)
		new_fd = f_s->fd;
	else if (f_s->name && out)
		new_fd = open(f_s->name, f_s->flag, S_IRWXU);
	else if (f_s->name)
		new_fd = open(f_s->name, f_s->flag);
	if (new_fd > -1 && *to_rep > -1 && *to_rep!= new_fd)
		safe_close(to_rep);
	*to_rep = new_fd;
}

void open_cmd_files(t_cmd * cmd)
{
	t_list *tmp_lst;
	t_file *tmp_file;

	tmp_lst = cmd->red.in_list;
	while(tmp_lst)
	{
		tmp_file = tmp_lst->content;
		if (tmp_file)
			replace_fd(tmp_file, &(cmd->red.in_fd), false);
		tmp_lst = tmp_lst->next;
	}
	tmp_lst = cmd->red.out_list;
	while(tmp_lst)
	{
		tmp_file = tmp_lst->content;
		if (tmp_file)
			replace_fd(tmp_file, &(cmd->red.out_fd), true);
		tmp_lst = tmp_lst->next;
	}
	// replace_fd(cmd->red.out, &(cmd->red.out_fd));

	/*
	if (cmd->red.in_type == fd)//&& !((t_file *) (cmd->red.in_content))->sep)
		set_fd(cmd->red.in_content, &(cmd->red.in_fd), O_RDONLY);
	if (cmd->red.out_type == fd)
		set_fd(cmd->red.out_content, &(cmd->red.out_fd), O_WRONLY);
	if (cmd->red.err_type == fd)
		set_fd(cmd->red.err_content, &(cmd->red.err_fd), O_WRONLY);
	*/
}