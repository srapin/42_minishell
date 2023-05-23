/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_struct_priv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:08:04 by srapin            #+#    #+#             */
/*   Updated: 2023/05/23 02:15:51 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void init_file_struct(t_file *file_struct)
{
	file_struct->name = NULL;
	file_struct->fd = -1;
	file_struct->sep = NULL;
}

void set_fd(t_file * f_struct, int *fd, int flag)
{
	//heredoc?
	if (access(f_struct->name, flag) != 0)
	{
		perror("cannot open file");
		exit(EXIT_FAILURE);
	}
	if (f_struct->name && f_struct->fd < 0)
		f_struct->fd = open(f_struct->name, flag);
	if (f_struct->fd > -1)
		*fd = f_struct->fd;
}