/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 23:56:51 by srapin            #+#    #+#             */
/*   Updated: 2023/06/20 00:51:15 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/priv.h"
#include "../inc/minishell.h"

void mask_and_free_for_exec(t_cmd *cmd, t_cmd *first)
{
	cmd->val.args = NULL;
	cmd->val.path = NULL;
	// cmd->val.env = NULL;
	free_cmds(&first, true);
}

int count_cmds_linked_by_pipe(t_cmd *first_cmd)
{
	int nb;
	t_cmd *cmd;

	cmd = first_cmd;
	nb = 1;
	while(cmd && cmd->red.next_cmd)
	{
		nb++;
		cmd = cmd->red.next_cmd;
	}
	return nb;
}

void dup_cmd_file(t_cmd *cmd)
{
	open_cmd_files(cmd);
	if (cmd->red.in_fd > -1)
	{
		dup2(cmd->red.in_fd, STDIN_FILENO);
		safe_close(&(cmd->red.in_fd));
	}
	if (cmd->red.out_fd > -1)
	{
		dup2(cmd->red.out_fd, STDOUT_FILENO);
		safe_close(&(cmd->red.out_fd));
	}
}