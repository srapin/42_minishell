/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 00:57:49 by Helene            #+#    #+#             */
/*   Updated: 2023/06/12 19:06:45 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* L'argument donné à exit */
void ft_exit(t_cmd *cmd) // void ou int ?
{
    if (!cmd->val.args[1]) // ie aucun argument
    {
       ; // que faire
    }
    
}