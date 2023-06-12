/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 20:39:18 by Helene            #+#    #+#             */
/*   Updated: 2023/06/12 01:09:41 by Helene           ###   ########.fr       */
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
    int exit_status;

    i = 1;
    exit_status = 0;
    if (!cmd->val.args[i])
    {
        print_env(cmd->env);
        return (exit_status);
    }
}