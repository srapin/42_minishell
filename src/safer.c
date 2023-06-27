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

void safe_close_cmd_fd(t_cmd *cmd)
{
	safe_close(&(cmd->red.in_fd));
	safe_close(&(cmd->red.out_fd));
}


int is_dir(const char *path)
{
    struct stat path_stat;
	ft_memset(&path_stat, 0, sizeof(struct stat));
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

bool	value_is_not_null(t_cmd *cmd)
{
	if (!cmd->val.value || !cmd->val.value[0])
	{
		errno = ENOENT;
		return false;
	}
	return true;
}

bool look_like_a_cmd(t_cmd *cmd)
{
	return (!(ft_strnstr(cmd->val.value, "./", 2)) && !ft_strnstr(cmd->val.value, "/", 1));
}


bool	check_acces(t_cmd *cmd, t_cmd *first )
{
	int		i;
	char	*path;
	char	**paths;

	i = 0;
	if (!value_is_not_null(cmd))
		return (false);
	paths = get_path(cmd);
	while (paths && paths[i] && cmd->val.value)
	{
		path = ft_strjoin(paths[i], cmd->val.value);
		if (path && access(path, X_OK) == 0)
		{
			cmd->val.path = path;
			free_tab(paths);
			return (true);
		}
		free(path);
		i++;
	}
	free_tab(paths);
	if (is_dir(cmd->val.value))
	{
		errno = EISDIR;
		return false;
	}
	if (look_like_a_cmd(cmd))
		cmd_not_found(cmd, first);
	if (access(cmd->val.value, X_OK) != 0)
		return (false);
	(void) first;
	cmd->val.path = ft_strdup(cmd->val.value);
	return (true);
}

void reset_pip_tab(int pip_tab[2])
{
	pip_tab[0] = -1;
	pip_tab[1] = -1;
}

// #include "minishell.h"

// void	safe_close(int *fd)
// {
// 	if (*fd > 2)
// 	{
// 		close(*fd);
// 		*fd = -1;
// 	}
// }

// void	safe_pipe(int pipe_tab[2])
// {
// 	if (pipe(pipe_tab) == 2)
// 	{
// 		perror("pipe creation failed");
// 		exit(EXIT_FAILURE);
// 	}
// }

// void	safe_close_cmd_fd(t_cmd *cmd)
// {
// 	safe_close(&(cmd->red.in_fd));
// 	safe_close(&(cmd->red.out_fd));
// }

// bool	check_acces(t_cmd *cmd, t_cmd *first)
// {
// 	if (!value_is_not_null(cmd))
// 		return (false);
// 	try_to_find_path(cmd);
// 	if (is_dir(cmd->val.value))
// 	{
// 		errno = EISDIR;
// 		return (false);
// 	}
// 	if (look_like_a_cmd(cmd))
// 		cmd_not_found(cmd, first);
// 	if (access(cmd->val.value, X_OK) != 0)
// 		return (false);
// 	(void)first;
// 	cmd->val.path = ft_strdup(cmd->val.value);
// 	return (true);
// }

// void	reset_pip_tab(int pip_tab[2])
// {
// 	pip_tab[0] = -1;
// 	pip_tab[1] = -1;
// }
