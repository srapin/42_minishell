/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ht_delete.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 17:29:43 by Helene            #+#    #+#             */
/*   Updated: 2023/06/18 23:29:50 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_ht_item HT_DELETED_ITEM = {NULL, NULL};

/* Free the memory allocated for the t_ht_item *item*/
void    ht_del_item(t_ht_item *item)
{
    if (item != &HT_DELETED_ITEM) // item->key && item->value
    {
        free(item->key);
        free(item->value);
        free(item);
        item = NULL;
    }
}

/* Delete the entire hash table */
void    ht_del_hash_table(t_ht_hash_table *ht)
{
    size_t      i;
    t_ht_item   *current;
    
    i = 0;
    while (i < ht->size)
    {
        current = ht->items[i];
        if (current && current != &HT_DELETED_ITEM)
            ht_del_item(current);
        i++;
    }
    free(ht->items);
    //ht->items = NULL;
    free(ht);
    //ht = NULL;
}

/* Deletes the key-value pair stored in the hash table */
void    ht_delete(t_ht_hash_table *ht, const char *key)
{
    size_t      attempts;
    size_t      index;
    size_t      load;
    t_ht_item   *current;
    
    load = ht->count * 100 / ht->size;
    if (load < 10)
        ht_resize(ht, ht->base_size / 2); // resize down
    attempts = 0;
    index = ht_get_hash(key, ht->size, attempts);
    current = ht->items[index];
    while (current != NULL)
    {
        if (current != &HT_DELETED_ITEM && !ft_strcmp(current->key, key))
        {
            ht_del_item(current);
            ht->items[index] = &HT_DELETED_ITEM;
           /*  ht->items[index]->key = NULL;
            ht->items[index]->value = NULL; */ // = &HT_DELETED_ITEM; 
            ht->count--;
            return ; // rajoute apres, a enlever ?
        }
        index = ht_get_hash(key, ht->size, attempts);
        current = ht->items[index];
        attempts++;
    }
}
