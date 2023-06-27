/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_path_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 19:08:53 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/27 19:11:30 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Obtient le path complet (chemin absolu) depuis la racine
Néanmoins, des './' ou '../' peuvent encore être présents
*/
char	*get_full_path(t_ht_hash_table *env, char *arg_path)
{
	char	*pwd;

	pwd = *get_pwd(env);
	return (ft_strjoin(pwd, arg_path));
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
