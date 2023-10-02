/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:50:05 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exit_status(int status, int f_status)
{
	if (WIFEXITED(status))
		f_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		printf("procces stop with sig %d\n", WTERMSIG(status));
		f_status = WTERMSIG(status) + 128;
	}
	else if (WIFSTOPPED(status))
	{
		printf("procces stop with sig %d\n", WTERMSIG(status));
		f_status = WSTOPSIG(status) + 128;
	}
	return (f_status);
}

void	wait_childs(t_cmd *origin)
{
	int		status;
	int		f_status;
	int		ret;
	t_cmd	*cmd;

	cmd = origin;
	ret = g_exit_status;
	f_status = g_exit_status;
	status = 0;
	while (cmd)
	{
		if (cmd->pid < 0)
		{
			f_status = ret;
			cmd = cmd->red.next_cmd;
			continue ;
		}
		waitpid(cmd->pid, &status, 0);
		cmd = cmd->red.next_cmd;
		f_status = get_exit_status(status, f_status);
	}
	g_exit_status = f_status;
}

bool	check_ret(t_cmd *cmd, int ret)
{
	if (cmd->ctrl == pointvirgule)
		return (true);
	else if (cmd->ctrl == and)
	{
		if (ret == 0)
			return (true);
		return (false);
	}
	if (ret != 0)
		return (true);
	return (false);
}
