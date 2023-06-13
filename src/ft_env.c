/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 20:39:18 by Helene            #+#    #+#             */
/*   Updated: 2023/06/14 00:04:03 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    print_env(t_ht_hash_table *ht)
{
    int i;
    t_ht_item *item;

    i = 0;
    while (i < ht->size)
    {
        item = ht->items[i];
        if (ht->items[i] && ht->items[i]->key && ht->items[i]->value)
        {
            printf("%s=%s\n", ht->items[i]->key, ht->items[i]->value);
        }
        i++;
    }
}

/* A gerer sans options et sans arguments */
int    ft_env(t_cmd *cmd)
{
    //dprintf(1, "coucou depuis ft_env()\n");
    if (cmd->val.args[1])
    {
        printf("Error : env : too many arguments given. Zero needed");
        // free everything (?)
        return (1); // quelle valeur retourner ?
    }
    if (!cmd->val.args[1])
        print_env(cmd->env);
    return (EXIT_OK);
}