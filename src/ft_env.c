/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 20:39:18 by Helene            #+#    #+#             */
/*   Updated: 2023/06/12 22:57:27 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    print_env(t_ht_hash_table *ht)
{
    int i;

    i = 0;
    while (i < ht->count)
    {
        if (ht->items[i])
            printf("%s=%s\n", ht->items[i]->key, ht->items[i]->value);   
        i++;
    }
}

/* A gerer sans options et sans arguments */
int    ft_env(t_cmd *cmd)
{
    dprintf(1, "coucou depuis ft_env()\n");
    if (cmd->val.args[1])
    {
        printf("Error : env : too many arguments given. Zero needed");
        // free everything (?)
        return (1); // quelle valeur retourner ?
    }
    print_env(cmd->env);
    return (EXIT_OK);
}