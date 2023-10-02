/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ht_delete.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 17:29:43 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Verifier le fonctionnement !!! */
void	ht_del_item(t_ht_item *item)
{
	if (item->key || item->value)
	{
		if (item->key)
			free(item->key);
		if (item->value)
			free(item->value);
		if (item)
			free(item);
		item = NULL;
	}
}

/* Delete the entire hash table */
void	ht_del_hash_table(t_ht_hash_table *ht)
{
	size_t		i;
	t_ht_item	*current;

	i = 0;
	while (i < ht->size)
	{
		current = ht->items[i];
		if (current && (current->key || current->value))
			ht_del_item(current);
		i++;
	}
	free(ht->items);
	ht->items = NULL;
	free(ht);
	ht = NULL;
}

/* Deletes the key-value pair stored in the hash table */
void	ht_delete(t_ht_hash_table *ht, const char *key)
{
	size_t				attempts;
	size_t				index;
	size_t				load;
	t_ht_item			*current;
	static t_ht_item	ht_deleted_item = {NULL, NULL};

	load = ht->count * 100 / ht->size;
	if (load < 10)
		ht_resize(ht, ht->base_size / 2);
	attempts = 0;
	index = ht_get_hash(key, ht->size, attempts);
	current = ht->items[index];
	while (current != NULL)
	{
		if (current != &ht_deleted_item && !ft_strcmp(current->key, key))
		{
			ht_del_item(current);
			ht->items[index] = &ht_deleted_item;
			ht->count--;
			return ;
		}
		index = ht_get_hash(key, ht->size, attempts);
		current = ht->items[index];
		attempts++;
	}
}
