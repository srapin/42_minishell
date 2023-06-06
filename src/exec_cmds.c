/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 22:47:25 by srapin            #+#    #+#             */
/*   Updated: 2023/05/13 02:12:22y srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void exec_cmds(t_cmd *first_cmd)
{
	int		nb_cmds;
	int 	i;
	int		pip_tab[2];
	int		*pid;
	t_cmd *cmd;
	t_cmd *next;
	t_cmd *ret_cmd;
	int ret;

	cmd = first_cmd;

	
	while(cmd)
	{
		i = 0;
		nb_cmds = count_cmds(cmd);
		pid = malloc(nb_cmds * sizeof(int));
		next = cmd->next;
		ret_cmd = cmd;
		while(i < nb_cmds)
		{
			if (i < nb_cmds - 1)
				safe_pipe(pip_tab);
			else
				reset_pip_tab(pip_tab);
			// if (cmd->red.in_type == fd && ((t_file *) (cmd->red.in_content))->sep)
			// 	heredoc(cmd); //faire ca ailleurs

			pid[i] = fork();
			if (pid[i] < 0)
				fail_process();
			if (pid[i] == 0)
				child_process(cmd, pip_tab, pid);
			if (pid[i] > 0)
				parent_process(&cmd, pip_tab);
			i++;
		}
		ret = wait_childs(nb_cmds, pid);
		while (!check_ret(ret_cmd, ret))
		{
			cmd = next;
			if (cmd)
				next = cmd->next;
			ret_cmd = cmd;
		}
		cmd = next;
	}
}