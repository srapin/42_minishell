/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 19:58:14 by Helene            #+#    #+#             */
/*   Updated: 2023/06/14 17:36:33 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_TABLE_H
# define HASH_TABLE_H

#include <stdlib.h>
#include <string.h>
#include "lib.h"
//#include "libft.h"
#include <math.h> // a tej apres

#define HT_INITIAL_SIZE     50
// prime factore must be smaller than the ht's size (which should also be a prime)
#define HT_PRIME_FACTOR_A   23 // ?
#define HT_PRIME_FACTOR_B   11 // ?

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