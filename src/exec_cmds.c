/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 02:05:40 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	launch_process(t_cmd **cmd, t_cmd *first, int *pip_tab, bool need_pip)
{
	if (need_pip)
		safe_pipe(pip_tab);
	else
		reset_pip_tab(pip_tab);
	if (first && !first->red.next_cmd)
	{
		if (try_to_exec_builtins(*cmd, first, false) >= 0)
		{
			(*cmd)->pid = -1;
			return ;
		}
	}
	(*cmd)->pid = fork();
	if ((*cmd)->pid < 0)
		fail_process();
	if ((*cmd)->pid == 0)
		child_process(*cmd, first, pip_tab);
	if ((*cmd)->pid > 0)
		parent_process(cmd, pip_tab);
}

void	exec_cmds(t_cmd *first_cmd)
{
	int			nb_cmds;
	int			pip_tab[2];
	t_cmd		*cmd;
	t_cmd		*next;
	t_cmd		*ret_cmd;

	cmd = first_cmd;
	while (cmd)
	{
		nb_cmds = count_cmds_linked_by_pipe(cmd);
		next = cmd->next;
		ret_cmd = cmd;
		while (nb_cmds-- > 0)
			launch_process(&cmd, first_cmd, pip_tab, nb_cmds);
		wait_childs(ret_cmd);
		signal(SIGINT, sigint_during_cmd_exec);
		while (!check_ret(ret_cmd, g_exit_status))
		{
			cmd = next;
			if (cmd)
				next = cmd->next;
			ret_cmd = cmd;
		}
		cmd = next;
	}
}
