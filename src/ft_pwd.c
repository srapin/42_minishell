/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 00:59:44 by Helene            #+#    #+#             */
/*   Updated: 2023/06/12 13:12:54 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Quel que soit le nombre d'arguments donnés, il printera dans
tous les cas le chemin absolu vers le dossier courant */
// quels cas d'erreur à gérer ?????
int     ft_pwd(t_cmd *cmd)
{
    dprintf(1, "coucou depuis ft_pwd()\n");
    printf("%s\n", ht_search(cmd->env, "PWD"));
    return (EXIT_OK);
}