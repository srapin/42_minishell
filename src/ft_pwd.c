/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 00:59:44 by Helene            #+#    #+#             */
/*   Updated: 2023/06/24 11:20:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Quel que soit le nombre d'arguments donnés, il printera dans
tous les cas le chemin absolu vers le dossier courant */
// quels cas d'erreur à gérer ?????
char	**get_pwd(t_ht_hash_table *env)
{
	static char	*pwd; 

	if (!pwd)
		pwd = ft_strdup(ht_search(env, "PWD"));
	return (&pwd);
}

void	update_pwd(t_ht_hash_table *env, char *new_pwd)
{
	char	**pwd;

	pwd = get_pwd(env);
	free(*pwd);
	*pwd = ft_strdup(new_pwd);
}

void	free_pwd(t_ht_hash_table *env)
{
	char	**pwd;

	pwd = get_pwd(env);
	free(*pwd);
	*pwd = NULL;
}

int	ft_pwd(t_cmd *cmd, t_cmd *first)
{
	// char        *path;

	// path =
	// getcwd(path);
	// printf("%s\n", path);
	(void) first;
	printf("%s\n", *get_pwd(cmd->env));
	//printf("%s\n", ht_search(cmd->env, "PWD"));
	return (EXIT_OK);
}