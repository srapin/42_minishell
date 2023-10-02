/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_second_sort.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:36:57 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	assess_filename(t_filename **filenames, char *filename_pos,
	t_filename **current_f)
{
	t_filename	*tmp;

	if (!filename_pos)
	{
		tmp = *current_f;
		*current_f = (*current_f)->next;
		del_filename(filenames, tmp);
		return (0);
	}
	return (1);
}

char	*get_filename_pos(t_token_list *current, char *f_pos,
		int next_wcard_index, int i)
{
	char	*to_find;
	char	*filename_pos;

	to_find = ft_substr(current->content, i, next_wcard_index - i);
	filename_pos = ft_search_str_in_str(f_pos,
			to_find);
	free(to_find);
	to_find = NULL;
	return (filename_pos);
}

int	parse_current_filename(t_filename **filenames, t_filename **current_f,
		t_token_list *current, size_t prefix_len)
{
	size_t		i;
	int			next_wcard_index;
	char		*filename_pos;

	i = prefix_len + 1;
	filename_pos = (*current_f)->filename + prefix_len;
	while (i < current->length)
	{
		next_wcard_index = get_next_wcard_index(current, i);
		if (next_wcard_index >= 0)
		{
			filename_pos = get_filename_pos(current, filename_pos,
					next_wcard_index, i);
			if (!assess_filename(filenames, filename_pos, current_f))
				return (0);
			i = next_wcard_index + 1;
			while (current->content[i] && current->content[i] == '*'
				&& !is_in_quotes(current, i))
				i++;
		}
		else
			return (1);
	}
	return (1);
}

/*
Passe le prefix. 
Tombe ainsi sur un '*'. Itère sur le d_name jusqu'a ce que 
l'on trouve dans le d_name la meme string que
celle suivant le '*' trouvé dans le mot originel,
	ou que l'on ait parcouru l'entièreté du d_name. 
Si ce qui suit le '*' est la chaine vide (ie on a "*\0"),
	arrête et recommence l'opération sur le d_name suivant.
Sinon : en continuant dans le mot originel,
	si tombe sur un autre '*' : si ce qui suit est non vide :
    Si a entièrement parcouru le d_name précédemment, supprime le d_name 
    Sinon,
	itère à nouveau sur le d_name jusqu'a trouver la 
	première apparition de la string recherchée,
    ou jusqu'à ce que la fin du d_name soit reached.
Etc...    

Sous chaine entre deux '*' : 
    taille de la sous chaine : next_wcard_index - (curr_wcard_index + 1)
    index du debut de la sous chaine : curr_wcard_index + 1
*/
void	second_sort(t_filename **filenames, t_token_list *current, char *prefix)
{
	size_t		prefix_len;
	t_filename	*current_f;

	if (!filenames || !(*filenames))
		return ;
	if (!(*filenames)->filename)
		return ;
	prefix_len = ft_strlen(prefix);
	current_f = (*filenames);
	while (current_f)
	{
		if (parse_current_filename(filenames, &current_f, current, prefix_len))
			current_f = current_f->next;
	}
}
