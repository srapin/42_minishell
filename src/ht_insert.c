/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ht_insert.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 17:25:42 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ht_hash(const char *key, const int prime_factor,
		const size_t num_buckets)
{
	long	hash;
	size_t	i;
	size_t	key_len;

	i = -1;
	hash = 0;
	key_len = ft_strlen(key);
	while (++i < key_len)
	{
		hash += (ft_pow(prime_factor, key_len - (i + 1)) * key[i]);
		hash %= num_buckets;
	}
	return ((size_t)hash);
}

size_t	ht_get_hash(const char *key, const size_t num_buckets, size_t attempts)
{
	size_t	hash_a;
	size_t	hash_b;

	hash_a = ht_hash(key, HT_PRIME_FACTOR_A, num_buckets);
	hash_b = ht_hash(key, HT_PRIME_FACTOR_B, num_buckets);
	return ((hash_a + (attempts * (hash_b + 1))) % num_buckets);
}

void	intervert_hts(t_ht_hash_table *ht, t_ht_hash_table *new_ht,
		int base_size)
{
	size_t		size_tmp;
	t_ht_item	**tmp;

	ht->base_size = base_size;
	ht->count = new_ht->count;
	size_tmp = ht->size;
	ht->size = new_ht->size;
	new_ht->size = size_tmp;
	tmp = ht->items;
	ht->items = new_ht->items;
	new_ht->items = tmp;
}

void	ht_resize(t_ht_hash_table *ht, size_t base_size)
{
	size_t			i;
	t_ht_item		*current;
	t_ht_hash_table	*new_ht;

	if (base_size < HT_INITIAL_SIZE)
		return ;
	i = 0;
	new_ht = ht_new(base_size);
	if (!new_ht)
		return ;
	while (i < ht->size)
	{
		current = ht->items[i];
		if (current && (current->key || current->value))
			ht_insert_item(new_ht, ft_strdup(current->key),
				ft_strdup(current->value));
		i++;
	}
	intervert_hts(ht, new_ht, base_size);
	ht_del_hash_table(new_ht);
}

void	ht_insert_item(t_ht_hash_table *ht, char *key, char *value)
{
	int			attempts;
	size_t		load;
	size_t		index;
	t_ht_item	*new_item;
	t_ht_item	*current;

	new_item = ht_new_item(key, value);
	if (!new_item)
		return ;
	load = ht->count * 100 / ht->size;
	if (load > 70)
		ht_resize(ht, ht->base_size * 2);
	attempts = 1;
	index = ht_get_hash(key, ht->size, 0);
	current = ht->items[index];
	while (current && (current->key || current->value))
	{
		index = ht_get_hash(key, ht->size, attempts);
		current = ht->items[index];
		attempts++;
	}
	ht->items[index] = new_item;
	ht->count++;
}
