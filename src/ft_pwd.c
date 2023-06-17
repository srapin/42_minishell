/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 00:59:44 by Helene            #+#    #+#             */
/*   Updated: 2023/06/17 23:35:58 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Quel que soit le nombre d'arguments donnés, il printera dans
tous les cas le chemin absolu vers le dossier courant */
// quels cas d'erreur à gérer ?????
path    *get_pwd(t_ht_hash_table *env)
{
    static path pwd = NULL;
    if (!pwd)
        pwd = ft_strdup(ht_search(env, "PWD"));
    return (&pwd);
}

void    update_pwd(t_ht_hash_table *env, char *new_pwd)
{
    char **pwd;
    pwd = get_pwd(env);
    free (*pwd);
    *pwd = ft_strdup(new_pwd); 
}

void    free_pwd(t_ht_hash_table *env)
{
    char **pwd;
    pwd = get_pwd(env);
    free(*pwd);
    *pwd = NULL;
}




int     ft_pwd(t_cmd *cmd)
{
    // char        *path;

    // path = 
    // getcwd(path);
    // printf("%s\n", path);
    
    printf("%s\n", *get_pwd(cmd->env));
    //printf("%s\n", ht_search(cmd->env, "PWD"));
    return (EXIT_OK);
}