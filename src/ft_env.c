/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 20:39:18 by Helene            #+#    #+#             */
/*   Updated: 2023/06/12 13:12:37 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    print_env(t_ht_hash_table *ht)
{
    int i;

    i = 0;
    while (ht->items[i])
    {
        printf("%s=%s\n", ht->items[i]->key, ht->items[i]->value);   
        i++;
    }
}

int    ft_env(t_cmd *cmd)
{
    int i;

    i = 1;
    dprintf(1, "coucou depuis ft_env()\n");
    if (!cmd->val.args[i])
    {
        print_env(cmd->env);
        exit(EXIT_OK);
    }
    // que faire dans le cas ou a des arguments ??
    exit(EXIT_OK);
}