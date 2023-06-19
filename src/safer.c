/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:36:03 by srapin            #+#    #+#             */
/*   Updated: 2023/06/19 17:43:48 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	safe_close(int *fd)
{
	if (*fd > -1)
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

void safe_close_cmd_fd(t_cmd *cmd)
{
	safe_close(&(cmd->red.in_fd));
	safe_close(&(cmd->red.out_fd));
}

bool	check_acces(char **paths, t_cmd *cmd)
{
	int		i;
	char	*path;

	i = 0;
	while (paths && paths[i])
	{
		path = ft_strjoin(paths[i], get_cmd_value(cmd));
		if (path && access(path, X_OK) == 0)
		{
			cmd->val.path = path;
			return (true);
		}
		free(path);
		i++;
	}
	if (access(cmd->val.value, X_OK) != 0)
		return (false);
	cmd->val.path = ft_strdup(cmd->val.value);
	return (true);
}

void reset_pip_tab(int pip_tab[2])
{
	pip_tab[0] = -1;
	pip_tab[1] = -1;
}