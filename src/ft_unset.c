/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 21:52:36 by Helene            #+#    #+#             */
/*   Updated: 2023/06/17 23:19:05 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// si la variable que veut unset n'a pas été export, ne fait rien et retourne 0
int ft_unset(t_cmd *cmd)
{
    int i;

    i = 1; // args[0] est le nom de la commande
    if (!cmd->val.args[i])
        return (EXIT_OK);
    while (cmd->val.args[i])
    {
        // détermine si la variable est dans l'environnement
        if (ht_search(cmd->env, cmd->val.args[i]))
            ht_delete(cmd->env, cmd->val.args[i]);
        if (is_in_export_history(cmd->export_history, cmd->val.args[i]))
            del_from_export_history(&cmd->export_history, cmd->val.args[i]);
        i++;
    }
    return (EXIT_OK); // dans auel cas retourne autre chose que 0 ?
}