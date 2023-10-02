/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_path_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 19:08:53 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/28 15:45:50 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Obtient le path complet (chemin absolu) depuis la racine
Néanmoins, des './' ou '../' peuvent encore être présents
*/
char	*get_full_path(t_ht_hash_table *env, char *arg_path)
{
	char	pwd[PATH_MAX];
	char	*tmp;
	char	*final_pwd;

	(void)env;
	if (!getcwd(pwd, PATH_MAX))
	{
		perror("getcwd");
		return (NULL);
	}
	tmp = ft_strjoin(pwd, "/");
	final_pwd = ft_strjoin(tmp, arg_path);
	free(tmp);
	return (final_pwd);
}

void	set_path(char **path, char *before, char *after)
{
	free(*path);
	*path = ft_strjoin(before, after);
	free(before);
	free(after);
	before = NULL;
	after = NULL;
}

void	curr_dir(char **path, int i)
{
	char	*before;
	char	*after;

	before = ft_substr(*path, 0, i);
	after = ft_substr(*path, i + 2, ft_strlen(*path));
	set_path(path, before, after);
}
