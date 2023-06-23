/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:02:42 by srapin            #+#    #+#             */
/*   Updated: 2023/06/23 22:42:22 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

void	print_err(int e, t_cmd *cmd)
{
	char	*mess;

	//dprintf(1, "print_err=%s}\n", cmd->val.value);
	mess = ft_strjoin("minishell: ", cmd->val.value);
	perror(mess);
	free(mess);
}

void	reset_defaults_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	cmd_not_found(t_cmd *cmd, t_cmd *first)
{
	int	e;

	e = errno;
	//dprintf(1, "print_err=%s}\n", cmd->val.value);
	print_err(e, cmd);
	exit(CMD_NOT_FOUND);
}

void	failed_to_open_files(t_cmd *first)
{
	int	e;

	free_cmds(&first, true);
	exit(CMD_NOT_FOUND);
}

void	child_process(t_cmd *cmd, t_cmd *first, int pipe_tab[2])
{
	char	*path;
	char	**args;
	char	**env;
	char	**tmp;

	//dprintf(1, "child proc, val =%s}\n", cmd->val.value);
	if (pipe_tab[0] > -1)
	{
		safe_close(&(pipe_tab[0]));
		cmd->red.out_fd = pipe_tab[1];
	}
	reset_defaults_signals();
	if (!dup_cmd_file(cmd))
		failed_to_open_files(first);
	try_to_exec_builtins(cmd, first, true);
	if (check_acces(cmd))
	{
		//dprintf(1, "after check access\n");
		path = cmd->val.path;
		env = hash_map_to_tab(cmd->env);
		args = cmd->val.args;
		cmd->val.args = NULL;
		cmd->val.path = NULL;
		free_cmds(&first, true);
		execve(path, args, env);
		free(path);
		free_tab(env);
		free_tab(args);
		perror("exceve failed : abort");
		exit(CMD_NOT_FOUND);
	}
	//dprintf(1, "cmd not found\n");
	cmd_not_found(cmd, first);
}

void	fail_process(void)
{
	perror("fork error");
}
