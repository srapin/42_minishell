/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 16:09:20 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/13 10:19:31 by Helene           ###   ########.fr       */
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

/*
 * Returns:
 *   1  - prime
 *   0  - not prime
 *   -1 - undefined (i.e. x < 2)
 */


int	search(int low, int high, int nbr)
{
	int long	mid;

	mid = (low + high) / 2;
	if (low > high)
		return (0);
	if ((mid * mid) == nbr)
		return (mid);
	if ((mid * mid) < nbr && ((mid + 1) * (mid + 1)) > nbr)
		return (mid +1);
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

// a le droit de declarer une static comme ca ?
static t_ht_item HT_DELETED_ITEM = {NULL, NULL};

t_ht_hash_table    *ht_new(size_t size)
{
    t_ht_hash_table *ht_new;

    ht_new = ft_calloc(sizeof(t_ht_hash_table), 1);
    if (!ht_new)
        return (NULL); // idk
    ht_new->count = 0; // utile sachant qu'on utilise calloc ?
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
            ht_insert_item(new_ht, current->key, current->value);
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

t_ht_item   *ht_new_item(char *key, char *value)
{
    t_ht_item *new;

    new = ft_calloc(sizeof(t_ht_item), 1);
    if (!new)
        return (NULL);
    new->key = key; // ft_strdup(key); (les arguments de la fonction sont deja malloc)
    new->value = value; // ft_strdup(value); (les arguments de la fonction sont deja malloc)
    return (new);
}

long    ft_pow(long a, long b)
{
    int i;
    long long result;

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

// à coder : moins chiant à utiliser ici que ft_strncmp
int     ft_strcmp(const char *s1, const char *s2)
{
    unsigned int	i;

    // if (ft_strlen(s1) != ft_strlen(s2))
    //     return (-1); 
	i = 0;
    
	while (s1 && s2 && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
}

/* Searchs the key in the hash map, and replaces its value by the new_value.
Returns 1 in case of success
Returns 0 in case the key isn't in the hash map. */
int    ht_modify_value(t_ht_hash_table *ht, const char *key, char *new_value) // new_value est deja malloc ici
{
    size_t      attempts;
    size_t      index;
    t_ht_item   *current;
    
    attempts = 1;
    index = ht_get_hash(key, ht->size, 0);
    current = ht->items[index];
    while (current)
    {
        if (current != &HT_DELETED_ITEM)
        {
            if (!ft_strcmp(current->key, key))
            {
                free(current->value);
                current->value = (char *)new_value; // new_value est deja malloc
                return (1);
            }
        }
        index = ht_get_hash(key, ht->size, attempts);
        current = ht->items[index];
        attempts++;
    }
    return (0);
}

char    *ht_search(t_ht_hash_table *ht, const char *key)
{
    size_t      attempts;
    size_t      index;
    t_ht_item   *current;
    
    attempts = 1;
    index = ht_get_hash(key, ht->size, 0);
    current = ht->items[index];
    while (current)
    {
        if (current != &HT_DELETED_ITEM)
        {
            if (!ft_strcmp(current->key, key))
                return (current->value);
        }
        index = ht_get_hash(key, ht->size, attempts);
        current = ht->items[index];
        attempts++;
    }
    return (NULL);
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
        if (current != &HT_DELETED_ITEM)
        {
            if (!ft_strcmp(current->key, key))
            {
                ht_del_item(current);
                ht->items[index] = &HT_DELETED_ITEM;  
                ht->count--;   
            }
        }
        attempts++;
        index = ht_get_hash(key, ht->size, attempts);
        current = ht->items[index];
    }
}

char    **hash_map_to_tab(t_ht_hash_table *ht)
{
    int i;
    int j;
    char *tmp;
    char **env;
    
    env = ft_calloc(sizeof(char *), (ht->count + 1));
    if (!env)
    {
        perror("malloc ");
        return (NULL);
    }
    i = 0;
    j = 0;
    // //dprintf(1, "top of while %i \n", ht->count);
    
    while (i < ht->count && j < ht->size)
    {
        // //dprintf(1, "step 0\n");
        
        while (!ht->items[j] || ht->items[j] == &HT_DELETED_ITEM)
            j++;
        env[i] = ft_calloc(sizeof(char), (ft_strlen(ht->items[j]->key) + ft_strlen(ht->items[j]->value) + 5)); // +2 : pour '=' et pour null-terminate
        // //dprintf(1, "step 1\n");
        if (!env[i])
        {
            perror("malloc ");
            free_tab(env);
            return (NULL);
        }
        // //dprintf(1, "step 2\n");
        tmp = ft_strjoin(ht->items[j]->key, "=");
        // //dprintf(1, "step 3\n");
        env[i] = ft_strjoin(tmp, ht->items[j]->value);
        // //dprintf(1, "step 4\n");
        free(tmp);
        // //dprintf(1, "%i, %s \n",i,  env[i]);
        i++;
        j++;
    }
    env[i] = NULL;
    // //dprintf(1, "end of while");
    return (env);
}
