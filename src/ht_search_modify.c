/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ht_search_modify.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 17:22:28 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;

	i = 0;
	while (s1 && s2 && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
}

/* Searchs the key in the hash map, and replaces its value by the new_value.
Returns 1 in case of success
Returns 0 in case the key isn't in the hash map. 
char *new-value has already been malloc-ed here*/
int	ht_modify_value(t_ht_hash_table *ht, const char *key, char *new_value)
{
	size_t		attempts;
	size_t		index;
	t_ht_item	*current;

	attempts = 1;
	index = ht_get_hash(key, ht->size, 0);
	current = ht->items[index];
	while (current)
	{
		if (current->key && current->value)
		{
			if (!ft_strcmp(current->key, key))
			{
				free(current->value);
				current->value = (char *)new_value;
				return (1);
			}
		}
		index = ht_get_hash(key, ht->size, attempts);
		current = ht->items[index];
		attempts++;
	}
	return (0);
}

char	*ht_search(t_ht_hash_table *ht, const char *key)
{
	size_t		attempts;
	size_t		index;
	t_ht_item	*current;

	attempts = 1;
	index = ht_get_hash(key, ht->size, 0);
	current = ht->items[index];
	while (current)
	{
		if (current->key || current->value)
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
