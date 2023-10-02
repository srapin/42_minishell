/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 16:09:20 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "hash_table.h"
#include "minishell.h"

/*
Open Addressing :
when a data item cannot be placed at the index 
calculated by the hash function, 
another location in the aray is sought.

We use open accessing with double hashing's 
technique to handle collisions.
The double hashing technique uses two hash functions 
to calculate an item's index in
the hash table after n collisions.

To minimize collisions, it is important to reduce 
the number of common factors 
between 𝑚 (here num_buckets) and the elements 
of 𝐾 (the set of keys). 
How can this be achieved? By choosing 𝑚 to be a 
number that has very few factors: 
a prime number.

Why implementing a dynamic resizing of the hash table ?
    - The hash table's performance diminishes with 
	  high rates of collisions
    - Without that, the insert function will fail 
	  when trying to store more than the initial
	  fixed size of items
Resizing :  up if load > 0.7
            down if load < 0.1

*/

t_ht_hash_table	*ht_new(size_t size)
{
	t_ht_hash_table	*ht_new;

	ht_new = ft_calloc(sizeof(t_ht_hash_table), 1);
	if (!ht_new)
		return (NULL);
	ht_new->count = 0;
	ht_new->base_size = size;
	ht_new->size = next_prime(size);
	ht_new->items = ft_calloc(sizeof(t_ht_item), ht_new->size);
	if (!ht_new->items)
	{
		free(ht_new);
		ht_new = NULL;
		return (NULL);
	}
	return (ht_new);
}

t_ht_item	*ht_new_item(char *key, char *value)
{
	t_ht_item	*new;

	new = ft_calloc(sizeof(t_ht_item), 1);
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	return (new);
}

int	set_env_var(t_ht_item *item, char **env, int i)
{
	char	*tmp;

	tmp = ft_strjoin(item->key, "=");
	env[i] = ft_strjoin(tmp, item->value);
	if (!env[i])
	{
		perror("malloc ");
		free_tab(env);
		return (0);
	}
	free(tmp);
	return (1);
}

char	**hash_map_to_tab(t_ht_hash_table *ht)
{
	size_t				i;
	size_t				j;
	char				**env;
	static t_ht_item	ht_deleted_item = {NULL, NULL};

	env = ft_calloc(sizeof(char *), (ht->count + 1));
	if (!env)
	{
		perror("malloc ");
		return (NULL);
	}
	i = 0;
	j = 0;
	while (i < ht->count && j < ht->size)
	{
		while (j < ht->size && (!ht->items[j]
				|| ht->items[j] == &ht_deleted_item))
			j++;
		if (!set_env_var(ht->items[j], env, i))
			return (NULL);
		i++;
		j++;
	}
	env[i] = NULL;
	return (env);
}
