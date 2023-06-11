/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:50:05 by srapin            #+#    #+#             */
/*   Updated: 2023/06/11 23:50:55 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/minishell.h"

int	wait_childs(int size, int *childs_pid)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < size && __WIFSIGNALED(status))
	{
		waitpid(childs_pid[i], &status, 0);
		//if ()
		i++;
	}
	free(childs_pid);
	// if (WIFEXITED(status) && __WIFSIGNALED(status))
	// 	g_exit_status = WEXITSTATUS(status);

		
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