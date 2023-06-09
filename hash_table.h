/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 19:58:14 by Helene            #+#    #+#             */
/*   Updated: 2023/06/23 14:53:21 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_TABLE_H
# define HASH_TABLE_H

#include <stdlib.h>
#include <string.h>
#include "lib.h"
#include <math.h>

#define HT_INITIAL_SIZE     100
#define HT_PRIME_FACTOR_A   419 // The variable a should be a prime number larger than the size of the alphabet. We're hashing ASCII strings, which has an alphabet size of 128, so we should choose a prime larger than that.
#define HT_PRIME_FACTOR_B   311

typedef struct  s_ht_item
{
    char *key;
    char *value;
}               t_ht_item;

typedef struct  s_ht_hash_table
{
    size_t      count;
    size_t      base_size;
    size_t      size;
    t_ht_item   **items;
}               t_ht_hash_table;

t_ht_hash_table     *ht_new(size_t size);
void                ht_resize(t_ht_hash_table *ht, size_t base_size);
void                ht_insert_item(t_ht_hash_table *ht, char *key, char *value);
t_ht_item           *ht_new_item(char *key, char *value);
char                *ht_search(t_ht_hash_table *ht, const char *key);
int                 ht_modify_value(t_ht_hash_table *ht, const char *key, char *new_value);
void                ht_delete(t_ht_hash_table *ht, const char *key);
void                ht_del_item(t_ht_item *item);
void                ht_del_hash_table(t_ht_hash_table *ht);

int                 ft_strcmp(const char *s1, const char *s2);
void                free_tab(char **tab);

size_t              ht_get_hash(const char *key, const size_t num_buckets, size_t attempts);

// ------ temporaire : a mettre dans la libft ici

long    ft_pow(long a, long b);
int	next_prime(int nb);
int	is_prime(int nb);

# endif