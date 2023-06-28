/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapi <rapi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:36:03 by srapin            #+#    #+#             */
/*   Updated: 2023/06/20 01:33:24 by rapi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safe_close(int *fd)
{
	if (*fd > 2)
	{
		close(*fd);
		*fd = -1;
	}
}

void	safe_pipe(int pipe_tab[2])
{
	if (pipe(pipe_tab) == 2)
	{
		perror("pipe creation failed");
		exit(EXIT_FAILURE);
	}
}

void	safe_close_cmd_fd(t_cmd *cmd)
{
	safe_close(&(cmd->red.in_fd));
	safe_close(&(cmd->red.out_fd));
}

bool	check_acces(t_cmd *cmd, t_cmd *first)
{
	if (!value_is_not_null(cmd))
		return (false);
	if (try_to_find_path(cmd))
		return (true);
	if (is_dir(cmd->val.value))
	{
		errno = EISDIR;
		return (false);
	}
	if (look_like_a_cmd(cmd))
		cmd_not_found(cmd, first);
	if (access(cmd->val.value, X_OK) != 0)
		return (false);
	cmd->val.path = ft_strdup(cmd->val.value);
	return (true);
}

void	reset_pip_tab(int pip_tab[2])
{
	pip_tab[0] = -1;
	pip_tab[1] = -1;
}
