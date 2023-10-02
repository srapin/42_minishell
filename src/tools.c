/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 23:01:51 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_strisequal(char *s1, char *s2)
{
	if (!s1 && !s2)
		return (true);
	if (!s1 || !s2)
		return (false);
	if (ft_strlen(s1) != ft_strlen(s2))
		return (false);
	if (!ft_strnstr(s1, s2, ft_strlen(s1)))
		return (false);
	return (true);
}

void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	add_slash(char **paths)
{
	int		i;
	char	*tmp;

	i = 0;
	while (paths && paths[i])
	{
		tmp = paths[i];
		paths[i] = ft_strjoin(tmp, "/");
		free(tmp);
		if (!paths[i])
		{
			while (paths[++i])
				free(paths[i]);
			free_tab(paths);
			return ;
		}
		i++;
	}
}

char	**get_path(t_cmd *cmd)
{
	int		i;
	char	**paths;
	char	**env;

	env = hash_map_to_tab(cmd->env);
	i = 0;
	paths = NULL;
	if (!env)
		perror("envp recuparation prob");
	while (env[i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5))
		{
			paths = ft_split(env[i] + 5 * sizeof(char), ':');
			break ;
		}
		i++;
	}
	add_slash(paths);
	free_tab(env);
	return (paths);
}
