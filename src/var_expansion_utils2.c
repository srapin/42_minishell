/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 20:32:47 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* value n'est pas free car est malloc dans la hash_map,
		et peut en re avoir besoin dans une autre commande*/
void	search_and_expand(t_ht_hash_table *ht, t_token_list **current,
		char *var, size_t dollar_index)
{
	char	*before_key;
	char	*after_value;
	char	*value;

	before_key = ft_substr((*current)->content, 0, dollar_index);
	value = ht_search(ht, var);
	after_value = ft_substr((*current)->content, dollar_index + ft_strlen(var)
			+ 1, (*current)->length);
	update_tk_content(current, before_key, value, after_value);
}

void	expand(t_ht_hash_table *ht, t_token_list **current, char *var,
		size_t dollar_index)
{
	if (!var)
		return ;
	if (!(*var))
	{
		if ((*current)->next && ((*current)->next->type == simple_quote
				|| (*current)->next->type == double_quote))
			remove_char(*current, dollar_index);
		else
			free(var);
		return ;
	}
	search_and_expand(ht, current, var, dollar_index);
}
