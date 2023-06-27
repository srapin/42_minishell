/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_struct_pub.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:08:11 by srapin            #+#    #+#             */
/*   Updated: 2023/06/21 03:42: by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/priv.h"

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

bool	replace_fd(t_file *f_s, int *to_rep, bool out)
{
	int		new_fd;
	char	*err_mess;

	new_fd = *to_rep;
	if (f_s->fd > -1)
		new_fd = f_s->fd;
	else if (f_s->name)
	{
		if (out && access(f_s->name, F_OK) != 0)
			new_fd = open(f_s->name, f_s->flag, S_IRWXU);
		else
			new_fd = open(f_s->name, f_s->flag);
		if (new_fd < 0)
		{
			err_mess = ft_strjoin("minishell: ", f_s->name);
			perror(err_mess);
			safe_close(to_rep);
			free(err_mess);
			return (false);
		}
	}
	if (new_fd > -1 && *to_rep > -1 && *to_rep != new_fd)
		safe_close(to_rep);
	*to_rep = new_fd;
	return (true);
}

bool	open_cmd_files(t_cmd *cmd)
{
	t_list	*tmp_lst;
	t_file	*tmp_file;
	bool	flag;

	flag = true;
	tmp_lst = cmd->red.out_list;
	while (tmp_lst && flag)
	{
		tmp_file = tmp_lst->content;
		if (tmp_file)
			flag = replace_fd(tmp_file, &(cmd->red.out_fd), true);
		tmp_lst = tmp_lst->next;
	}
	tmp_lst = cmd->red.in_list;
	while (tmp_lst && flag)
	{
		tmp_file = tmp_lst->content;
		if (tmp_file)
			flag = replace_fd(tmp_file, &(cmd->red.in_fd), false);
		tmp_lst = tmp_lst->next;
	}
	return (flag);
}
