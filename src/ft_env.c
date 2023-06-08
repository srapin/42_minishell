/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 20:39:18 by Helene            #+#    #+#             */
/*   Updated: 2023/06/07 16:02:00 by Helene           ###   ########.fr       */
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

void    ft_env()
{
    
}