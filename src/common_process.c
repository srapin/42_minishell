/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:02:42 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_process(t_cmd **cmd, int pipe_tab[2])
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	safe_close(&(pipe_tab[1]));
	if (!cmd || !(*cmd))
		return ;
	safe_close_cmd_fd(*cmd);
	*cmd = (*cmd)->red.next_cmd;
	if (!cmd || !(*cmd))
		return ;
	(*cmd)->red.in_fd = pipe_tab[0];
}

void	launch_execve(t_cmd *cmd, t_cmd *first)
{
	char	*path;
	char	**args;
	char	**env;

	path = cmd->val.path;
	env = hash_map_to_tab(cmd->env);
	args = cmd->val.args;
	cmd->val.args = NULL;
	cmd->val.path = NULL;
	free_cmds(&first, true);
	signal(SIGQUIT, SIG_DFL);
	execve(path, args, env);
	signal(SIGQUIT, SIG_IGN);
	free(path);
	free_tab(env);
	free_tab(args);
	perror("exceve failed : abort");
	exit(CMD_NOT_FOUND);
}

void	child_process(t_cmd *cmd, t_cmd *first, int pipe_tab[2])
{
	if (pipe_tab[0] > -1)
	{
		safe_close(&(pipe_tab[0]));
		cmd->red.out_fd = pipe_tab[1];
	}
	reset_defaults_signals();
	if (!dup_cmd_file(cmd))
		failed_to_open_files(first);
	if (!cmd->val.value)
	{
		free_cmds(&first, true);
		exit(EXIT_SUCCESS);
	}
	try_to_exec_builtins(cmd, first, true);
	if (check_acces(cmd, first))
		launch_execve(cmd, first);
	not_found(cmd, first);
}

void	fail_process(void)
{
	perror("fork error");
}
