/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_pub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:11:02 by srapin            #+#    #+#             */
/*   Updated: 2023/05/23 01:59:00 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/priv.h"
#include "../inc/minishell.h"
//todo
void add_in_redir_with_file_struct(t_cmd *cmd, t_file *file_struct)
{
	cmd->red.in_type = fd;
	cmd->red.in_content = file_struct;
	if (file_struct->fd > -1)
		cmd->red.in_fd = file_struct->fd;
}

void add_out_redir_with_file_struct(t_cmd *cmd, t_file *file_struct)
{
	cmd->red.out_type = fd;
	cmd->red.out_content = file_struct;
	if (file_struct->fd > -1)
		cmd->red.out_fd = file_struct->fd;
}

void add_err_redir_with_file_struct(t_cmd *cmd, t_file *file_struct)
{
	cmd->red.err_type = fd;
	cmd->red.err_content = file_struct;
	if (file_struct->fd > -1)
		cmd->red.err_fd = file_struct->fd;
}

void init_redirections(t_redirect *red)
{
	red->in_type = noneu;
	red->in_fd = -1;
	red->in_content = NULL;
	
	red->out_type = noneu;
	red->out_fd = -1;
	red->out_content = NULL;
	
	red->err_type = noneu;
	red->err_fd = -1;
	red->err_content = NULL;
}

t_redirect *create_redir()
{
	t_redirect *red;

	red = malloc(sizeof(red));
	init_redirections(red);
	return red;
}