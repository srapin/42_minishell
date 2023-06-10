/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_pub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:11:02 by srapin            #+#    #+#             */
/*   Updated: 2023/06/10 22:22:15 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:11:02 by srapin            #+#    #+#             */
/*   Updated: 2023/06/10 21:41:25 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/priv.h"
#include "../inc/minishell.h"
//todo
void add_in_redir_with_file_struct(t_cmd *cmd, t_file *file_struct)
{
	t_list *lst;
	lst = ft_lstnew(file_struct);
	ft_lstadd_back(&(cmd->red.in_list), lst);
}

void add_out_redir_with_file_struct(t_cmd *cmd, t_file *file_struct)
{
	t_list *lst;
	lst = ft_lstnew(file_struct);
	ft_lstadd_back(&(cmd->red.out_list), lst);
	//dprintf(1, "in add_in_redir_with_file_struct : command name = %s\n", cmd->val.value);
	dprintf(1, "in adding red at end file name = %s\n", ((t_file *)(cmd->red.out_list)->content)->name);
}

// void add_err_redir_with_file_struct(t_cmd *cmd, t_file *file_struct)
// {
// 	cmd->red.err_type = fd;
// 	cmd->red.err_content = file_struct;
// 	if (file_struct->fd > -1)
// 		cmd->red.err_fd = file_struct->fd;
// }

void init_redirections(t_redirect *red)
{
	dprintf(1, "init_red\n");
	red->in_list = NULL;
	red->out_list = NULL;
	red->in_fd = -1;
	red->out_fd = -1;
	red->next_cmd = NULL;
}

t_redirect *create_redir()
{
	t_redirect *red;

	red = malloc(sizeof(red));
	init_redirections(red);
	return red;
}