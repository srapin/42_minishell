/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:50:05 by srapin            #+#    #+#             */
/*   Updated: 2023/06/24 11:28:34 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/minishell.h"


// oid wait_for_childs(t_data    *d)
// {
//     int    i;
//     int    w;
//     int status;

//     i = 0;
//     status = 0;
//     while (i < d->cmds_nb && d->pid[i])
//     {
//         w = waitpid(d->pid[i], &status, WUNTRACED | WCONTINUED);
//         if (w == -1) {
//             perror("waitpid");
//             //exit(EXIT_FAILURE);
//         }
//         if (WIFEXITED(status)) {
//             printf("exited, status=%d\n", WEXITSTATUS(status));
//             status = WEXITSTATUS(status);
//         } else if (WIFSIGNALED(status)) {
//             printf("killed by signal %d\n", WTERMSIG(status));
//             status = WTERMSIG(status);
//         } else if (WIFSTOPPED(status)) {
//             printf("stopped by signal %d\n", WSTOPSIG(status));
//             WSTOPSIG(status);
//         } else if (WIFCONTINUED(status)) {
//             printf("continued\n");
//         }
//         i ++;
//     }
//     keep_exit_status(status);
// }

void	wait_childs(t_cmd *origin)
{
	int	status;
	int f_status;
	int ret;
	t_cmd *cmd;

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
			continue;
		}
		waitpid(cmd->pid, &status, 0);
		cmd = cmd->red.next_cmd;
		if (WIFEXITED(status))
			f_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status)) 
            f_status = WTERMSIG(status) + 128;// + 128;
		else if (WIFSTOPPED(status))
            f_status = WSTOPSIG(status) + 128;
	}
	// if (WIFEXITED(status) && __WIFSIGNALED(status))
	// 	g_exit_status = WEXITSTATUS(status);

	g_exit_status = f_status;
	// return ((WIFEXITED(status) && WEXITSTATUS(status))); // WIFSTOPPED
}

bool check_ret(t_cmd *cmd, int ret)
{
	if (cmd->ctrl == pointvirgule)
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