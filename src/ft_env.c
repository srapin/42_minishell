/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 20:39:18 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_ht_hash_table *ht)
{
	size_t			i;

	i = 0;
	while (i < ht->size)
	{
		if (ht->items[i] && ht->items[i]->key && ht->items[i]->value)
			printf("%s=%s\n", ht->items[i]->key, ht->items[i]->value);
		i++;
	}
}

/* A gerer sans options et sans arguments */
int	ft_env(t_cmd *cmd, t_cmd *first)
{
	(void) first;
	if (cmd->val.args[1])
	{
		printf("Error : env : too many arguments given. Zero needed\n");
		return (1);
	}
	print_env(cmd->env);
	return (EXIT_OK);
}
