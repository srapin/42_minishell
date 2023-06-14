/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ht_insert.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 17:25:42 by Helene            #+#    #+#             */
/*   Updated: 2023/06/14 17:34:16 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ht_item HT_DELETED_ITEM = {NULL, NULL};

size_t  ht_hash(const char *key, const int prime_factor, const size_t num_buckets)
{
    long hash;
    size_t i;
    size_t key_len;

    i = -1;
    hash = 0;
    key_len = ft_strlen(key);
    while (++i < key_len)
    {
        hash += (ft_pow(prime_factor, key_len - (i + 1)) * key[i]);
        hash %= num_buckets; // applying it at every steps allows to avoid overflows
    }
    return ((size_t)hash);
}

size_t  ht_get_hash(const char *key, const size_t num_buckets, size_t attempts)
{
    size_t hash_a;
    size_t hash_b;

    hash_a = ht_hash(key, HT_PRIME_FACTOR_A, num_buckets);
    hash_b = ht_hash(key, HT_PRIME_FACTOR_B, num_buckets);
    return ((hash_a + attempts * hash_b + 1) % num_buckets); // hash_b + 1 in case hash_b returns 0 ; this would indeed result in an infinite loop
}

void    ht_resize(t_ht_hash_table *ht, size_t base_size)
{
    int         i;
    size_t      size_tmp;
    t_ht_item   **tmp;
    t_ht_item   *current;
    t_ht_hash_table *new_ht;

    if (base_size < HT_INITIAL_SIZE)
        return ;
    i = 0;
    new_ht = ht_new(base_size);
    if (!new_ht)
        return ;
    while (i < ht->size)
    {
        current = ht->items[i];
        if (current && current != &HT_DELETED_ITEM)
            ht_insert_item(new_ht, ft_strdup(current->key), ft_strdup(current->value));
        i++;
    }

    ht->base_size = base_size;
    ht->count = new_ht->count;
    
    size_tmp = ht->size;
    ht->size = new_ht->size;
    new_ht->size = size_tmp;
    
    tmp = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp;
    ht_del_hash_table(new_ht);
}

void    ht_insert_item(t_ht_hash_table *ht, char *key, char *value)
{
    /* Implement hash functions hash_a and hash_b to compute indexes */
    int         attempts;
    size_t      load;
    size_t      index;
    t_ht_item   *new_item;
    t_ht_item   *current;

    new_item = ht_new_item(key, value);
    if (!new_item)
        return ;
    load = ht->count * 100 / ht->size;
    if (load > 70)
        ht_resize(ht, ht->base_size * 2); // resize up 
    attempts = 1;
    index = ht_get_hash(key, ht->size, 0);
    current = ht->items[index];
    // iterate through the hash table's indexes until we find an empty bucket, where we will be able to insert our new item
    while (current && current != &HT_DELETED_ITEM)
    {
        index = ht_get_hash(key, ht->size, attempts);
        current = ht->items[index];
        attempts++;
    }
    ht->items[index] = new_item;
    ht->count++;
}
