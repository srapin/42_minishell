/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 16:09:20 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/24 11:21:21 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash_table.h"

/*
Open Addressing :
when a data item cannot be placed at the index calculated by the hash function, 
another location in the aray is sought.

We use open accessing with double hashing's technique to handle collisions.
The double hashing technique uses two hash functions to calculate an item's index in
the hash table after n collisions.

To minimize collisions, it is important to reduce the number of common factors 
between 𝑚 (here num_buckets) and the elements of 𝐾 (the set of keys). 
How can this be achieved? By choosing 𝑚 to be a number that has very few factors: 
a prime number.

Why implementing a dynamic resizing of the hash table ?
    - The hash table's performance diminishes with high rates of collisions
    - Without that, the insert function will fail when trying to store more 
      than the initial fixed size of items
Resizing :  up if load > 0.7
            down if load < 0.1

*/

// ------------- A METTRE DANS /libft ---------------------

int	search(int low, int high, int nbr)
{
	int long	mid;

	mid = (low + high) / 2;
	if (low > high)
		return (0);
	if ((mid * mid) == nbr)
		return (mid);
	if ((mid * mid) < nbr && ((mid + 1) * (mid + 1)) > nbr)
		return (mid + 1);
	if ((mid * mid) < nbr)
		return (search(mid + 1, high, nbr));
	else
		return (search(low, mid, nbr));
}

int	ft_sqrt(int nbr)
{
	return (search(0, nbr, nbr));
}

int	search_prime(int nb, int sqrt, int div)
{
	if (nb <= 2)
		return (nb == 2);
	if (nb == 3)
		return (1);
	if (nb % div == 0)
		return (0);
	if (div > sqrt)
		return (1);
	else
		return (search_prime(nb, sqrt, div + 1));
}

int	is_prime(int nb)
{
	int	sqrt;

	sqrt = ft_sqrt(nb);
	return (search_prime(nb, sqrt, 2));
}

/*
 * Return the next prime after x, or x if x is prime
 */
int	next_prime(int nb)
{
	if (nb < 2)
		return (2);
	if (is_prime(nb))
		return (nb);
	else
		return (next_prime(nb + 1));
}

long	ft_pow(long a, long b)
{
	int			i;
	long long	result;

	// if (a < 0)
	//     return (0); // comment gerer ? c'est relou avec la division entière
	i = 0;
	result = 1;
	while (++i <= b)
		result *= a;
	if (result > __LONG_MAX__)
		return (0); // idk
	return (result);
}

// ---------- CI-DESSUS : METTRE DANS /libft -------------

static t_ht_item	HT_DELETED_ITEM = {NULL, NULL};

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
		ht_new = NULL; // ?
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
	size_t		i;
	size_t		j;
	// char	*tmp;
	char	**env;

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
				|| ht->items[j] == &HT_DELETED_ITEM))
			j++;
		if (!set_env_var(ht->items[j], env, i))
			return (NULL);
		i++;
		j++;
	}
	env[i] = NULL;
	return (env);
}
