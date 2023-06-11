/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 21:52:36 by Helene            #+#    #+#             */
/*   Updated: 2023/06/11 23:41:05 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// si la variable a unset n'a pas été export, ne fait rien et retourne 0
int ft_unset(t_cmd *cmd)
{
    int i;

    i = 1; // args[0] est le nom de la commande
    if (!cmd->val.args[i])
        return (EXIT_OK);
    while (cmd->val.args[i])
    {
        
    }
}