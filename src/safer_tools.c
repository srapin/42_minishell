/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safer_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 22:46:31 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_dir(const char *path)
{
	struct stat	path_stat;

	ft_memset(&path_stat, 0, sizeof(struct stat));
	stat(path, &path_stat);
	return (S_ISDIR(path_stat.st_mode));
}

bool	value_is_not_null(t_cmd *cmd)
{
	if (!cmd->val.value || !cmd->val.value[0])
	{
		errno = ENOENT;
		return (false);
	}
	return (true);
}

bool	look_like_a_cmd(t_cmd *cmd)
{
	return (!(ft_strnstr(cmd->val.value, "./", 2))
		&& !ft_strnstr(cmd->val.value, "/", 1));
}

bool	try_to_find_path(t_cmd *cmd)
{
	char	**paths;
	int		i;
	char	*path;

	i = 0;
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
	return (false);
}
