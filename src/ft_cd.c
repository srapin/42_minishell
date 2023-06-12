/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 01:12:19 by Helene            #+#    #+#             */
/*   Updated: 2023/06/12 02:01:24 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* cd with only a relative or absolute path 
-> est-ce que ~/path_name est un chemin relatif ?  */
/*
Quelles erreurs peut-on avoir a gérer ? 
*/
int     ft_cd(t_cmd *cmd)
{
    char    *pwd;
    char    *after_rel;
    char    *new_pwd;

    if (!cmd->val.args[1]) // si n'a aucun argument
        return (EXIT_OK);
    if (cmd->val.args[2]) // si il y a plus d'un argument. val.args est null-terminated
        return (CD_TOO_MANY_ARGS);
    if (cmd->val.args[1][0] == '.') // chemin relatif
    {
        pwd = ft_strdup(ht_search(cmd->env, "PWD"));
        after_rel = ft_substr(cmd->val.args[1], 1, ft_strlen(cmd->val.args[1]));
        new_pwd = ft_strjoin(pwd, after_rel);
        ht_modify_value(cmd->env, "PWD", new_pwd);
        free(pwd);
        free(after_rel);
        pwd = NULL;
        after_rel = NULL;
    }
    else // chemin absolu
    {
        new_pwd = ft_strdup(cmd->val.args[1]);
        ht_modify_value(cmd->env, "PWD", new_pwd);
    }
    return (EXIT_OK);
}