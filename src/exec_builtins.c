/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 20:57:20 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	(*get_builtins_foo(char *str))(t_cmd *cmd, t_cmd *first)
{
	if (ft_strisequal(str, "echo"))
		return (&ft_echo);
	if (ft_strisequal(str, "cd"))
		return (&ft_cd);
	if (ft_strisequal(str, "pwd"))
		return (&ft_pwd);
	if (ft_strisequal(str, "export"))
		return (&ft_export);
	if (ft_strisequal(str, "unset"))
		return (&ft_unset);
	if (ft_strisequal(str, "env"))
		return (&ft_env);
	if (ft_strisequal(str, "exit"))
		return (&ft_exit);
	return (NULL);
}

int	reset_files(int old_in, int old_out)
{
	if (old_in >= 0)
		dup2(old_in, STDIN_FILENO);
	if (old_out >= 0)
		dup2(old_out, STDOUT_FILENO);
	safe_close(&old_in);
	safe_close(&old_out);
	return (1);
}

void	quit_builtins_child(t_cmd *first, int ret)
{
	free_cmds(&first, true);
	exit(ret);
}

void	save_in_out(int *in, int *out)
{
	*in = dup(STDIN_FILENO);
	*out = dup(STDOUT_FILENO);
}

int	try_to_exec_builtins(t_cmd *cmd, t_cmd *first, bool is_child)
{
	int	(*foo)(t_cmd *, t_cmd *);
	int	ret;
	int	old_in;
	int	old_out;

	ret = -1;
	old_in = -1;
	old_out = -1;
	foo = get_builtins_foo(cmd->val.value);
	if (!foo)
		return (ret);
	if (!is_child)
	{
		if (foo != &ft_exit)
			save_in_out(&old_in, &old_out);
		if (!dup_cmd_file(cmd))
			return (reset_files(old_in, old_out));
	}
	ret = foo(cmd, first);
	if (is_child)
		quit_builtins_child(first, ret);
	g_exit_status = ret;
	reset_files(old_in, old_out);
	return (ret);
}
