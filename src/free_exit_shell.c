/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit_shell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 16:37:37 by Helene            #+#    #+#             */
/*   Updated: 2023/06/10 17:07:03 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Free the memory allocated for the t_ht_item *item*/
void    ht_del_item(t_ht_item *item)
{
    if (!item)
        return ;
    free(item->key);
    free(item->value);
    free(item);
}

/* Delete the entire hash table */
void    ht_del_hash_table(t_ht_hash_table *ht)
{
    size_t     i;
    t_ht_item   *current;
    
    i = 0;
    while (i < ht->size)
    {
        current = ht->items[i];
        if (current)
            ht_del_item(current);
        i++;
    }
    free(ht->items);
    free(ht);
}