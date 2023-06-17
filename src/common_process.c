/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:44:39 by srapin            #+#    #+#             */
/*   Updated: 2023/06/17 23:50:52 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	parent_process(t_cmd **cmd, int pipe_tab[2])
{
	
	safe_close(&(pipe_tab[1]));
	if (!cmd || !(*cmd))
		return;
	safe_close_cmd_fd(*cmd);
	//if ((*cmd)->red.in_type == fd && ((t_file *) ((*cmd)->red.in_content))->sep)
		//close
	// t_cmd ** tmp;
	// tmp = cmd;
	*cmd = (*cmd)->red.next_cmd;
	// free_cmd(tmp);
	////dprintf(STDOUT_FILENO, "parent pro %s\n", (*cmd)->val.value);
	if (!cmd || !(*cmd))
		return;
	(*cmd)->red.in_fd = pipe_tab[0];
}

void	child_process(t_cmd *cmd, int pipe_tab[2])
{
	char	**paths;

	if (pipe_tab[0] > -1)
	{
		safe_close(&(pipe_tab[0]));
		cmd->red.out_fd = pipe_tab[1];
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	//dprintf(1, "in child proc outfile name = %s\n", ((t_file *)(cmd->red.out_list)->content)->name   );
	dup_cmd_file(cmd);
	//dprintf(1, "child_proc\n");
	
	// rajouté par ln
	try_to_exec_builtins(cmd, true);
	
	paths = get_path(cmd);
	if (check_acces(paths, cmd))
	{
	 	execve(cmd->val.path, cmd->val.args, cmd->val.env);
	}
	//perror("cmd not found");
	if (errno) // 2 : cmd not found. 13 : permission denied
	{
		if (errno == 2)
			printf("minishell : %s : command not found\n", cmd->val.value);
		else if (errno == 13)
			printf("minishell : %s : permission denied\n", cmd->val.value);
	}
	//printf("errno value : %d\n", errno);
	free_tab(paths);
	free_tab(cmd->val.args);
	exit(CMD_NOT_FOUND);
}

void	fail_process(void)
{
	perror("fork error");
}