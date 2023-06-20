/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:50:05 by srapin            #+#    #+#             */
/*   Updated: 2023/06/20 03:50:28 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/minishell.h"

int	wait_childs(t_cmd *origin)
{
	int	status;
	t_cmd *cmd;

	cmd = origin;

	status = 0;
	while (cmd)
	{
		waitpid(cmd->pid, &status, 0);
		cmd = cmd->red.next_cmd;
	}
	// if (WIFEXITED(status) && __WIFSIGNALED(status))
	// 	g_exit_status = WEXITSTATUS(status);

	g_exit_status = status % 255;//todo	
	return (WIFEXITED(status) && WEXITSTATUS(status));
}

bool check_ret(t_cmd *cmd, int ret)
{
	if (cmd->ctrl == none)
		return true;
	else if (cmd->ctrl == and)
	{
		if (ret == 0)
			return true;
		return false;
	}
	if (ret != 0)
		return true;
	return false;
}