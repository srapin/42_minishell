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

#include "minishell.h"

bool	err_while_open_files(t_file *f_s, int *to_rep)
{
	char	*err_mess;

	err_mess = ft_strjoin("minishell: ", f_s->name);
	if (errno == ENOENT)
		g_exit_status = CMD_NOT_FOUND;
	else if (errno == 13)
		g_exit_status = NO_FILE_OR_DIR;
	else if (errno == EISDIR)
		g_exit_status = CMD_NOT_EXECUTABLE;
	else
		g_exit_status = CMD_NOT_FOUND;
	perror(err_mess);
	safe_close(to_rep);
	free(err_mess);
	return (false);
}

bool	replace_fd(t_file *f_s, int *to_rep)
{
	int		new_fd;

	new_fd = *to_rep;
	if (f_s->fd > -1)
		new_fd = f_s->fd;
	else if (f_s->name)
	{
		if (f_s->out && access(f_s->name, F_OK) != 0)
			new_fd = open(f_s->name, f_s->flag, S_IRWXU);
		else
			new_fd = open(f_s->name, f_s->flag);
		if (new_fd < 0)
		{
			return (err_while_open_files(f_s, to_rep));
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
	int		*to_rep;

	flag = true;
	tmp_lst = cmd->red.files;
	while (tmp_lst && flag)
	{
		tmp_file = tmp_lst->content;
		if (tmp_file->out)
			to_rep = &(cmd->red.out_fd);
		else
			to_rep = &(cmd->red.in_fd);
		if (tmp_file)
			flag = replace_fd(tmp_file, to_rep);
		if (!flag)
		{
			safe_close(&(cmd->red.out_fd));
			safe_close(&(cmd->red.in_fd));
		}
		tmp_lst = tmp_lst->next;
	}
	return (flag);
}
