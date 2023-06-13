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


void launch_process(t_cmd **cmd, int *pip_tab, bool need_pip)
{
	if (need_pip)
		safe_pipe(pip_tab);
	else
	{
		
		if (try_to_exec_builtins(*cmd, false)>= 0)
			return;
		reset_pip_tab(pip_tab);
	}
	(*cmd)->pid = fork();
	if ((*cmd)->pid < 0)
		fail_process();
	if ((*cmd)->pid == 0)
		child_process(*cmd, pip_tab);
	if ((*cmd)->pid > 0)
		parent_process(cmd, pip_tab);
}


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
	int foo(t_cmd *);

	cmd = first_cmd;
	////dprintf(1, "coucou test\n");
	//dprintf(1, "in exec, first command name = %s\n", cmd->val.value);
	
	while(cmd)
	{
		i = 0;
		nb_cmds = count_cmds(cmd);
		next = cmd->next;
		ret_cmd = cmd;

		while(i < nb_cmds)
		{
			launch_process(&cmd, pip_tab, (i < nb_cmds - 1));
			i++;
		}
		ret = wait_childs(ret_cmd);
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